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

public:
    Request(Command, int, IMessage*);

    IMessage* getMessage();
    int getSocketId();
    void senResponse(int, std::string);
};

#endif