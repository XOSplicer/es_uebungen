#include "Communication.h"
#include "ClientServerReal.h"

bool ClientServer::Start(Mode mode, unsigned int port, const char * ip) {
  if (Mode::Client == mode) {
    ClientServerReal csr;
    return csr.Start(ClientServerReal::Mode::Client, port, ip);
  } else if (Mode::Server == mode) {
    ClientServerReal csr;
    return csr.Start(ClientServerReal::Mode::Server, port, ip);
  } else {
    return false;
  }
}
