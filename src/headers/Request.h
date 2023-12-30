#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <stdlib.h>

#include "Message.h"
#include "Commands.h"

class Request
{
private:
    int socketId;
    IMessage* message;
    Command method;
    std::string ip;

public:
    Request(Command, int, IMessage*, std::string);

    IMessage* getMessage();
    int getSocketId();
    void sendResponse(int, std::string);
    std::string getIp();
};

#endif