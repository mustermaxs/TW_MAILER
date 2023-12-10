#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include "./headers/Message.h"
#include "./headers/MessageHandler.h"
#include "./headers/IFileHandler.h"

MessageHandler::~MessageHandler() {  };

bool MessageHandler::createMessage(const std::string& username, Message message)
{
    //create directoryname from username
    std::string directoryName = this->msgsRootDir + username + "/";

    fileHandler->createDirectoryIfNotExists(directoryName);

    std::vector<std::string> fileNames = fileHandler->getFileNamesInDir(directoryName);
    
    //iterate over filenames and extract highest id
    int highestID = 0;
    std::string id;

    id = "1";

    if (!fileNames.empty())
    {
        for (auto& fileName : fileNames)
        {
            id = fileName;
            highestID = std::max(highestID, std::stoi(id));
        }
    }

    std::string newFileName = directoryName + std::to_string(highestID + 1);


    fileHandler->writeToFile(newFileName, message.toString());

}

std::vector<Message> MessageHandler::getMessagesByUsername(const std::string& username)
{
    return std::vector<Message>();
}

std::string MessageHandler::getMessage(const std::string& username, int messageNumber)
{
    return "";
}

bool MessageHandler::deleteMessage(const std::string& username, int messageID)
{
    return false;
}
