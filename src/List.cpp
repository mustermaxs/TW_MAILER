#include "./headers/List.h"

List::List()
{
    throw new std::invalid_argument("No arguments provided to constructor of class Message.");
}

List::List(std::string username) { this->username = username; }

std::string List::getUserName() const { return this->username; };

std::string List::toString() const
{
    std::string res = "";

    for (Message message : messages)
    {
        res += message.getSubject() + "\n";
    }

    return res;
};