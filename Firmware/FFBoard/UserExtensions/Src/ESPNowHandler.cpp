/*
 * ESPNowHandler.cpp
 *
 *  Created on: March 22, 2025
 *      Author: Jatinder
 */
#include "target_constants.h"
#ifdef ESPNOW
#include "esp_log.h"
#include "esp_now.h"
#include "esp_timer.h"
#include <ESPNowHandler.h>
#include <math.h>

static const char *TAG = "ESPNOW";

static QueueHandle_t s_ESPNowQueue = nullptr;

static std::vector<uint8_t> s_recv_buffer;
static uint32_t s_recv_offset = 0;
constexpr uint32_t RECV_BUFFER_SIZE = 1024;

bool ESPNowHandler::inUse = false;
ClassIdentifier ESPNowHandler::info = {
    .name = "ESPNow",
    .id = CLSID_ESPNOW,
};

const ClassIdentifier ESPNowHandler::getInfo() { return info; }

bool ESPNowHandler::isCreatable() {
  return !ESPNowHandler::inUse; // Creatable if not already in use for example
                                // by another axis
}

void ESPNowHandler::add_client(const uint8_t *address) {
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

void ESPNowHandler::remove_client(
    const std::array<uint8_t, ESP_NOW_ETH_ALEN> &address) {
  ESP_LOGI(TAG, "Removing to %02X:%02X:%02X:%02X:%02X:%02X", address[0],
           address[1], address[2], address[3], address[4], address[5]);

  if (!esp_now_is_peer_exist(address.data())) {
    return;
  }

  esp_now_del_peer(address.data());
  mConnected = false;
}

void ESPNowHandler::send_connect_ack() {
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

void ESPNowHandler::espnow_recv_cb(const esp_now_recv_info_t *recv_info,
                                   const uint8_t *data, int len) {
  EventCallbackData event;
  event.type = EventType::Receive;
  uint8_t *mac_addr = recv_info->src_addr;

  if (mac_addr == NULL) {
    ESP_LOGE(TAG, "Send cb arg error");
    return;
  }

  // maybe have a buffer per queue?
  event.receive.data = &s_recv_buffer[s_recv_offset];
  event.receive.len = len;

  memcpy(event.receive.mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
  memcpy(event.receive.data, data, len);

  if (s_recv_offset + len > s_recv_buffer.size()) {
    s_recv_offset = 0;
  }

  for (const auto &queue : sRegisteredQueues) {
    if (xQueueSend(queue, &event, ESPNOW_MAXDELAY) != pdTRUE) {
      ESP_LOGW(TAG, "Send queue fail");
    }
  }
}

ESPNowHandler::ESPNowHandler()
    : CommandHandler("espnow", CLSID_ESPNOW),
      Thread("ESPNOW-HANDLER", ESPNOW_THREAD_MEM, ESPNOW_THREAD_PRIO) {

  if (s_ESPNowQueue == nullptr) {
    s_ESPNowQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(EventCallbackData));
  }

  if (s_ESPNowQueue == nullptr) {
    ESP_LOGE(TAG, "failed to create espnow queue");
  }

  if (s_recv_buffer.size() < RECV_BUFFER_SIZE) {
    s_recv_buffer.resize(RECV_BUFFER_SIZE);
  }

  register_recv_queue(s_ESPNowQueue);

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
  inUse = true;
}

ESPNowHandler::~ESPNowHandler() {
  inUse = false;
  // unregister_recv_queue(s_ESPNowQueue);
}

void ESPNowHandler::registerCommands() {
  CommandHandler::registerCommands();
  registerCommand("lsdev", ESPNowHandler_commands::list_available_devices,
                  "List available devices", CMDFLAG_GET);
}

void ESPNowHandler::register_recv_queue(QueueHandle_t queue) {
  sRegisteredQueues.push_back(queue);
}

void ESPNowHandler::restoreFlash() {

  // read the saved mac address of the controller
  std::array<uint8_t, ESP_NOW_ETH_ALEN> controller_addr;

  uint16_t *data = reinterpret_cast<uint16_t *>(controller_addr.data());

  Flash_Read(ESPNOW_DRV_CONTROLLER_ADDR_0, &data[0]);
  Flash_Read(ESPNOW_DRV_CONTROLLER_ADDR_1, &data[1]);
  Flash_Read(ESPNOW_DRV_CONTROLLER_ADDR_2, &data[2]);

  add_client(controller_addr.data());
}

void ESPNowHandler::saveFlash() {
  constexpr std::array<uint8_t, ESP_NOW_ETH_ALEN> CONTROLLER_ADDR = {
      0x80, 0x65, 0x99, 0xEB, 0x03, 0xF9};
  const uint16_t *data =
      reinterpret_cast<const uint16_t *>(CONTROLLER_ADDR.data());

  Flash_Write(ESPNOW_DRV_CONTROLLER_ADDR_0, data[0]);
  Flash_Write(ESPNOW_DRV_CONTROLLER_ADDR_1, data[1]);
  Flash_Write(ESPNOW_DRV_CONTROLLER_ADDR_2, data[2]);
}

void ESPNowHandler::Run() {
  while (true) {
    uint32_t send_len = 0;

    EventCallbackData event;
    if (xQueueReceive(s_ESPNowQueue, &event, portMAX_DELAY) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(1));
      continue;
    }

    const uint8_t *data = &event.receive.data[0];
    CmdType cmd_type = static_cast<CmdType>(data[0]);
    const uint8_t *address = event.receive.mac_addr;

    switch (event.type) {
    case EventType::Receive: {
      if (cmd_type == CmdType::Connect) {
        ESP_LOGI(TAG, "Got connect request from %02X:%02X:%02X:%02X:%02X:%02X",
                 address[0], address[1], address[2], address[3], address[4],
                 address[5]);
        mConnected = true;
        uint8_t protocol_ver = data[1];
        if (protocol_ver == ESPNOW_PROTOCOL_VERSION) {
          add_client(address);
          send_connect_ack();
        }
      }
    } break;
    case EventType::Send: {
    } break;
    }
  }
}

CommandStatus ESPNowHandler::command(const ParsedCommand &cmd,
                                     std::vector<CommandReply> &replies) {
  switch (static_cast<ESPNowHandler_commands>(cmd.cmdId)) {
  case ESPNowHandler_commands::list_available_devices: {
    if (cmd.type == CMDtype::get) {
      // TODO: pack multiple
      int64_t packed = 0;

      packed |= (int64_t)mControllerAddress[0] << 0;
      packed |= (int64_t)mControllerAddress[1] << 8;
      packed |= (int64_t)mControllerAddress[2] << 16;
      packed |= (int64_t)mControllerAddress[3] << 24;
      packed |= (int64_t)mControllerAddress[4] << 32;
      packed |= (int64_t)mControllerAddress[5] << 40;

      replies.push_back(packed);
    } else {
      return CommandStatus::ERR;
    }
  } break;
  default:
    return CommandStatus::NOT_FOUND;
  }

  return CommandStatus::OK;
}

#endif
