#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class ClientServer {
 public:
    enum Mode : unsigned int {
      Server,
      Client
    };

    bool Start(Mode mode, unsigned int port, const char * ip);
};

#endif /* COMMUNICATION_H */
