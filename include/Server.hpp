#ifndef SERVER_H
#define SERVER_H

#include <atomic>

class Server {
public:
  Server(int t_port);
  void run();
  void stop();

private:
  int m_port;
  int m_server_sock;
  std::atomic<bool> m_running;
};

#endif
