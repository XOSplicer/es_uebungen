#include "Communication.h"

bool ClientServer::Start(Mode mode, unsigned int port, const char * ip) {
  if (Mode::Client == mode
      && !ip) {
    return false;
  } /* ip may be null for server when binding to all interfaces */
  if (Mode::Server == mode) {
    return StartServer(port, ip);
  } else if (Mode::Client == mode) {
    return StartClient(port, ip);
  } else {
    return false;
  }
}

bool ClientServer::StartServer(unsigned int port, const char * ip) {
  DEBUG("Starting mode=Server, port=" << port << ", ip=" << ip);

  if(!InitServer(port, ip)) {
    DEBUG("Failed to init server");
    return false;
  }

  bool running = true;
  Packet* recv_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));
  Packet* send_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));

  while (running) {
    memset(recv_buf, 0, MAX_PACKET_LENGTH);
    memset(send_buf, 0, MAX_PACKET_LENGTH);
    /* blocking */
    if (!RecvPacket(recv_buf)) {
      DEBUG("Failed to recieve packet");
      return false; //TODO not return
      continue;
    }
    DEBUG("Recieved:");
    DebugPacket(recv_buf);
    if (!ServerCreateResponse(send_buf, recv_buf)) {
      DEBUG("Failed to create response");
      return false; //TODO not return
      continue;
    }
    DEBUG("Sending:");
    DebugPacket(send_buf);
    if (!SendPacket(send_buf)) {
      DEBUG("Failed to send response");
      return false; //TODO not return
      continue;
    }
    if (Command::Shutdown == recv_buf->command) {
      DEBUG("Recieved Command::Shutdown, shutting down");
      if (!ShutdownServer()) {
        DEBUG("Failed to shut down server");
        return false; /* actually return here */
      }
      running = false;
    }
  }

  m_packet_buffer.Deallocate(recv_buf);
  m_packet_buffer.Deallocate(send_buf);

  return true;
}

bool ClientServer::StartClient(unsigned int port, const char * ip) {
  DEBUG("Starting mode=Client, port=" << port << ", ip=" << ip);

  if(!InitClient(port, ip)) {
    DEBUG("Failed to init client");
    return false;
  }

  Packet* recv_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));
  Packet* send_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  memset(send_buf, 0, MAX_PACKET_LENGTH);
  //TODO
  /* arbitrary test commands */

  /* ending with server shutdown */

  if(!ShutdownClient()) {
    DEBUG("Failed to shut down client");
    return false;
  }

  m_packet_buffer.Deallocate(recv_buf);
  m_packet_buffer.Deallocate(send_buf);

  return true;
}

bool ClientServer::InitServer(unsigned int /*port*/, const char * /*ip*/) {
  /* create socket */
  m_socket = socket (AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (m_socket < 0) {
    DEBUG("Failed to open socket");
    return false;
  }
  /* Bind server to given port and ip */

  //TODO
  return true;
}

bool ClientServer::InitClient(unsigned int /*port*/, const char * /*ip*/) {
  //TODO
  return true;
}

bool ClientServer::ShutdownServer() {
  //TODO
  return true;
}

bool ClientServer::ShutdownClient() {
  //TODO
  return true;
}

bool ClientServer::ServerCreateResponse(Packet* buffer, Packet* recieved) {
  if(!buffer
      || !recieved) {
    return false;
  }
  switch (static_cast<Command>(recieved->command)) {
    case Command::Echo : {
      return CreatePacket(buffer, recieved->payloadLength,
                          NextSequenceNumber(), Command::EchoReply,
                          recieved->handle, recieved->data);
    } break;
    case Command::Shutdown : {
      return CreatePacket(buffer, 0, NextSequenceNumber(),
                          Command::ShutdownReply, recieved->handle, nullptr);
    } break;
    default: {
      uint16_t data[1] = {recieved->command};
      return CreatePacket(buffer, sizeof(data), NextSequenceNumber(),
                          Command::GeneralErrorReply, recieved->handle,
                          data);
    } break;
  }
}

bool ClientServer::CreatePacket(Packet* buffer, u_int16_t payloadLength,
                  uint16_t sequenceNumber, Command command,
                  uint16_t handle, const void* data_buf) {
  if (!buffer
      || payloadLength > MAX_PAYLOAD_LENGTH) {
    return false;
  }
  buffer->payloadLength = payloadLength;
  buffer->sequenceNumber = sequenceNumber;
  buffer->command = static_cast<uint16_t>(command);
  buffer->handle = handle;
  if(data_buf) {
    std::memcpy(buffer->data, data_buf, payloadLength);
  }
  return true;
}

bool ClientServer::SendPacket(Packet* packet) {
  if (!packet) {
    return false;
  }
  //TODO
  return true;
}

bool ClientServer::RecvPacket(Packet* buffer) {
  if (!buffer) {
    return false;
  }
  //TODO
  /* test dummy */
  char data[16] = "test";
  return CreatePacket(buffer, sizeof(data), NextSequenceNumber(), Command::Echo, 42, data);

  return true;
}

uint16_t ClientServer::NextSequenceNumber() {
  return ++m_sequence_number;
}

void ClientServer::DebugPacket(Packet* packet) {
  if (!packet) {
    return;
  }
  DEBUG("Packet[payloadLength=" << packet->payloadLength
          << ", sequenceNumber=" << packet->sequenceNumber
          << ", command=" << packet->command
          << ", handle=" << packet->handle
          << ", data=" << static_cast<const unsigned char*>(packet->data));
}
