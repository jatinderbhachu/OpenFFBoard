/*
 * ESPNowAnalog.h
 *
 *  Created on: 03.23.2025
 *      Author: Jatinder
 */

#include "ESPNowAnalog.h"
#include "esp_log.h"
#include "math.h"

#ifdef ESPNOW

static const char *TAG = "ESPNOW_ANALOG";
static QueueHandle_t s_ESPNowQueue = nullptr;

ClassIdentifier ESPNowAnalog::info = {
    .name = "ESPNOW Analog",
    .id = CLSID_ANALOG_ESPNOW,
};
const ClassIdentifier ESPNowAnalog::getInfo() { return info; }

ESPNowAnalog::ESPNowAnalog()
    : CommandHandler("espnow_analog", CLSID_ANALOG_ESPNOW, 0),
      Thread("ESPNOW-ANALOG", ESPNOW_THREAD_MEM, ESPNOW_THREAD_PRIO) {

  if (s_ESPNowQueue == nullptr) {
    s_ESPNowQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(EventCallbackData));
  }

  if (s_ESPNowQueue == nullptr) {
    ESP_LOGE(TAG, "failed to create espnow queue");
  }

  auto *espnow_handler = ESPNowHandler::get();
  espnow_handler->register_recv_queue(s_ESPNowQueue);

  CommandHandler::registerCommands();
  restoreFlash();
  this->buf.resize(axes, 0);
  Start();
}

ESPNowAnalog::~ESPNowAnalog() {}

void ESPNowAnalog::saveFlash() {}

void ESPNowAnalog::restoreFlash() {}

CommandStatus ESPNowAnalog::command(const ParsedCommand &cmd,
                                    std::vector<CommandReply> &replies) {
  return CommandStatus::OK;
}

void ESPNowAnalog::Run() {
  while (true) {
    uint32_t send_len = 0;

    EventCallbackData event;
    if (xQueueReceive(s_ESPNowQueue, &event, portMAX_DELAY) != pdTRUE) {
      // vTaskDelay(pdMS_TO_TICKS(1));
      vTaskDelay(1);
      continue;
    }

    CmdType cmd_type = static_cast<CmdType>(event.receive.data[0]);
    uint8_t *data = &event.receive.data[1];
    uint8_t *address = event.receive.mac_addr;

    switch (event.type) {
    case EventType::Receive: {
      if (cmd_type == CmdType::ControllerState) {
        const ControllerState *controller_state =
            reinterpret_cast<const ControllerState *>(&data[0]);

        this->buf[0] = controller_state->axis1;
        this->buf[1] = controller_state->axis2;
      }
    } break;
    case EventType::Send: {
    } break;
    }
  }
}

#endif
