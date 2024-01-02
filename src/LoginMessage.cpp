#include "headers/LoginMessage.h"

LoginMessage::LoginMessage()
{
    this->username = "";
    this->password = "";
};

void LoginMessage::setPassword(std::string pwd) { this->password = pwd; };
void LoginMessage::setUsername(std::string username) { this->username = username; };

std::string LoginMessage::getUsername() const
{
    return this->username;
};

std::string LoginMessage::getPassword() const
{
    return this->password;
};

LoginMessage *LoginMessage::parse(const std::vector<std::string> msgLines)
{
    LoginMessage *msg = new LoginMessage();
    msg->username = msgLines[0];
    msg->password = msgLines[1];

    return msg;
};

LoginMessage *LoginMessage::fromLines(const std::vector<std::string> msgLines)
{
    LoginMessage *msg = new LoginMessage();
    std::map<std::string, std::string> msgMap = {
        {"USERNAME", ""},
        {"PASSWORD", ""}};

    for (const auto &line : msgLines)
    {
        std::string delimiter = ":";
        std::string key = line.substr(0, line.find(delimiter));
        std::string value = line.substr(line.find(delimiter) + 1, line.length());

        if (key == "USERNAME")
            msg->setUsername(value);
        if (key == "PASSWORD")
            msg->setPassword(value);
    }

    return msg;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
