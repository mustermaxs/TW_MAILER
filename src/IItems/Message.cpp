#include "Message.h"

Message::Message(std::string receiver, std::string subject, std::string message)
{
    this->receiver = receiver;
    this->subject = subject;
    this->message = message;
};

Message::Message()
{
    throw new std::invalid_argument("No arguments provided to constructor of class Message.");
}

 std::string Message::GetReceiver() const { return this->receiver; };
 std::string Message::GetSubject() const { return this->subject; };
 std::string Message::GetMessage() const { return this->message; };

std::string Message::ToString() const
{
    std::string res =
    this->subject + "\n" +
    this->message + "\n";

    return res;
};