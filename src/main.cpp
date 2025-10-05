#include "Server.hpp"
#include <string>

int main(const int argc, const char *argv[]) {
  int port = 6379;

  if (argc >= 2)
    port = std::stoi(argv[1]);

  Server server = Server(port);
  return 0;
}
