/*
 * ESPNowDriver.cpp
 *
 *  Created on: March 22, 2025
 *      Author: Jatinder
 */
#include "target_constants.h"
#ifdef ESPNOW
#include "esp_log.h"
#include "esp_now.h"
#include "esp_timer.h"
#include <ESPNowDriver.h>
#include <math.h>

static const char *TAG = "ESPNOW";

static QueueHandle_t s_ESPNowQueue = nullptr;

bool ESPNowDriver_1::inUse = false;
ClassIdentifier ESPNowDriver_1::info = {
    .name = "ESPNowDriver",
    .id = CLSID_MOT_ESPNOW1,
};

const ClassIdentifier ESPNowDriver_1::getInfo() { return info; }

bool ESPNowDriver_1::isCreatable() {
  return !ESPNowDriver_1::inUse; // Creatable if not already in use for example
                                 // by another axis
}

void ESPNowDriver::add_client(const uint8_t *address) {
  ESP_LOGI(TAG, "Connecting to %02X:%02X:%02X:%02X:%02X:%02X", address[0],
           address[1], address[2], address[3], address[4], address[5]);

  memcpy(mControllerAddress.data(), address, mControllerAddress.size());

  if (esp_now_is_peer_exist(mControllerAddress.data())) {
    return;
  }

  esp_now_peer_info_t peer;
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  peer.channel = ESPNOW_CHANNEL;
  peer.ifidx = ESPNOW_WIFI_IF;
  peer.encrypt = false;

  memcpy(peer.peer_addr, mControllerAddress.data(), ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
}

void ESPNowDriver::remove_client(
    const std::array<uint8_t, ESP_NOW_ETH_ALEN> &address) {
  ESP_LOGI(TAG, "Removing to %02X:%02X:%02X:%02X:%02X:%02X", address[0],
           address[1], address[2], address[3], address[4], address[5]);

  if (!esp_now_is_peer_exist(address.data())) {
    return;
  }

  esp_now_del_peer(address.data());
  mConnected = false;
}

void ESPNowDriver::send_connect_ack() {
  uint8_t send_buf[ESPNOW_MAX_DATA_SIZE];
  uint32_t send_len = 0;
  send_buf[send_len++] = (uint8_t)CmdType::Connect_ack;
  if (esp_now_send(mControllerAddress.data(), &send_buf[0], send_len) !=
      ESP_OK) {
    const auto &address = mControllerAddress;
    ESP_LOGE(TAG, "Send error, dst = %02X:%02X:%02X:%02X:%02X:%02X", address[0],
             address[1], address[2], address[3], address[4], address[5]);
  }
}

void ESPNowDriver::espnow_recv_cb(const esp_now_recv_info_t *recv_info,
                                  const uint8_t *data, int len) {
  EventCallbackData event;
  event.type = EventType::Receive;
  uint8_t *mac_addr = recv_info->src_addr;

  if (mac_addr == NULL) {
    ESP_LOGE(TAG, "Send cb arg error");
    return;
  }

  memcpy(event.receive.mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
  event.receive.data = (uint8_t *)malloc(len);
  event.receive.len = len;
  memcpy(event.receive.data, data, len);

  if (xQueueSend(s_ESPNowQueue, &event, ESPNOW_MAXDELAY) != pdTRUE) {
    ESP_LOGW(TAG, "Send queue fail");
  }
}

ESPNowDriver::ESPNowDriver()
    : CommandHandler("espnow", CLSID_MOT_ESPNOW1),
      Thread("ESPNOW-RECVR", ESPNOW_THREAD_MEM, ESPNOW_THREAD_PRIO) {

  if (s_ESPNowQueue == nullptr) {
    s_ESPNowQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(EventCallbackData));
  }

  if (s_ESPNowQueue == nullptr) {
    ESP_LOGE(TAG, "failed to create espnow queue");
  }

  // ESP_ERROR_CHECK(esp_now_register_send_cb(this->espnow_send_cb));
  ESP_ERROR_CHECK(esp_now_register_recv_cb(this->espnow_recv_cb));

  /* Add broadcast peer information to peer list. */
  esp_now_peer_info_t peer;
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  peer.channel = ESPNOW_CHANNEL;
  peer.ifidx = ESPNOW_WIFI_IF;
  peer.encrypt = false;
  memcpy(peer.peer_addr, ESPNOW_BROADCAST_ADDR, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));

  restoreFlash();

  this->registerCommands();
  this->Start();
}

ESPNowDriver::~ESPNowDriver() { this->turn(0); }

