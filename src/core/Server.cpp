#include "Server.hpp"
#include "CommandHandler.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

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

  std::cout << "Server shutdown gracefully\n";
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
    std::cerr << "Error listening on port: " << m_port << "\n";
    return;
  }

  std::cout << "Server listening on port: " << m_port << "\n";

  std::vector<std::thread> threads;
  CommandHandler cmd_handler;

  while (m_running) {

    int client_socket = accept(m_server_sock, nullptr, nullptr);
    if (client_socket < 0) {
      if (m_running) {
        std::cerr << "Error accepting client connection.\n";
      }
      break;
    }

    threads.emplace_back([client_socket, &cmd_handler]() {
      char buffer[1024];
      while (true) {
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0)
          break;

        std::string request(buffer, bytes);
        std::string response = cmd_handler.process_cmd(request);

        send(client_socket, response.c_str(), response.size(), 0);
      }
      close(client_socket);
    });
  }

  for (std::thread &t : threads) {
    if (t.joinable())
      t.join();
  }
}
