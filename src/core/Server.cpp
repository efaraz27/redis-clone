#include "Server.hpp"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

static Server *g_server = nullptr;

Server::Server(int t_port)
    : m_port(t_port), m_server_sock(-1), m_running(true) {

  g_server = this;
}

void Server::stop() {
  m_running = false;

  if (m_server_sock != -1) {
    close(m_server_sock);
  }

  std::cout << "Server shutdown gracefully.\n";
}

void Server::run() {
  m_server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (m_server_sock < 0) {
    std::cerr << "Error creating a socket.\n";
    return;
  }

  int opt = 1;
  setsockopt(m_server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(m_port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(m_server_sock, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    std::cerr << "Error binding socket.\n";
    return;
  }

  if (listen(m_server_sock, 10) < 0) {
    std::cerr << "Error listening on port: " << m_port << ".\n";
    return;
  }

  std::cout << "Server listening on port: " << m_port << ".\n";
}
