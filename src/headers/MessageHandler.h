#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <string>
#include <exception>

#include "Message.h"
#include "FileHandler.h"
#include "ConnectionConfig.h"

namespace fs = std::filesystem;

// TODO constants in config file schreiben und von dort auslesen
// z.B.: msgsRootDir etc.

//////////////////////////////////////////////////////////////////////
// Facade that takes care (indirectly) of all the file handling
// related to operations with messages.
// e.g. reading/writing/deleting messages, creating userspecific dirs etc.
//////////////////////////////////////////////////////////////////////

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