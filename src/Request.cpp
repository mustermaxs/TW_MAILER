#include "./headers/Request.h"


Request::Request(Command method, int *socketId, Message* message)
{
    this->method = method;
    this->socketId = socketId;
    this->message = message;
};

Message* Request::getMessage()
{
    return this->message;
};

int* Request::getSocketId()
{
    return this->socketId;
};