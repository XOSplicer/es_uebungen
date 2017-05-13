#include "Communication.h"

bool ClientServer::Start(Mode mode, unsigned int port, const char * ip) {
  if (Mode::Client == mode
      && !ip) {
    return false;
  } /* ip may be null for server when binding to all interfaces */
  m_mode = mode;
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

  DEBUG("Server running");
  while (running) {
    memset(recv_buf, 0, MAX_PACKET_LENGTH);
    memset(send_buf, 0, MAX_PACKET_LENGTH);
    /* blocking */
    if (!RecvPacket(recv_buf)) {
      DEBUG("Failed to recieve packet");
      continue;
    }
    DEBUG("Recieved:");
    DebugPacket(recv_buf);
    if (!ServerCreateResponse(send_buf, recv_buf)) {
      DEBUG("Failed to create response");
      continue;
    }
    DEBUG("Sending:");
    DebugPacket(send_buf);
    if (!SendPacket(send_buf)) {
      DEBUG("Failed to send response");
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

  bool running = true;

  DEBUG("Client running");

  //TODO
  /* arbitrary test commands */

  memset(send_buf, 0, MAX_PACKET_LENGTH);
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  char data[] = "TEST";
  CreatePacket(send_buf, sizeof(data), NextSequenceNumber(), Command::Echo, 11, data);
  DEBUG("Sending:");
  DebugPacket(send_buf);
  SendPacket(send_buf);
  RecvPacket(recv_buf);
  DEBUG("Recieved:");
  DebugPacket(recv_buf);

  memset(send_buf, 0, MAX_PACKET_LENGTH);
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  CreatePacket(send_buf, 0, NextSequenceNumber(), Command::Invalid, 22, nullptr);
  DEBUG("Sending:");
  DebugPacket(send_buf);
  SendPacket(send_buf);
  RecvPacket(recv_buf);
  DEBUG("Recieved:");
  DebugPacket(recv_buf);

  memset(send_buf, 0, MAX_PACKET_LENGTH);
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  CreatePacket(send_buf, 0, NextSequenceNumber(), Command::Unsupported, 33, nullptr);
  DEBUG("Sending:");
  DebugPacket(send_buf);
  SendPacket(send_buf);
  RecvPacket(recv_buf);
  DEBUG("Recieved:");
  DebugPacket(recv_buf);

  /* ending with server shutdown */
  uint16_t handle = 66;
  while (running) {
    memset(send_buf, 0, MAX_PACKET_LENGTH);
    memset(recv_buf, 0, MAX_PACKET_LENGTH);
    CreatePacket(send_buf, 0, NextSequenceNumber(), Command::Shutdown, handle, nullptr);
    DEBUG("Sending:");
    DebugPacket(send_buf);
    SendPacket(send_buf);
    RecvPacket(recv_buf);
    DEBUG("Recieved:");
    DebugPacket(recv_buf);
    if (send_buf->handle == recv_buf->handle
        && static_cast<uint16_t>(Command::ShutdownReply) == recv_buf->command) {
      DEBUG("Recieved ShutdownReply");
      running = false;
    } else {
      DEBUG("Did not recieve ShutdownReply");
    }
    handle++;
  }


  if(!ShutdownClient()) {
    DEBUG("Failed to shut down client");
    return false;
  }

  m_packet_buffer.Deallocate(recv_buf);
  m_packet_buffer.Deallocate(send_buf);

  return true;
}

bool ClientServer::InitServer(unsigned int port, const char * ip) {
  /* ip may be null for INADDR_ANY */
  DEBUG("Initializing server");
  if(!port) {
    return false;
  }
  /* create socket */
  m_socket = socket (AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (m_socket < 0) {
    DEBUG("Failed to open socket");
    return false;
  }
  /* Bind server to given port and ip or any ip if nullprt */
  /* we dont need other_addr here */
  /* step 1 find out the server address we are on */
  memset((char*) &m_my_addr, 0, sizeof(m_my_addr));
  m_my_addr.sin_family = AF_INET;
  m_my_addr.sin_port = htons(port);
  if (ip) {
    hostent *hp; /* host information */
    hp = gethostbyname(ip);
    if (hp) {
      memcpy((void*) &(m_my_addr.sin_addr), hp->h_addr_list[0], hp->h_length);
      /* just pick the first one here, TODO validate that its ipv4 */
    } else {
      DEBUG("Could not get host for " << ip << "fallback to INADDR_ANY");
      m_my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
  } else {
    m_my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  }

  /* step 2 do the bind to server address */
  if (bind(m_socket, (sockaddr*) &m_my_addr, sizeof(m_my_addr)) < 0) {
    DEBUG("Could not bind socket to address");
    return false;
  }

  /* all went well */
  return true;
}

bool ClientServer::InitClient(unsigned int port, const char * ip) {
  DEBUG("Initializing client");
  if (!ip || !port) {
    return false;
  }
  /* create socket */
  m_socket = socket (AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (m_socket < 0) {
    DEBUG("Failed to open socket");
    return false;
  }

  /* find our address to be any, this is where we bind the socket to */
  memset((char*) &m_my_addr, 0, sizeof(m_my_addr));
  m_my_addr.sin_family = AF_INET;
  m_my_addr.sin_port = htons(0); /* any port */
  m_my_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* any address */

  /* find out server address, this is where we send messages */
  memset((char*) &m_other_addr, 0, sizeof(m_other_addr));
  m_other_addr.sin_family = AF_INET;
  m_other_addr.sin_port = htons(port);
  hostent *hp; /* host information */
  hp = gethostbyname(ip);
  if (hp) {
    memcpy((void*) &(m_other_addr.sin_addr), hp->h_addr_list[0], hp->h_length);
    /* just pick the first one here, TODO validate that its ipv4 */
  } else {
    DEBUG("Could not get host for " << ip);
    return false;
  }

  /* bind to client addr */
  if (bind(m_socket, (sockaddr*) &m_my_addr, sizeof(m_my_addr)) < 0) {
    DEBUG("Could not bind socket to address");
    return false;
  }

  /* all went fine */
  return true;
}

bool ClientServer::ShutdownServer() {
  DEBUG("Shutting down server");
  close(m_socket);
  return true;
}

bool ClientServer::ShutdownClient() {
  DEBUG("Shutting down client");
  close(m_socket);
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
  //TODO implement safety mechanism here
  HostToNetPacket(packet);
  auto size = sizeof(ClientServer::Packet) + packet->payloadLength;
  if(sendto(m_socket, packet, size, 0, (sockaddr*) &m_other_addr, sizeof(m_other_addr)) < 0) {
    DEBUG("sendto failed");
    return false;
  }
  NetToHostPacket(packet); /* also revert, so we can keep on using it */
  /* all went well */
  return true;
}

/* blocking */
bool ClientServer::RecvPacket(Packet* buffer) {
  if (!buffer) {
    return false;
  }

  sockaddr_in recv_addr;
  socklen_t recv_addr_len = sizeof(recv_addr);
  int recv_len;

  //TODO implement safety mechanism here
  if (Mode::Server == m_mode) {
    recv_len = recvfrom(m_socket, buffer, MAX_PACKET_LENGTH, 0,
                        (sockaddr*) &recv_addr, &recv_addr_len);
    //TODO maybe check for payload len aswell
    if (recv_len < static_cast<int>(sizeof(ClientServer::Packet))) {
      return false;
    }

    /* copy recieved addr to other_addr here */
    memcpy(&m_other_addr, &recv_addr, sizeof(recv_addr));

    NetToHostPacket(buffer);

    /* all went well */
    return true; /* end server */
  } else if (Mode::Client == m_mode) {
    recv_len = recvfrom(m_socket, buffer, MAX_PACKET_LENGTH, 0,
                        (sockaddr*) &recv_addr, &recv_addr_len);
    //TODO maybe check for payload len aswell
    if (recv_len < static_cast<int>(sizeof(ClientServer::Packet))) {
      return false;
    }

    /* do not copy recieved addr to other_addr here */
    //TODO chech if other addr == recieved addr to increase security

    NetToHostPacket(buffer);

    /* all went well */
    return true; /* end client */
  } else {
    return false;
  }
}

/* will not convert payload */
void ClientServer::HostToNetPacket(Packet* packet) {
  if (!packet) {
    return;
  }
  /* htons converts host to net uint16_t */
  packet->payloadLength   = htons(packet->payloadLength);
  packet->sequenceNumber  = htons(packet->sequenceNumber);
  packet->command         = htons(packet->command);
  packet->handle          = htons(packet->handle);
}

/* will not convert payload */
void ClientServer::NetToHostPacket(Packet* packet) {
  if (!packet) {
    return;
  }
  /* htons converts net to host uint16_t */
  packet->payloadLength   = ntohs(packet->payloadLength);
  packet->sequenceNumber  = ntohs(packet->sequenceNumber);
  packet->command         = ntohs(packet->command);
  packet->handle          = ntohs(packet->handle);
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
