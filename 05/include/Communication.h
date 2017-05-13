#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstdint>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "PoolAllocator.h"
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

    enum Command : uint16_t {
      Invalid           = 0,
      GeneralErrorReply = 1,
      Echo              = 2,
      EchoReply         = 3,
      Shutdown          = 4,
      ShutdownReply     = 5,
      Unsupported       = 6
    };

  public:
    /* usefull for UDP datagramm */
    static constexpr uint16_t MAX_PACKET_LENGTH   = 1500;
    static constexpr uint16_t MAX_PAYLOAD_LENGTH  = MAX_PACKET_LENGTH - sizeof(Packet);

  public:
    ClientServer()
    : m_packet_buffer(),
      m_sequence_number(0),
      m_socket(-1),
      m_server_addr(),
      m_client_addr() {}
    bool Start(Mode mode, unsigned int port, const char * ip);

  private:
    bool StartServer(unsigned int port, const char * ip);
    bool StartClient(unsigned int port, const char * ip);
    bool InitServer(unsigned int port, const char * ip);
    bool InitClient(unsigned int port, const char * ip);
    bool ShutdownServer();
    bool ShutdownClient();
    bool ServerCreateResponse(Packet* buffer, Packet* recieved);
    bool CreatePacket(Packet* buffer, u_int16_t payloadLength,
                      uint16_t sequenceNumber, Command command,
                      uint16_t handle, const void* data_buf);
    bool SendPacket(Packet* packet);
    bool RecvPacket(Packet* buffer);
    /** will not convert payload */
    void HostToNetPacket(Packet* packet);
    void NetToHostPacket(Packet* packet);
    uint16_t NextSequenceNumber();
    void DebugPacket(Packet* packet);

  private:
    PoolAllocator<10, MAX_PACKET_LENGTH> m_packet_buffer;
    uint16_t m_sequence_number;
    int m_socket;
    sockaddr_in m_server_addr, m_client_addr;
};

#endif /* COMMUNICATION_H */
