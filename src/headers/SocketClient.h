#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include "ISocketHandler.h"

class SocketClient : public ISocketHandler
{
protected:
    int socketId;
    std::string ip;
    struct sockaddr_in address;
    int port;

public:
    SocketClient(std::string, int);
    bool makeConnection() override;
    void closeConnection() override;
    bool sendData(const std::string) override;
    bool createSocket() override;
    std::string receiveData() override;
    int getSocketId() override;
};

#endif