#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

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

class SocketClient
{
private:
    int socketId;
    std::string ip;
    struct sockaddr_in address;
    int port;

public:
    SocketClient(std::string, int);
    bool connectToServer();
    bool sendData(const std::string data);
    std::string receiveData();
    void closeConnection();
    bool createSocket();
    int getSocketId() { return this->socketId; };
};

#endif