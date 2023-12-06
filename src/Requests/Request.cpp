
#include <iostream>
#include <stdlib.h>
#include "Request.h"
#include "../IItems/Message.h"

Request::Request(METHOD method, int socketID, Message message)
{
    this->method = method;
    this->socketID = socketID;
    this->message = message;
};

Message Request::getMessage()
{
    return this->message;
}

