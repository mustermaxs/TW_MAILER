#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include "ISocketHandler.h"
#include "IFileHandler.h"

class SocketClient : public ISocketHandler
{
protected:
    int socketId;
    std::string ip;
    struct sockaddr_in address;
    int port;

public:
    bool sendData(const std::string, int) override { return true; };
    bool sendData(const std::string);
    SocketClient(std::string, int);
    bool makeConnection() override;
    bool createSocket() override;
    int getSocketId() override;
};

#endif