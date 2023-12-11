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

Message Message::fromString(const std::string msgString)
{
    std::vector<std::string> msgLines;
    std::string delimiter = "\n";
    std::string line = msgString.substr(0, msgString.find(delimiter));

    while (line.length() > 0)
    {
        msgLines.push_back(line);
        msgString.erase(0, line.length() + delimiter.length());
        line = msgString.substr(0, msgString.find(delimiter));
    }

    return Message::fromFile(msgLines);
};

Message Message::fromFile(const std::vector<std::string> msgLines)
{
    Message msg;
    std::map<std::string, std::string> msgMap = {
        {"SENDER", ""},
        {"RECEIVER", ""},
        {"SUBJECT", ""},
        {"MESSAGE", ""}};

    for (const auto &line : msgLines)
    {
        // split by first :
        std::string delimiter = ":";
        std::string key = line.substr(0, line.find(delimiter));
        std::string value = line.substr(line.find(delimiter) + 1, line.length());

        msgMap[key] = value;
    }

    msg.setSender(msgMap["SENDER"]);
    msg.setReceiver(msgMap["RECEIVER"]);
    msg.setSubject(msgMap["SUBJECT"]);
    msg.setContent(msgMap["MESSAGE"]);

    return msg;
};
