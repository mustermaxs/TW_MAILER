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
#define IPADDRESS "asdasd"

class ISocketHandler
{
protected:
    int socketId = -1;
    std::string ip = "";
    struct sockaddr_in address;
    int port = -1;

public:
    virtual ~ISocketHandler();
    virtual bool makeConnection() = 0;
    virtual bool sendData(const std::string, int);
    std::string receiveData(int);
    virtual void closeConnection(int);
    virtual bool createSocket() = 0;
    virtual int getSocketId();
};

#endif