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

namespace fs = std::filesystem;

// TODO constants in config file schreiben und von dort auslesen
// z.B.: msgsRootDir etc.

class MessageHandler
{
    IFileHandler* fileHandler;
    // TODO msgsRootDir /var/spool/<usermessageDir>
    std::string msgsRootDir = "/home/mustermax/vscode_projects/TW_MAILER/messages/";

public:
    MessageHandler(IFileHandler* handler);
    ~MessageHandler();
    bool saveMessage(const std::string& username, Message message);
    std::vector<Message*>* getMessagesByUsername(const std::string& username);
    Message* getMessage(const std::string& username, int messageNumber);
    bool deleteMessage(const std::string& username, int messageID);

};

#endif