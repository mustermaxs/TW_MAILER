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
        std::string message;
    public:
        Message() {};
        Message(std::string receiver, std::string subject, std::string message);
         std::string GetReceiver() const;
         std::string GetSubject() const;
         std::string GetMessage() const;
         std::string ToString() const override;
};

#endif