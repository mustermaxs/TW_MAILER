#include "./headers/MessageHandler.h"


MessageHandler::~MessageHandler() {  };



/// @brief Saves serialized message object to user specific directory.
/// @param username name of receiver as defined in message header.
/// @param message message content, including header (subject, receiver, sender, content).
/// @return bool, indicating if saving message was successful or not.
bool MessageHandler::saveMessage(const std::string& username, Message message)
{
    std::string directoryName = this->msgsRootDir + username + "/";

    fileHandler->createDirectoryIfNotExists(directoryName);

    std::vector<std::string> fileNames = fileHandler->getFileNamesInDir(directoryName);
    
    // obtains the highest message index (n) in the directory
    // and assigns the saved message index n+1
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
};




/// @brief Gets deserialized message object from specific user and message id.
/// @param username name of sender
/// @param messageNumber index of message
/// @return Deserialized message object
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
};




/// @brief Gets all deserialized message objects for a specific user.
/// @param username message recipient.
/// @return vector of deserialized message objects
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
};




/// @brief Deletes message by username and message id.
/// @param username name of receiver.
/// @param messageID message id
/// @return Returns true if attempt to delete message  succeded.
bool MessageHandler::deleteMessage(const std::string& username, int messageID)
{
    return false;
};
