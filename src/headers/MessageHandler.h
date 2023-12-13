#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <string>
#include <exception>

#include "Message.h"
#include "RecursiveFileHandler.h"
#include "ConnectionConfig.h"

namespace fs = std::filesystem;

// TODO constants in config file schreiben und von dort auslesen
// z.B.: msgsRootDir etc.

class MessageHandler
{
    IFileHandler* fileHandler;
    std::string msgsRootDir;

public:
    MessageHandler(IFileHandler* handler);
    ~MessageHandler();
    bool saveMessage(const std::string& username, IMessage& message);
    std::vector<IMessage*>* getMessagesByUsername(const std::string& username);
    IMessage* getMessage(const std::string& username, int messageNumber);
    bool deleteMessage(const std::string& username, int messageID);

};

#endif