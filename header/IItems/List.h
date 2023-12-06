#ifndef LIST_H
#define LIST_H

#include <string>
#include <exception>
#include <vector>

#include "IItem.h"
#include "Message.h"

class List : IItem
{
    private:
        std::string username;
        std::vector<Message> messages;
    public:
        List() {};
        List(std::string username);
        std::string GetUserName() const;
        std::vector<Message> GetMessages();
        std::string toString() const override;
        
};

#endif