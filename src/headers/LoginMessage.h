#ifndef LOGINMESSAGE_H
#define LOGINMESSAGE_H

#include "IMessage.h"
#include <exception>

class LoginMessage : public IMessage
{
private:
    std::string username;
    std::string password;

public:
    LoginMessage();
    ~LoginMessage() override{};
    std::string getUsername() const;
    std::string getPassword() const;
    static LoginMessage* fromLines(const std::vector<std::string> msgLines);
    std::string toString() override { throw new std::runtime_error("Dont't use this!"); return ""; };
    LoginMessage *parse(const std::vector<std::string> msgLines) override;

    void setUsername(std::string username);
    void setPassword(std::string password);
};

#endif