#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include "Message.h"
#include "IFileHandler.h"

namespace fs = std::filesystem;

// TODO constants in config file schreiben und von dort auslesen
// z.B.: msgsRootDir etc.

class MessageHandler
{
    IFileHandler* fileHandler;
    std::string msgsRootDir = "./messages/";

public:
    MessageHandler(IFileHandler* handler) : fileHandler(handler) {}
    ~MessageHandler();
    bool createMessage(const std::string& username, Message message);
    std::vector<Message> getMessagesByUsername(const std::string& username);
    std::string getMessage(const std::string& username, int messageNumber);
    bool deleteMessage(const std::string& username, int messageID);

};

#endif