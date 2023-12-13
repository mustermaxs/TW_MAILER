#ifndef IMESSAGE_H
#define IMESSAGE_H

#include <string>
#include <exception>
#include <map>
#include <vector>

#include "IItem.h"

class IMessage
{
    private:

protected:
    std::string sender;
    std::string receiver;
    std::string subject;
    std::string content;
    int messageNumber;

public:
    virtual ~IMessage() {};
     std::string getSender() const;
     std::string getReceiver() const;
     std::string getSubject() const;
     std::string getContent() const;
    virtual std::string toString() = 0;

    int getMessageNumber() const;

     IMessage *setSender(std::string);
     IMessage *setReceiver(std::string);
     IMessage *setSubject(std::string);
     IMessage *setContent(std::string);
     IMessage *setMessageNumber(int);
    virtual IMessage *parse(const std::vector<std::string> msgLines) = 0;
};

#endif