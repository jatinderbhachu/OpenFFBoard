/*
 * ESPNowDriver.h
 *
 *  Created on: March 22, 2025
 *      Author: Jatinder
 */

#ifndef USEREXTENSIONS_SRC_ESPNOW_H_
#define USEREXTENSIONS_SRC_ESPNOW_H_
#include "CommandHandler.h"
#include "ESPNowDriverProtocol.h"
#include "Encoder.h"
#include "MotorDriver.h"
#include "PersistentStorage.h"
#include "thread.hpp"

#ifdef ESPNOW
#define ESPNOW_THREAD_MEM 512
#define ESPNOW_THREAD_PRIO 25 // Must be higher than main thread

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
  void add_client(const uint8_t *address);
  void remove_client(const std::array<uint8_t, ESP_NOW_ETH_ALEN> &address);

  void send_connect_ack();

  static void espnow_recv_cb(const esp_now_recv_info_t *recv_info,
                             const uint8_t *data, int len);
  static void espnow_task(void *param);

  int64_t mLastConnectAttempt = 0;
  bool mActive = false;
  bool mConnected = false;
  float mPos = 0.0f;
  float mPosOffset = 0.0f;
  float mLastOPos = 0.0f;

private:
  std::array<uint8_t, ESP_NOW_ETH_ALEN> mControllerAddress;
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

#endif /* USEREXTENSIONS_SRC_ESPNOW_H_ */
#endif
