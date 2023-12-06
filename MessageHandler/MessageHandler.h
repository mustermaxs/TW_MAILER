#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include "Message.h"
#include "../FileHandler/IFileHandler.h"

namespace fs = std::filesystem;


class MessageHandler
{

public:
    bool createMessage(const std::string& username, Message message);
    std::vector<Message> getMessagesByUsername(const std::string& username);
    std::string getMessage(const std::string& username, int messageNumber);
    bool deleteMessage(const std::string& username, int messageID);

};

#endif

/*
- bool createMessage(username, message)
    if userfolder does not exist


- Vector getUsermessages(username)
    if userfolder does not exist
        return Vector.empty

    return userfolder.messages

---

- string getMessage(username, messageNumber)
    return userfolder[messageNumber]

---

- bool deleteMessage(username, messageID)
    if sucess
        return true
    return false

*/