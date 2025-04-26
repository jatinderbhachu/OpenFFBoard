/*
 * ESBAnalog.h
 *
 *  Created on: 03.23.2025
 *      Author: Jatinder
 */

#ifndef USEREXTENSIONS_SRC_ESB_ANALOG_H_
#define USEREXTENSIONS_SRC_ESB_ANALOG_H_

#include "constants.h"
#ifdef ESB_CONNECTION

#include "AnalogSource.h"
#include "CommandHandler.h"
#include "PersistentStorage.h"
#include "thread.hpp"

#define ESB_ANALOG_THREAD_MEM 512
#define ESB_ANALOG_THREAD_PRIO 25 // Must be higher than main thread

class ESBAnalog : public AnalogSource,
                  public CommandHandler,
                  cpp_freertos::Thread {
public:
ESBAnalog();
  virtual ~ESBAnalog();

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
#endif /* USEREXTENSIONS_SRC_ESB_ANALOG_H_ */