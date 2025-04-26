/*
 * ESBDriver.h
 *
 *  Created on: April 26, 2025
 *      Author: Jatinder
 */

#pragma once
#include "CommandHandler.h"
#include "Encoder.h"
#include "FastAvg.h"
#include "MotorDriver.h"
#include "PersistentStorage.h"
#include "thread.hpp"

#ifdef ESB_CONNECTION
#define ESB_DRIVER_THREAD_MEM 512
#define ESB_DRIVER_THREAD_PRIO 25 // Must be higher than main thread

class ESBDriver : public MotorDriver,
                  public Encoder,
                  public PersistentStorage,
                  public CommandHandler,
                  cpp_freertos::Thread {
public:
  ESBDriver();
  ~ESBDriver();

  const ClassIdentifier getInfo();
  static bool isCreatable();
  static ClassIdentifier info;
  static bool inUse;

  void turn(int16_t power) override;
  void stopMotor() override;
  void startMotor() override;
  Encoder *getEncoder() override;
  bool hasIntegratedEncoder() { return true; }

  bool motorReady() override;

  float getPos_f() override;
  uint32_t getCpr() override;
  int32_t getPos() override;
  void setPos(int32_t pos) override;
  EncoderType getEncoderType() override;

  void saveFlash() override;    // Write to flash here
  void restoreFlash() override; // Load from flash

  CommandStatus command(const ParsedCommand &cmd,
                        std::vector<CommandReply> &replies) override;
  void registerCommands();

  void Run();

private:
  float mPos = 0.0f;
  float mPosOffset = 0.0f;
};

#endif