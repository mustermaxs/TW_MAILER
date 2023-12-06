#include "../header/IItems/List.h"

List::List()
{
    throw new std::invalid_argument("No arguments provided to constructor of class Message.");
}

List::List(std::string username) { this->username = username; }

std::string List::GetUserName() const { return this->username; };

std::string List::ToString() const
{
    std::string res = "";

    for (Message message : messages)
    {
        res += message.GetSubject() + "\n";
    }

    return res;
};