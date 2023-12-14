#include "./headers/MessageHandler.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

MessageHandler::MessageHandler(IFileHandler* fileHandler)
{
    this->fileHandler = fileHandler;
    this->msgsRootDir = ConnectionConfig::getInstance()->getBaseDirectory();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


MessageHandler::~MessageHandler()
{
    delete this->fileHandler;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Saves serialized message object to user specific directory.
/// @param username name of receiver as defined in message header.
/// @param message message content, including header (subject, receiver, sender, content).
/// @return bool, indicating if saving message was successful or not.
/// @note If directory for user doesn't exist, it will be created.

bool MessageHandler::saveMessage(const std::string &username, IMessage& message)
{
    std::string directoryName = this->msgsRootDir + username + "/";

    this->fileHandler->createDirectoryIfNotExists(directoryName);

    std::vector<std::string> fileNames = fileHandler->getFileNamesInDir(directoryName);

    // obtains the highest message index (n) in the directory
    // and assigns the saved message index n+1
    int highestID = 0;
    std::string id;

    id = "1";

    if (!fileNames.empty())
    {
        for (auto &fileName : fileNames)
        {
            id = fileName;
            highestID = std::max(highestID, std::stoi(id));
        }
    }

    std::string newFilePath = directoryName + std::to_string(highestID + 1);

    return this->fileHandler->writeToFile(newFilePath, message.toString());
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Gets deserialized message object from specific user and message id.
/// @param username name of sender
/// @param messageNumber index of message
/// @return Deserialized message object
/// @note If message doesn't exist, an exception will be thrown.
IMessage* MessageHandler::getMessage(const std::string &username, int messageNumber)
{
    std::string directoryName = this->msgsRootDir + username + "/";
    std::vector<std::string> fileNames = this->fileHandler->getFileNamesInDir(directoryName);
    std::string msgId = std::to_string(messageNumber);

    SearchResult res = this->fileHandler->searchFileInDirRecursively(msgId, directoryName);

    if (!res.fileExists)
    {
        return NULL;
        
        throw new std::invalid_argument("Message doesn't exist");
    }

    std::vector<std::string> fileContent = this->fileHandler->readFileLines(directoryName + msgId);
    
    IMessage* msg = Message::fromLines(fileContent);
    msg->setMessageNumber(messageNumber);

    return msg; // return pointer, not by value, so that call method can free allocation
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Gets all deserialized message objects for a specific user.
/// @param username message recipient.
/// @return vector of deserialized message objects
/// @note If user doesn't exist, an exception will be thrown.
std::vector<IMessage*>* MessageHandler::getMessagesByUsername(const std::string &username)
{
    std::vector<IMessage*>* messages = new std::vector<IMessage*>();
    std::string directoryName = this->msgsRootDir + username + "/";

    std::vector<std::string> fileNames = this->fileHandler->getFileNamesInDir(directoryName);

    for (auto &fileName : fileNames)
    {
        std::vector<std::string> fileLines = this->fileHandler->readFileLines(directoryName + fileName);
        Message* msg = Message::fromLines(fileLines);
        msg->setMessageNumber(std::stoi(fileName));
        messages->push_back(msg);
    }

    return messages;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Deletes message by username and message id.
/// @param username name of receiver.
/// @param messageID message id
/// @return Returns true if attempt to delete message  succeded.
/// @note If message doesn't exist, an exception will be thrown.
bool MessageHandler::deleteMessage(const std::string &username, int messageID)
{
    std::string directoryName = this->msgsRootDir + username + "/";
    std::string msgId = std::to_string(messageID);

    SearchResult res = this->fileHandler->searchFileInDirRecursively(msgId, directoryName);

    if (!res.fileExists)
    {
        return false;
    }

    return this->fileHandler->deleteFile(directoryName + msgId);
};
