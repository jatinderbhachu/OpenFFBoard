/*
 * ESPNowButtons.h
 *
 *  Created on: 03.23.2025
 *      Author: Jatinder
 */

#ifndef USEREXTENSIONS_SRC_ESPNOWBUTTONS_H_
#define USEREXTENSIONS_SRC_ESPNOWBUTTONS_H_

#include "constants.h"
#ifdef ESPNOW

#include "ButtonSource.h"
#include "CommandHandler.h"
#include "PersistentStorage.h"
#include "cpp_target_config.h"
#include "ESPNowHandler.h"
#include "thread.hpp"

#define ESPNOW_THREAD_MEM 512
#define ESPNOW_THREAD_PRIO 25 // Must be higher than main thread

class ESPNowButtons : public ButtonSource,
                      public CommandHandler,
                      cpp_freertos::Thread {
public:
  enum class ESPNowButton_commands : uint32_t { invert };

  ESPNowButtons();
  virtual ~ESPNowButtons();

  const ClassIdentifier getInfo();
  static ClassIdentifier info;

  void restoreFlash();
  void saveFlash();
  void Run();

  uint8_t readButtons(uint64_t *buf);

  CommandStatus command(const ParsedCommand &cmd,
                        std::vector<CommandReply> &replies);

private:
  bool invert = false;
  volatile uint64_t currentButtons = 0;
};

#endif
#endif /* USEREXTENSIONS_SRC_ESPNOWBUTTONS_H_ */
