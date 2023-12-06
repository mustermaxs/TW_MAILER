#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <stdlib.h>
#include "../IItems/Message.h"

class Request
{
enum METHOD {SEND, LIST, READ, DEL, QUIT};

    public:
        METHOD method;
        int socketID;
        Message message;

        Request(METHOD, int, Message);

    Message getMessage();

};


#endif