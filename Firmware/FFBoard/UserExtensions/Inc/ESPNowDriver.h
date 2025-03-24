/*
 * ESPNowDriver.h
 *
 *  Created on: March 22, 2025
 *      Author: Jatinder
 */

#ifndef USEREXTENSIONS_SRC_ESPNOW_DRIVER_H_
#define USEREXTENSIONS_SRC_ESPNOW_DRIVER_H_
#include "CommandHandler.h"
#include "ESPNowDriverProtocol.h"
#include "Encoder.h"
#include "FastAvg.h"
#include "MotorDriver.h"
#include "PersistentStorage.h"
#include "thread.hpp"

#ifdef ESPNOW
#define ESPNOW_THREAD_MEM 512
#define ESPNOW_THREAD_PRIO 25 // Must be higher than main thread

enum class ESPNowDriver_commands : uint32_t {
  pos_update_rate,
  torque_update_rate,
};

class ESPNowDriver : public MotorDriver,
                     public PersistentStorage,
                     public Encoder,
                     public CommandHandler,
                     cpp_freertos::Thread {
public:
  ESPNowDriver();
  virtual ~ESPNowDriver();

  // static ClassIdentifier info;
  const ClassIdentifier getInfo() = 0;
  // static bool isCreatable();

  void turn(int16_t power) override;
  void stopMotor() override;
  void startMotor() override;
  Encoder *getEncoder() override;
  bool hasIntegratedEncoder() { return true; }

  bool motorReady() override;
  void Run();

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
  std::string getHelpstring() {
    return "Wireless motor driver through ESPNow protocol";
  };

private:
  float mPos = 0.0f;
  float mPosOffset = 0.0f;
  float mLastOPos = 0.0f;

  FastMovingAverage<float> mPosUpdateAvg{40};
  uint64_t mLastPosUpdate = 0;
  FastMovingAverage<float> mTorqueUpdateAvg{40};
  uint64_t mLastTorqueUpdate = 0;
};

/**
 * Instance 1 of ESPNowDriver
 * Use for M0 output
 */
class ESPNowDriver_1 : public ESPNowDriver {
public:
  ESPNowDriver_1() : ESPNowDriver() { inUse = true; }
  const ClassIdentifier getInfo();
  ~ESPNowDriver_1() { inUse = false; }
  static bool isCreatable();
  static ClassIdentifier info;
  static bool inUse;
};

#endif /* USEREXTENSIONS_SRC_ESPNOW_DRIVER_H_ */
#endif
