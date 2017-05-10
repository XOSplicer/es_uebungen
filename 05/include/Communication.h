#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstdint>

#include "debug.h"


class ClientServer {
 public:
    enum Mode : unsigned int {
      Server,
      Client
    };

  public:
    struct Packet {
      uint16_t payloadLength;
      uint16_t sequenceNumber;
      uint16_t command;
      uint16_t handle;
      uint8_t data[0];
    } __attribute__((packed));

    enum class Command : uint16_t {
      Invalid           = 0,
      GeneralErrorReply = 1,
      Echo              = 2,
      EchoReply         = 3,
      Shutdown          = 4,
      ShutdownReply     = 5,
      Unsupported       = 6
    };

  public:
    ClientServer() {}
    bool Start(Mode mode, unsigned int port, const char * ip);

  private:
    bool StartServer(unsigned int port, const char * ip);
    bool StartClient(unsigned int port, const char * ip);
    bool createPacket(void* buffer, u_int16_t payloadLength,
                      uint16_t sequenceNumber, Command command,
                      uint16_t handle, void* data_buf);

};

#endif /* COMMUNICATION_H */