void ESPNowDriver::registerCommands() { CommandHandler::registerCommands(); }

void ESPNowDriver::restoreFlash() {

  // read the saved mac address of the controller
  std::array<uint8_t, ESP_NOW_ETH_ALEN> controller_addr;

  uint16_t *data = reinterpret_cast<uint16_t *>(controller_addr.data());

  Flash_Read(ESPNOW_DRV_CONTROLLER_ADDR_0, &data[0]);
  Flash_Read(ESPNOW_DRV_CONTROLLER_ADDR_1, &data[1]);
  Flash_Read(ESPNOW_DRV_CONTROLLER_ADDR_2, &data[2]);

  add_client(controller_addr.data());
}

void ESPNowDriver::saveFlash() {
  constexpr std::array<uint8_t, ESP_NOW_ETH_ALEN> CONTROLLER_ADDR = {
      0x80, 0x65, 0x99, 0xEB, 0x03, 0xF9};
  const uint16_t *data =
      reinterpret_cast<const uint16_t *>(CONTROLLER_ADDR.data());

  Flash_Write(ESPNOW_DRV_CONTROLLER_ADDR_0, data[0]);
  Flash_Write(ESPNOW_DRV_CONTROLLER_ADDR_1, data[1]);
  Flash_Write(ESPNOW_DRV_CONTROLLER_ADDR_2, data[2]);
}

void ESPNowDriver::Run() {
  while (true) {
    uint32_t send_len = 0;

    EventCallbackData event;
    if (xQueueReceive(s_ESPNowQueue, &event, portMAX_DELAY) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(1));
      continue;
    }

    uint8_t *data = &event.receive.data[0];
    CmdType cmd_type = static_cast<CmdType>(data[0]);
    uint8_t *address = event.receive.mac_addr;

    switch (event.type) {
    case EventType::Receive: {
      if (cmd_type == CmdType::Connect) {
        ESP_LOGI(TAG, "Got connect request from %02X:%02X:%02X:%02X:%02X:%02X",
                 address[0], address[1], address[2], address[3], address[4],
                 address[5]);
        mConnected = true;
        uint8_t protocol_ver = data[1];
        if(protocol_ver == ESPNOW_PROTOCOL_VERSION) {
          add_client(address);
          send_connect_ack();
        }
      } else if (cmd_type == CmdType::EncoderPos) {
        float pos = *(float *)&data[1];
        mPos = pos;
      }

      free(data);
    } break;
    case EventType::Send: {
      // ESP_LOGW(TAG, "send data cb");
    } break;
    }
  }
}

void ESPNowDriver::stopMotor() {
  mActive = false;
  this->turn(0);
}

void ESPNowDriver::startMotor() { mActive = true; }

Encoder *ESPNowDriver::getEncoder() { return static_cast<Encoder *>(this); }

/**
 * Must be in encoder cpr if not just used to zero the axis
 */
void ESPNowDriver::setPos(int32_t pos) {
  // Only change encoder count internally as offset
  mPosOffset = mLastOPos - ((float)pos / (float)getCpr());
}

float ESPNowDriver::getPos_f() { return mPos / (2.0f * M_PI); }

bool ESPNowDriver::motorReady() {
  if (!mConnected) {
    return false;
  }

  return true;
}
int32_t ESPNowDriver::getPos() { return getCpr() * getPos_f(); }

uint32_t ESPNowDriver::getCpr() { return 1024; }

EncoderType ESPNowDriver::getEncoderType() { return EncoderType::absolute; }

void ESPNowDriver::turn(int16_t power) {
  uint8_t send_buf[ESPNOW_MAX_DATA_SIZE];
  uint32_t send_len = 0;
  send_buf[send_len++] = (uint8_t)CmdType::ApplyTorque;
  memcpy(&send_buf[send_len], &power, sizeof(power));
  send_len += sizeof(power);

  // ESP_LOGI(TAG, "turn %d\n", power);

  if (esp_now_send(mControllerAddress.data(), &send_buf[0], send_len) !=
      ESP_OK) {
    // const auto& address = mControllerAddress;
    // ESP_LOGE(TAG, "Send error, turn(), dst = %02X:%02X:%02X:%02X:%02X:%02X",
    // address[0],
    //         address[1], address[2], address[3], address[4], address[5]);
  }
}

CommandStatus ESPNowDriver::command(const ParsedCommand &cmd,
                                    std::vector<CommandReply> &replies) {
  return CommandStatus::OK;
}

#endif
