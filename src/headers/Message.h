#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <exception>
#include <map>
#include <vector>

#include "IItem.h"



class Message : IItem
{
    private:
        std::string sender;
        std::string receiver;
        std::string subject;
        std::string content;
        int messageNumber;

    public:
        Message();
        ~Message() {};
        std::string getSender() const;
        std::string getReceiver() const;
        std::string getSubject() const;
        std::string getContent() const;
        std::string toString() const override;
        int getMessageNumber() const;

        Message* setSender(std::string);
        Message* setReceiver(std::string);
        Message* setSubject(std::string);
        Message* setContent(std::string);
        Message* setMessageNumber(int);
        static Message* fromFileLines(const std::vector<std::string> msgLines);
        // static Message* fromString(const std::string msgString);
};

#endif