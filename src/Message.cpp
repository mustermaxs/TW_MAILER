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
int Message::getMessageNumber() const { return this->messageNumber; };

Message *Message::setSender(std::string sender)
{
    this->sender = sender;
    return this;
};
Message *Message::setReceiver(std::string receiver)
{
    this->receiver = receiver;
    return this;
};
Message *Message::setSubject(std::string subject)
{
    this->subject = subject;
    return this;
};
Message *Message::setContent(std::string content)
{
    this->content = content;
    return this;
};
Message *Message::setMessageNumber(int messageNumber)
{
    this->messageNumber = messageNumber;
    return this;
};


std::string Message::toString() const
{
    std::string res =
        "SENDER:" + this->sender + "\n" +
        "RECEIVER:" + this->receiver + "\n" +
        "SUBJECT:" + this->subject + "\n" +
        "MESSAGE:" + this->content + "\n";

    return res;
};

// Message* Message::fromString(const std::string msgString)
// {
        
// };

// TODO evtl. string übergeben und nicht vector of strings
Message* Message::fromLines(const std::vector<std::string> msgLines)
{
    Message* msg = new Message();
    std::map<std::string, std::string> msgMap = {
        {"SENDER", ""},
        {"ID", ""},
        {"RECEIVER", ""},
        {"SUBJECT", ""},
        {"MESSAGE", ""}};
        
    bool readingMsgBody = false;
    std::string msgBody = "";

    for (const auto &line : msgLines)
    {
        // split by first :
        std::string delimiter = ":";
        std::string key = line.substr(0, line.find(delimiter));
        std::string value = line.substr(line.find(delimiter) + 1, line.length());

        if (key == "SENDER") msg->setSender(value);
        if (key == "RECEIVER") msg->setReceiver(value);
        if (key == "ID") msg->setMessageNumber(std::stoi(value));
        if (key == "SUBJECT") msg->setSubject(value);
        if (key == "MESSAGE")
        { 
            readingMsgBody = true;
            msgBody += value + "\n";

            continue;
        }

        if (readingMsgBody)
        {
            msgBody = msgBody + line + "\n";

            if (line == ".")
            {
                msg->setContent(msgBody);
                readingMsgBody = false;

            }
        }
    }

    return msg;
};
