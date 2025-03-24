/*
 * ESPNowButtons.h
 *
 *  Created on: 03.23.2025
 *      Author: Jatinder
 */

#include "ESPNowButtons.h"
#include "esp_log.h"
#include "math.h"

#ifdef ESPNOW

static const char *TAG = "ESPNOWBTN";
static QueueHandle_t s_ESPNowQueue = nullptr;

ClassIdentifier ESPNowButtons::info = {
    .name = "ESPNow Buttons",
    .id = CLSID_BTN_ESPNOW,
};
const ClassIdentifier ESPNowButtons::getInfo() { return info; }

ESPNowButtons::ESPNowButtons()
    : CommandHandler("espnowbtn", CLSID_BTN_ESPNOW, 0),
      Thread("ESPNOW-BTNS", ESPNOW_THREAD_MEM, ESPNOW_THREAD_PRIO) {

  if (s_ESPNowQueue == nullptr) {
    s_ESPNowQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(EventCallbackData));
  }

  if (s_ESPNowQueue == nullptr) {
    ESP_LOGE(TAG, "failed to create espnow queue");
  }

  auto *espnow_handler = ESPNowHandler::get();
  espnow_handler->register_recv_queue(s_ESPNowQueue);

  ButtonSource::btnnum = 8;
  registerCommands();

  registerCommand("invert", ESPNowButton_commands::invert, "Invert buttons",
                  CMDFLAG_GET | CMDFLAG_SET);

  restoreFlash();
  Start();
}

ESPNowButtons::~ESPNowButtons() {}

void ESPNowButtons::saveFlash() {}

void ESPNowButtons::restoreFlash() {}

uint8_t ESPNowButtons::readButtons(uint64_t *buf) {
  if (invert) {
    *buf = ~currentButtons;
  } else {
    *buf = currentButtons;
  }
  *buf &= (uint64_t)pow<uint64_t>(2, btnnum) - (uint64_t)1;
  return ButtonSource::btnnum;
}

void ESPNowButtons::Run() {
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
        currentButtons = controller_state->buttons;
      }
    } break;
    case EventType::Send: {
    } break;
    }
  }
}

CommandStatus ESPNowButtons::command(const ParsedCommand &cmd,
                                     std::vector<CommandReply> &replies) {

  switch (static_cast<ESPNowButton_commands>(cmd.cmdId)) {
  case ESPNowButton_commands::invert:
    return handleGetSet(cmd, replies, this->invert);
    break;
  default:
    return CommandStatus::NOT_FOUND;
  }
  return CommandStatus::OK;
}

#endif
