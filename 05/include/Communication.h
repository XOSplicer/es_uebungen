#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstdint>
#include <cstring> /* for memcpy, memset */
#include <sys/types.h> /* for socket_addr */
#include <sys/socket.h> /* for socket */
#include <netinet/in.h> /* for socket type */
#include <arpa/inet.h> /* for socket addr */
#include <netdb.h> /* for gethostbyname */
#include <unistd.h> /* for close */

#include "PoolAllocator.h"
#include "debug.h"

/*

Implementation of transmission safety:
For each single packet

      Sender (while in sendpacket)                    Reciever (while in recvpacket)

1.    Send DATA Wrapper
        totalLength = sizeof(Wrapper)
                      + sizeof(Packet)
                      + packet->payloadLength
        sequenceNumber = generated (counted up > 1)
        ackNumber = 0
        packetChecksum = fletcher_16 of packet
        type = DATA
        packet = packet

2.                                                      Recieve Wrapper
                                                          check totalLength
                                                          check type
                                                          check checksum

3a.                                                     Send Ack if good
                                                          totalLength = sizeof(Wrapper)
                                                          sequenceNumber = generated
                                                          ackNumber = recieved seqNr
                                                          checksum = 0
                                                          type = ACK
                                                          packet = empty

3b.                                                     Send Nack if not good
                                                          totalLength = sizeof(Wrapper)
                                                          sequenceNumber = generated
                                                          ackNumber = recieved seqNr
                                                          checksum = 0
                                                          type = ACK
                                                          packet = empty

4a.    Recieve valid ACK
        finished

4b.    Recieve invalid ACK / NACK / timeout
        Resend the Wrapped package up to MAX_SEND_RETRIES times
        until finished or abort

validation is done via all fields except for sequenceNumber

 */

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

    struct Wrapper {
      uint16_t totalLength;
      uint16_t sequenceNumber;
      uint16_t ackNumber;
      uint16_t packetChecksum;
      uint8_t type;
      Packet packet[0];
    } __attribute__((packed));

    enum WrapperType : uint8_t {
      ACK   = 0,
      NACK  = 1,
      DATA  = 2
    };

  public:
    /* usefull for UDP datagramm */
    static constexpr uint16_t MAX_TRANSMISSION_LENGTH = 1500;
    static constexpr uint16_t MAX_PACKET_LENGTH =
      MAX_TRANSMISSION_LENGTH - sizeof(Wrapper);
    static constexpr uint16_t MAX_PAYLOAD_LENGTH =
      MAX_PACKET_LENGTH - sizeof(Packet);
    static constexpr size_t MAX_SEND_RETRIES = 10;

  public:
    ClientServer()
    : m_packet_buffer(),
      m_sequence_number(1),
      m_socket(-1),
      m_my_addr(),
      m_other_addr(),
      m_mode(Mode::Server),
      m_wrapper_number(1) {}
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
    bool SendPacket(const Packet* packet);
    /* blocking */
    bool RecvPacket(Packet* buffer);
    /* will not convert payload */
    void HostToNetPacket(Packet* packet);
    /* will not convert payload */
    void NetToHostPacket(Packet* packet);
    uint16_t NextSequenceNumber();
    void DebugPacket(const Packet* packet);
    uint16_t PacketChecksum(const Packet* packet);
    bool ValidateWrapper(const Wrapper* wrapper, ssize_t bytes_recieved,
                                        uint16_t ack_expected_number);
    bool SendAckWrapper(uint16_t recieved_seq, uint16_t seq_number);
    bool SendNackWrapper(uint16_t recieved_seq, uint16_t seq_number);
    bool SendDataWrapper(const Packet* packet, uint16_t seq_number);
    /* blocking and validating */
    bool RecvWrapper(Wrapper* buffer, uint16_t ack_expected_number);
    /* will not convert possible packet */
    void HostToNetWrapper(Wrapper* wrapper);
    /* will not convert possible packet */
    void NetToHostWrapper(Wrapper* wrapper);
    uint16_t NextWrapperNumber();
    void DebugWrapper(const Wrapper* wrapper);

  private:
    PoolAllocator<10, MAX_PACKET_LENGTH> m_packet_buffer;
    uint16_t m_sequence_number;
    int m_socket;
    sockaddr_in m_my_addr, m_other_addr;
    Mode m_mode;
    uint16_t m_wrapper_number;
};

/* fletcher 16 checksum */
uint16_t fletcher_16 (const uint8_t* data, const size_t length);

#endif /* COMMUNICATION_H */
