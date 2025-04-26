/*
 * ESBConnection.h
 *
 *  Created on: April 22, 2025
 *      Author: Jatinder
 */

#pragma once
#include "CommandHandler.h"
#include "FastAvg.h"
#include "PersistentStorage.h"
#include "thread.hpp"

#ifdef ESB_CONNECTION
#define ESB_THREAD_MEM 512
#define ESB_THREAD_PRIO 25 // Must be higher than main thread

enum class ESBConnection_commands : uint32_t {
  list_available_devices,
  rx_rate,
};

struct ControllerState {
  float encoder_angle;
  uint16_t axis1;
  uint16_t axis2;
  uint8_t buttons;
};

class ESBConnection : public PersistentStorage,
                      public CommandHandler,
                      cpp_freertos::Thread {
public:
  ESBConnection();
  ~ESBConnection();

  const ClassIdentifier getInfo();
  static bool isCreatable();
  static ClassIdentifier info;
  static bool inUse;

  void saveFlash() override;    // Write to flash here
  void restoreFlash() override; // Load from flash

  CommandStatus command(const ParsedCommand &cmd,
                        std::vector<CommandReply> &replies) override;
  void registerCommands();

  void Run();

  ControllerState controller_state;

  static ESBConnection *get() {
    static ESBConnection handler;
    return &handler;
  }

private:
  bool mConnected = false;
  float mPos = 0.0f;
  float mPosOffset = 0.0f;
  float mLastOPos = 0.0f;

  FastMovingAverage<float> mRXAvg{40};
  uint32_t mLastRX = 0;
  FastMovingAverage<float> mTXAvg{40};
  uint32_t mLastTX = 0;
  uint64_t mLastPosUpdate = 0;

};

#endif