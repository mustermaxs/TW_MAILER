#ifndef ISOCKETHANDLER_H
#define ISOCKETHANDLER_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF 4096
#define IPADDRESS "127.0.0.1"

class ISocketHandler
{
protected:
    int socketId;
    std::string ip;
    struct sockaddr_in address;
    int port;

public:
    virtual ~ISocketHandler(){};
    virtual bool makeConnection() = 0;
    virtual bool sendData(const std::string data) = 0;
    virtual std::string receiveData() = 0;
    virtual void closeConnection() = 0;
    virtual bool createSocket() = 0;
    virtual int getSocketId() = 0;

    // virtual int getSocketId() { return this->socketId; };
};

#endif