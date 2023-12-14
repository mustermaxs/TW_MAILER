#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include "ISocketHandler.h"

class SocketServer : public ISocketHandler
{
private:
public:
    SocketServer();
    ~SocketServer();
    bool makeConnection();
    bool sendData(const std::string data);
    std::string receiveData();
    void closeConnection();
    bool createSocket();
    int getSocketId();
};

#endif