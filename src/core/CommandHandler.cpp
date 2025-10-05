#include "CommandHandler.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> parse_ws_command(const std::string &input) {
  std::vector<std::string> tokens;
  std::istringstream iss(input);
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

std::vector<std::string> parse_resp_cmd(const std::string &input) {
  std::vector<std::string> tokens;

  if (input.empty())
    return tokens;

  if (input[0] != '*') {
    return parse_ws_command(input);
  }

  size_t pos = 0;

  if (input[pos] != '*')
    return tokens;
  pos++;

  size_t crlf = input.find("\r\n", pos);

  if (crlf == std::string::npos)
    return tokens;

  int num_elements = std::stoi(input.substr(pos, crlf - pos));
  pos = crlf + 2;

  for (int i = 0; i < num_elements; i++) {
    if (pos >= input.size() || input[pos] != '$')
      break;
    pos++;

    crlf = input.find("\r\n", pos);

    if (crlf == std::string::npos)
      break;

    int token_len = std::stoi(input.substr(pos, crlf - pos));

    pos = crlf + 2;

    if (pos >= input.size())
      break;

    std::string token = input.substr(pos, token_len);
    tokens.push_back(token);

    pos += token_len + 2;
  }

  return tokens;
}

CommandHandler::CommandHandler() {}

std::string CommandHandler::process_cmd(const std::string &t_cmd) {
  auto tokens = parse_resp_cmd(t_cmd);

  if (tokens.empty()) {

    return "-Error: Invalid command\r\n";
  }

  std::string cmd = tokens[0];
  std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

  std::ostringstream res;

  return res.str();
}
