#ifndef SENDREQUEST_H
#define SENDREQUEST_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include "IRequest.h"

class SendRequest : IRequest
{
    public:
        std::string sender;
        std::string receiver;
        std::string subject;
        std::string message;

        SendRequest(std::string sender, std::string receiver, std::string subject)
        {
            this->sender = sender;
            this->receiver = receiver;
            this->subject = subject;
        }

};


#endif