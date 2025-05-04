/*
 * ESBAnalog.h
 *
 *  Created on: 03.23.2025
 *      Author: Jatinder
 */

#include "ESBAnalog.h"
#include "ESBConnection.h"
#include "math.h"

#ifdef ESB_CONNECTION

ClassIdentifier ESBAnalog::info = {
    .name = "ESB Analog",
    .id = CLSID_ANALOG_ESB,
};
const ClassIdentifier ESBAnalog::getInfo() { return info; }

ESBAnalog::ESBAnalog() : CommandHandler("esb_analog", CLSID_ANALOG_ESB, 0) {
  CommandHandler::registerCommands();
  restoreFlash();
  this->buf.resize(axes, 0);
}

ESBAnalog::~ESBAnalog() {}

void ESBAnalog::saveFlash() {}

void ESBAnalog::restoreFlash() {}

CommandStatus ESBAnalog::command(const ParsedCommand &cmd,
                                 std::vector<CommandReply> &replies) {
  return CommandStatus::OK;
}

int32_t mapRange(int32_t value, int32_t old_min, int32_t old_max,
                 int32_t new_min, int32_t new_max) {
  return new_min +
         (value - old_min) * (new_max - new_min) / (old_max - old_min);
}

std::vector<int32_t> *ESBAnalog::getAxes() {
  const ControllerState &state = ESBConnection::get_controller_state();

  this->buf[0] = mapRange(state.axis1, 0, 4096, INT16_MIN, INT16_MAX);
  this->buf[1] = mapRange(state.axis2, 0, 4096, INT16_MIN, INT16_MAX);
  return &this->buf;
}

#endif