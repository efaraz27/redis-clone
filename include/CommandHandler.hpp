#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>

class CommandHandler {
public:
  CommandHandler();
  std::string process_cmd(const std::string &t_cmd);
};

#endif
