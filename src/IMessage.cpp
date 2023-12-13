#include "./headers/IMessage.h"


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// IMessage::IMessage()
// {
//     this->sender = "";
//     this->receiver = "";
//     this->subject = "";
//     this->content = "";
//     this->messageNumber = 0;
// }

//////////////////////////////////////////////////////////////////////
// multiple getters/setters                                         //
//////////////////////////////////////////////////////////////////////
std::string IMessage::getSender() const { return this->sender; };
std::string IMessage::getReceiver() const { return this->receiver; };
std::string IMessage::getSubject() const { return this->subject; };
std::string IMessage::getContent() const { return this->content; };
int IMessage::getMessageNumber() const { return this->messageNumber; };

IMessage *IMessage::setSender(std::string sender)
{
    this->sender = sender;
    return this;
};
IMessage *IMessage::setReceiver(std::string receiver)
{
    this->receiver = receiver;
    return this;
};
IMessage *IMessage::setSubject(std::string subject)
{
    this->subject = subject;
    return this;
};
IMessage *IMessage::setContent(std::string content)
{
    this->content = content;
    return this;
};
IMessage *IMessage::setMessageNumber(int imessageNumber)
{
    this->messageNumber = imessageNumber;
    return this;
};