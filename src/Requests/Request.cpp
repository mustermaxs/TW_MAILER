
#include <iostream>
#include <stdlib.h>
#include "Request.h"

Request::Request(METHOD method, int socketID, Message message)
{
    this->method = method;
    this->socketID = socketID;
    this->message = message;
};


