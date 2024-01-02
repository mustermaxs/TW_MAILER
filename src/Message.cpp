#include "./headers/Message.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Message::Message()
{
    this->sender = "";
    this->receiver = "";
    this->subject = "";
    this->content = "";
    this->messageNumber = 0;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Serializes message object. Prepends sturcture elements
/// so that parsing is easier.
/// @return String - serialized message object.
std::string Message::toString()
{
    std::string res =
        "SENDER:" + this->sender + "\n" +
        "RECEIVER:" + this->receiver + "\n" +
        "SUBJECT:" + this->subject + "\n" +
        "MESSAGE:" + this->content + "\n";

    return res;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Deserizalizes message object from string.
/// @param msgLines Vector of strings.
/// @return Pointer to message object.
Message *Message::fromLines(const std::vector<std::string> msgLines)
{
    Message *msgBase = new Message();
    Message *msg = msgBase->parse(msgLines);

    return msg;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Message *Message::parse(const std::vector<std::string> msgLines)
{
    Message *msg = new Message();
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
        std::string delimiter = ":";
        std::string key = line.substr(0, line.find(delimiter));
        std::string value = line.substr(line.find(delimiter) + 1, line.length());

        if (key == "SENDER")
            msg->setSender(value);
        if (key == "RECEIVER")
            msg->setReceiver(value);
        if (key == "ID")
            msg->setMessageNumber(std::stoi(value));
        if (key == "SUBJECT")
            msg->setSubject(value);
        if (key == "MESSAGE")
        {
            readingMsgBody = true;
            msgBody += value + "\n";

            continue;
        }

        if (readingMsgBody)
        {
            msgBody = msgBody + line + "\n";

            if (line == msgLines.back())
            {

                readingMsgBody = false;
            }
        }
    }
    msg->setContent(msgBody);
    
    return msg;
};