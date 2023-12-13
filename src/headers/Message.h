#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <exception>
#include <map>
#include <vector>

#include "IMessage.h"



class Message : public IMessage
{
    public:
        Message();
        ~Message() override {} ;
        std::string toString() override;
        static Message* fromLines(const std::vector<std::string> msgLines);
        Message* parse(const std::vector<std::string> msgLines) override;
};

#endif