#include "./headers/Request.h"
#include "./headers/Message.h"



Request::Request(METHOD method, int socketID, Message message)
{
    this->method = method;
    this->socketID = socketID;
    this->message = message;
};

Message Request::getMessage()
{
    return this->message;
};