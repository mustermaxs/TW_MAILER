#include "Message.h"

Message::Message(std::string receiver, std::string subject, std::string content)
{
    this->receiver = receiver;
    this->subject = subject;
    this->content = content;
};

Message::Message()
{
    throw new std::invalid_argument("No arguments provided to constructor of class Message.");
}

 std::string Message::getReceiver() const { return this->receiver; };
 std::string Message::getSubject() const { return this->subject; };
 std::string Message::getContent() const { return this->content; };

std::string Message::toString() const
{
    std::string res =
    this->subject + "\n" +
    this->content + "\n";

    return res;
};