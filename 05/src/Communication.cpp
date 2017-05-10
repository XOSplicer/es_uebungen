#include "Communication.h"

bool ClientServer::Start(ClientServer::Mode mode, unsigned int port, const char * ip) {
  if (!ip) {
    return false;
  }
  if (ClientServer::Mode::Server == mode) {
    return ClientServer::StartServer(port, ip);
  } else if (ClientServer::Mode::Client == mode) {
    return ClientServer::StartClient(port, ip);
  } else {
    return false;
  }
}

bool ClientServer::StartServer(unsigned int port, const char * ip) {
  DEBUG("Starting mode=Server, port=" << port << ", ip=" << ip);

  return false;
}

bool ClientServer::StartClient(unsigned int port, const char * ip) {
  DEBUG("Starting mode=Client, port=" << port << ", ip=" << ip);

  return false;
}
