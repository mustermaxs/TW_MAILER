#include "./headers/Message.h"


Message::Message()
{
    this->sender = "";
    this->receiver = "";
    this->subject = "";
    this->content = "";
    this->messageNumber = 0;
}

std::string Message::getSender() const { return this->sender; };
std::string Message::getReceiver() const { return this->receiver; };
std::string Message::getSubject() const { return this->subject; };
std::string Message::getContent() const { return this->content; };
int Message::getMessageNumber() const {return this->messageNumber; };

Message* Message::setSender(std::string sender) { this->sender = sender; return this; };
Message* Message::setReceiver(std::string receiver) { this->receiver = receiver; return this; };
Message* Message::setSubject(std::string subject) { this->subject = subject; return this; };
Message* Message::setContent(std::string content) { this->content = content; return this; };
Message* Message::setMessageNumber(int messageNumber) { this->messageNumber = messageNumber; return this; };

std::string Message::toString() const
{
    std::string res =
    this->subject + "\n" +
    this->content + "\n";

    return res;
};