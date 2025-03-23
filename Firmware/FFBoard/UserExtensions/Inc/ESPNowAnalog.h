/*
 * ESPNowAnalog.h
 *
 *  Created on: 03.23.2025
 *      Author: Jatinder
 */

#ifndef USEREXTENSIONS_SRC_ESPNOWANALOG_H_
#define USEREXTENSIONS_SRC_ESPNOWANALOG_H_

#include "constants.h"
#ifdef ESPNOW

#include "AnalogSource.h"
#include "CommandHandler.h"
#include "ESPNowHandler.h"
#include "PersistentStorage.h"
#include "thread.hpp"

#define ESPNOW_THREAD_MEM 512
#define ESPNOW_THREAD_PRIO 25 // Must be higher than main thread

class ESPNowAnalog : public AnalogSource,
                      public CommandHandler,
                      cpp_freertos::Thread {
public:
//   enum class ESPNowAnalog_commands : uint32_t {  };

  ESPNowAnalog();
  virtual ~ESPNowAnalog();

  const ClassIdentifier getInfo();
  static ClassIdentifier info;

  void restoreFlash();
  void saveFlash();

  void Run();

  CommandStatus command(const ParsedCommand &cmd,
                        std::vector<CommandReply> &replies);
private:
  const uint8_t axes = 2;
};

#endif
#endif /* USEREXTENSIONS_SRC_ESPNOWANALOG_H_ */
