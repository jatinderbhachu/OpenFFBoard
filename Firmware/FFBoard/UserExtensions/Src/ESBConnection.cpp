/*
 * ESBConnection.cpp
 *
 *  Created on: April 22, 2025
 *      Author: Jatinder
 */
#include "target_constants.h"
#ifdef ESB_CONNECTION

#include "ClassIDs.h"
#include <ESBConnection.h>
#include <dk_buttons_and_leds.h>
#include <esb.h>
#include <math.h>
#include <nrf.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/nrf_clock_control.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/irq.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/types.h>

#if defined(CONFIG_CLOCK_CONTROL_NRF2)
#include <hal/nrf_lrcconf.h>
#endif

static struct esb_payload rx_payload;
K_MSGQ_DEFINE(esb_msgq, sizeof(struct esb_payload), 10, 1);

bool ESBConnection::inUse = false;
ClassIdentifier ESBConnection::info = {
    .name = "ESBConnection",
    .id = CLSID_ESB,
};

const ClassIdentifier ESBConnection::getInfo() { return info; }

bool ESBConnection::isCreatable() {
  return !ESBConnection::inUse; // Creatable if not already in use for example
                                // by another axis
}

void event_handler(struct esb_evt const *event) {
  switch (event->evt_id) {
  case ESB_EVENT_TX_SUCCESS:
    break;
  case ESB_EVENT_TX_FAILED:
    break;
  case ESB_EVENT_RX_RECEIVED:
    if (esb_read_rx_payload(&rx_payload) == 0) {
      while (k_msgq_put(&esb_msgq, &rx_payload, K_NO_WAIT) != 0) {
        /* message queue is full: purge old data & try again */
        k_msgq_purge(&esb_msgq);
      }
    } else {
      // err
    }
    break;
  }
}

ESBConnection::ESBConnection()
    : CommandHandler("esb_con", CLSID_ESB),
      Thread("ESB-CON", ESB_THREAD_MEM, ESB_THREAD_PRIO) {
  int err;
  uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
  uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
  uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8};

  struct esb_config config = ESB_DEFAULT_CONFIG;

  config.protocol = ESB_PROTOCOL_ESB_DPL;
  config.bitrate = ESB_BITRATE_2MBPS;
  config.mode = ESB_MODE_PRX;
  config.event_handler = event_handler;
  config.selective_auto_ack = true;
  if (IS_ENABLED(CONFIG_ESB_FAST_SWITCHING)) {
    config.use_fast_ramp_up = true;
  }

  err = esb_init(&config);
  err = esb_set_base_address_0(base_addr_0);
  err = esb_set_base_address_1(base_addr_1);
  err = esb_set_prefixes(addr_prefix, ARRAY_SIZE(addr_prefix));

  esb_start_rx();

  this->registerCommands();
  this->Start();
  inUse = true;
}

ESBConnection::~ESBConnection() { inUse = false; }

void ESBConnection::registerCommands() { CommandHandler::registerCommands(); }

void ESBConnection::restoreFlash() {}

void ESBConnection::saveFlash() {}

void ESBConnection::Run() {

  struct esb_payload payload;
  while (true) {
    k_msgq_get(&esb_msgq, &payload, K_FOREVER);
    controller_state = *reinterpret_cast<ControllerState *>(payload.data);
    Delay(100);
  }
}

CommandStatus ESBConnection::command(const ParsedCommand &cmd,
                                     std::vector<CommandReply> &replies) {
  switch (static_cast<ESBConnection_commands>(cmd.cmdId)) {
  case ESBConnection_commands::list_available_devices: {
    if (cmd.type == CMDtype::get) {
      replies.push_back(0);
    } else {
      return CommandStatus::ERR;
    }
  } break;
  case ESBConnection_commands::rx_rate: {
    if (cmd.type == CMDtype::get) {
      float periodAvg = mRXAvg.getAverage();
      if ((HAL_GetTick() - mLastRX) > 1000 || periodAvg == 0) {
        // Reset average
        mRXAvg.clear();
        replies.push_back(0.0f);
      } else {
        replies.push_back(1000.0 / periodAvg);
      }
    }
  } break;
  default:
    return CommandStatus::NOT_FOUND;
  }

  return CommandStatus::OK;
}

#endif