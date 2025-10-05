#include "Server.hpp"
#include <string>

int main(const int argc, const char *argv[]) {
  int port = 6379;

  if (argc >= 2)
    port = std::stoi(argv[1]);

  Server server = Server(port);

  // persistance
  // std::thread persistance_thread([](){
  //   while (true) {
  //     std::this_thread::sleep_for(std::chrono::seconds(300));
  //
  //   }
  // })

  server.run();

  return 0;
}
