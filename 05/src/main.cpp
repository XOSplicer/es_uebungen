/**
 * @author Felix Stegmaier
 */

#include <unistd.h> /* for write*/

#include "Communication.h"
#include "debug.h"

#define PORT 55047 //private, random constant

void print_usage();

int main(int argc, char const *argv[]) {
  if (2 > argc) {
    print_usage();
    DEBUG("sizeof(ClientServer::Packet)=" << sizeof(ClientServer::Packet));
    DEBUG("sizeof(ClientServer::Command)=" << sizeof(ClientServer::Command));
    DEBUG("MAX_PAYLOAD_LENGTH=" << ClientServer::MAX_PAYLOAD_LENGTH);
    DEBUG("Command::ShutdownReply=" << static_cast<uint16_t>(ClientServer::Command::ShutdownReply));
    return 1;
  }
  ClientServer client_server;
  if ('-' == argv[1][0]
      && 's' == argv[1][1]) {
        //bind to all interfaces on port PORT
    return client_server.Start(ClientServer::Mode::Server, PORT, nullptr)
      ? 0 : 2 ;
  } else if ('-' == argv[1][0]
      && 'c' == argv[1][1]
      && argc == 3 ) {
    return client_server.Start(ClientServer::Mode::Client, PORT, argv[2])
      ? 0 : 2;
  } else {
    print_usage();
    return 1;
  }
}


void print_usage() {
  char print_buf[256] = {0};
  Printf(print_buf, print_buf + sizeof(print_buf), "Options: \n\t-s\t\tserver mode\n\t-c <server IP>\tclient mode\nServer uses UDP port %d\n", PORT);
  write(STDOUT, print_buf, strlen(print_buf));
}
