#include "./headers/MessageHandler.h"


MessageHandler::~MessageHandler() {  };

bool MessageHandler::saveMessage(const std::string& username, Message message)
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


    return fileHandler->writeToFile(newFileName, message.toString());

}

Message MessageHandler::getMessage(const std::string& username, int messageNumber)
{
    std::string directoryName = this->msgsRootDir + username + "/";
    std::vector<std::string> fileNames = fileHandler->getFileNamesInDir(directoryName);
    std::string msgId = std::to_string(messageNumber);

    SearchResult res = fileHandler->searchFileInDir(msgId, directoryName);

    if (!res.fileExists)
    {
        throw new std::invalid_argument("Message doesn't exist");
    }

    std::vector<std::string> fileContent = fileHandler->readFileLines(directoryName+msgId);

    return Message::fromString(fileContent);
}

std::vector<Message> MessageHandler::getMessagesByUsername(const std::string& username)
{
    std::vector<Message> messages;
    std::string directoryName = this->msgsRootDir + username + "/";

    std::vector<std::string> fileNames = fileHandler->getFileNamesInDir(directoryName);

    for (auto& fileName : fileNames)
    {
        std::vector<std::string> fileContent = fileHandler->readFileLines(directoryName + fileName);
        

    }

    return messages;
}


bool MessageHandler::deleteMessage(const std::string& username, int messageID)
{
    return false;
}
