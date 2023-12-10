#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <stdlib.h>
#include "Message.h"

enum METHOD
{
    SEND,
    LIST,
    READ,
    DEL,
    QUIT
};

class Request
{

public:
    METHOD method;
    int socketID;
    Message message;

    Request(METHOD, int, Message);

    Message getMessage();
};

#endif