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
#include "FastAvg.h"
#include "MotorDriver.h"
#include "PersistentStorage.h"
#include "thread.hpp"

#ifdef ESPNOW
#define ESPNOW_THREAD_MEM 512
#define ESPNOW_THREAD_PRIO 25 // Must be higher than main thread

enum class ESPNowHandler_commands : uint32_t {
  list_available_devices,
};

class ESPNowHandler : public PersistentStorage,
                      public CommandHandler,
                      cpp_freertos::Thread {
public:
  ESPNowHandler();
  ~ESPNowHandler();

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

  static void register_recv_queue(QueueHandle_t queue);
  // static void unregister_recv_queue(QueueHandle_t queue);

  static ESPNowHandler *get() {
    static ESPNowHandler handler;
    return &handler;
  }

private:
  void add_client(const uint8_t *address);
  void remove_client(const std::array<uint8_t, ESP_NOW_ETH_ALEN> &address);
  void send_connect_ack();

  static void espnow_recv_cb(const esp_now_recv_info_t *recv_info,
                             const uint8_t *data, int len);
  static void espnow_task(void *param);

private:
  inline static std::vector<QueueHandle_t> sRegisteredQueues = {};
  bool mConnected = false;
  std::array<uint8_t, ESP_NOW_ETH_ALEN> mControllerAddress;
};

#endif /* USEREXTENSIONS_SRC_ESPNOW_H_ */
#endif
