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
#include <ESPNowHandler.h>
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

ESPNowDriver::ESPNowDriver()
    : CommandHandler("espnow_drv", CLSID_MOT_ESPNOW1),
      Thread("ESPNOW-RECVR", ESPNOW_THREAD_MEM, ESPNOW_THREAD_PRIO) {

  if (s_ESPNowQueue == nullptr) {
    s_ESPNowQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(EventCallbackData));
  }

  if (s_ESPNowQueue == nullptr) {
    ESP_LOGE(TAG, "failed to create espnow queue");
  }

  auto *espnow_handler = ESPNowHandler::get();
  espnow_handler->register_recv_queue(s_ESPNowQueue);

  restoreFlash();

  this->registerCommands();
  this->Start();
}

ESPNowDriver::~ESPNowDriver() { this->turn(0); }

void ESPNowDriver::registerCommands() {
  CommandHandler::registerCommands();
  registerCommand("pos_update", ESPNowDriver_commands::pos_update_rate,
                  "Get pos update rate", CMDFLAG_GET);
  registerCommand("torq_update", ESPNowDriver_commands::torque_update_rate,
                  "Get torque update rate", CMDFLAG_GET);
}

void ESPNowDriver::restoreFlash() {}

void ESPNowDriver::saveFlash() {}

void ESPNowDriver::Run() {
  while (true) {
    uint32_t send_len = 0;

    EventCallbackData event;
    if (xQueueReceive(s_ESPNowQueue, &event, portMAX_DELAY) != pdTRUE) {
      // vTaskDelay(pdMS_TO_TICKS(1));
      vTaskDelay(1);
      continue;
    }

    uint8_t *data = &event.receive.data[0];
    CmdType cmd_type = static_cast<CmdType>(data[0]);
    uint8_t *address = event.receive.mac_addr;

    switch (event.type) {
    case EventType::Receive: {
      if (cmd_type == CmdType::EncoderPos) {
        float pos = *(float *)&data[1];
        mPos = pos;
        mPosUpdateAvg.addValue((uint32_t)(HAL_GetTick() - mLastPosUpdate));
        mLastPosUpdate = HAL_GetTick();
      }
    } break;
    case EventType::Send: {
    } break;
    }
  }
}

void ESPNowDriver::stopMotor() { this->turn(0); }

void ESPNowDriver::startMotor() {}

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
  } else {
    mTorqueUpdateAvg.addValue((uint32_t)(HAL_GetTick() - mLastTorqueUpdate));
    mLastTorqueUpdate = HAL_GetTick();
  }
}

CommandStatus ESPNowDriver::command(const ParsedCommand &cmd,
                                    std::vector<CommandReply> &replies) {
  switch (static_cast<ESPNowDriver_commands>(cmd.cmdId)) {
  case ESPNowDriver_commands::pos_update_rate: {
    if (cmd.type == CMDtype::get) {
      float periodAvg = mPosUpdateAvg.getAverage();
      if ((HAL_GetTick() - mLastPosUpdate) > 1000 || periodAvg == 0) {
        // Reset average
        mPosUpdateAvg.clear();
        replies.push_back(0.0f);
      } else {
        replies.push_back(1000.0 / periodAvg);
      }
    } else {
      return CommandStatus::ERR;
    }
  } break;
  case ESPNowDriver_commands::torque_update_rate: {
    if (cmd.type == CMDtype::get) {
      float periodAvg = mTorqueUpdateAvg.getAverage();
      if ((HAL_GetTick() - mLastTorqueUpdate) > 1000 || periodAvg == 0) {
        // Reset average
        mTorqueUpdateAvg.clear();
        replies.push_back(0.0f);
      } else {
        replies.push_back(1000.0 / periodAvg);
      }
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
