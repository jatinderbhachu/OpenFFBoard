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
  float dir = 0.01f;
  while (true) {
#if 0
    mPos = ESBConnection::get()->controller_state.encoder_angle;
#else
    // mPos = sin(k_uptime_get() / 200.0f);
    mPos += dir;

    if (abs(mPos) > 1.0f) {
      dir *= -1.0f;
    }
#endif
    k_sleep(K_USEC(500));
  }
}

CommandStatus ESBDriver::command(const ParsedCommand &cmd,
                                 std::vector<CommandReply> &replies) {
  return CommandStatus::OK;
}

void ESBDriver::turn(int16_t power) {}
void ESBDriver::stopMotor() {}
void ESBDriver::startMotor() {}
Encoder *ESBDriver::getEncoder() { return this; }

bool ESBDriver::motorReady() { return true; }

float ESBDriver::getPos_f() { return mPos / (2.0f * M_PI); }
uint32_t ESBDriver::getCpr() { return 1024; }
int32_t ESBDriver::getPos() { return getCpr() * getPos_f(); }
void ESBDriver::setPos(int32_t) {}
EncoderType ESBDriver::getEncoderType() { return EncoderType::absolute; }

#endif