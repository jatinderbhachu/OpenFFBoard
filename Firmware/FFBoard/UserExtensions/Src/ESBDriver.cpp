/*
 * ESBDriver.cpp
 *
 *  Created on: April 22, 2025
 *      Author: Jatinder
 */
#include "target_constants.h"
#ifdef ESB_CONNECTION

#include "ClassIDs.h"
#include <ESBConnection.h>
#include <ESBDriver.h>
#include <zephyr/kernel.h>

#include "math.h"

bool ESBDriver::inUse = false;
ClassIdentifier ESBDriver::info = {
    .name = "ESBDriver",
    .id = CLSID_MOT_ESB,
};

const ClassIdentifier ESBDriver::getInfo() { return info; }

bool ESBDriver::isCreatable() {
  return !ESBDriver::inUse; // Creatable if not already in use for example
                            // by another axis
}

ESBDriver::ESBDriver()
    : CommandHandler("esb_drv", CLSID_MOT_ESB),
      Thread("ESB-DRV", ESB_DRIVER_THREAD_MEM, ESB_DRIVER_THREAD_PRIO) {
  this->registerCommands();
  this->Start();
  inUse = true;
}

ESBDriver::~ESBDriver() { inUse = false; }

void ESBDriver::registerCommands() { CommandHandler::registerCommands(); }

void ESBDriver::restoreFlash() {}

void ESBDriver::saveFlash() {}

void ESBDriver::Run() {
  while (true) {
    k_sleep(K_MSEC(5000));

    // const ControllerState &state = ESBConnection::get_controller_state();
    // mPos = state.encoder_angle;
    // k_sleep(K_USEC(500));
  }
}

CommandStatus ESBDriver::command(const ParsedCommand &cmd,
                                 std::vector<CommandReply> &replies) {
  return CommandStatus::OK;
}

void ESBDriver::turn(int16_t power) {
  int err = ESBConnection::get()->send_data((uint8_t *)&power, sizeof(power));
}
void ESBDriver::stopMotor() {}
void ESBDriver::startMotor() {}
Encoder *ESBDriver::getEncoder() { return this; }

bool ESBDriver::motorReady() { return true; }

float ESBDriver::getPos_f() {

  const ControllerState &state = ESBConnection::get_controller_state();
  mPos = state.encoder_angle;

  return mPos / (2.0f * M_PI);
}
uint32_t ESBDriver::getCpr() { return 1024; }
int32_t ESBDriver::getPos() { return getCpr() * getPos_f(); }
void ESBDriver::setPos(int32_t) {}
EncoderType ESBDriver::getEncoderType() { return EncoderType::absolute; }

#endif