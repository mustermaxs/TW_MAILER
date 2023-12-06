#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <exception>

#include "IItem.h"

class Message : IItem
{
    private:
        std::string receiver;
        std::string subject;
        std::string content;
    public:
        Message() {};
        Message(std::string receiver, std::string subject, std::string message);
         std::string getReceiver() const;
         std::string getSubject() const;
         std::string getContent() const;
         std::string toString() const override;
};

#endif