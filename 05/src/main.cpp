/**
 * @author Felix Stegmaier
 */

#include "Communication.h"
#include "debug.h"

int main(int argc, char const *argv[]) {
  if (2 != argc) {
    DEBUG("options: -s : server mode -c : client mode");
    DEBUG("sizeof(ClientServer::Packet)=" << sizeof(ClientServer::Packet));
    DEBUG("sizeof(ClientServer::Command)=" << sizeof(ClientServer::Command));
    DEBUG("MAX_PAYLOAD_LENGTH=" << ClientServer::MAX_PAYLOAD_LENGTH);
    uint16_t data[2] = {6, 7};
    DEBUG("sizeof(uint16_t data[2])=" << sizeof(data));
    DEBUG("Command::ShutdownReply=" << static_cast<uint16_t>(ClientServer::Command::ShutdownReply));
    return 1;
  }
  ClientServer client_server;
  if ('-' == argv[1][0]
      && 's' == argv[1][1]) {
    return client_server.Start(ClientServer::Mode::Server, 9000, "127.0.0.1")
      ? 0 : 2 ;
  } else if ('-' == argv[1][0]
      && 'c' == argv[1][1]) {
    return client_server.Start(ClientServer::Mode::Client, 9000, "127.0.0.1")
      ? 0 : 2;
  } else {
    DEBUG("options: -s : server mode -c : client mode");
    return 1;
  }
}
