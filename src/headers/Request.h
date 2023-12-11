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
    Message message;
    Command method;

public:
    Request(Command, int, Message);

    Message getMessage();
    int getSocketId();
    void senResponse(int, std::string);
};

#endif