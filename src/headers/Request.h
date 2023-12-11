#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <stdlib.h>

#include "Message.h"


// valid tw mailer commands
// as required
enum COMMAND
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
    COMMAND method;
    int socketID;
    Message message;

    Request(COMMAND, int, Message);

    Message getMessage();
};

#endif