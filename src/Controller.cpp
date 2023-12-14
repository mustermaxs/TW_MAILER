#include "./headers/Controller.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Controller::Controller()
{
    this->messageHandler = new MessageHandler(new FileHandler());
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


Controller::~Controller()
{
    delete messageHandler;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief On SEND commmand. Handles messages sent by the client.
/// @param req Request.
void Controller::receiveMessage(Request req)
{
    try
    {
        IMessage *requestMessage = req.getMessage();
        std::string username = requestMessage->getReceiver();

        bool messageCreated = messageHandler->saveMessage(username, *requestMessage);

        std::string resBody = "";

        if (!messageCreated)
        {
            resBody = "ERR\n";
        }
        else
        {
            resBody = "OK\n";
        }

        sendResponse(req.getSocketId(), resBody);
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error in receiveMessage: " << ex.what() << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief On LIST command. Handles request to list all messages for a specific user.
/// @param req Request.
void Controller::listMessages(Request req)
{
    try
    {
        IMessage *requestMessage = req.getMessage();
        std::string username = requestMessage->getReceiver();
        std::vector<IMessage *> *messages = messageHandler->getMessagesByUsername(username);
        int messagesCount = messages->size();

        std::string resBody = "";
        std::string messagesCountStr = std::to_string(messagesCount);
        resBody = resBody + "Number of messages: " + messagesCountStr + "\n";

        if (messagesCount)
        {
            for (const auto &message : *messages)
            {
                resBody += "[" + std::to_string(message->getMessageNumber()) + "] " + message->getSubject() + "\n";
            }
        }

        for (auto &msg : *messages)
        {
            delete msg;
        }
        delete messages;

        sendResponse(req.getSocketId(), resBody);
    }
    catch (const std::runtime_error &ex)
    {
        std::cerr << "Error in listMessages: " << ex.what() << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
    catch (...)
    {
        std::cerr << "Error in listMessages: " << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief On READ command. Handles client request to read a specific message.
/// @param req Request.
void Controller::readMessage(Request req)
{
    try
    {
        std::string resBody = "";
        IMessage *requestMessage = req.getMessage();

        std::string username = requestMessage->getReceiver();
        int messageNumber = requestMessage->getMessageNumber();

        IMessage *message = messageHandler->getMessage(username, messageNumber);

        if (message == NULL)
        {
            throw std::runtime_error("Message not found");
        }

        resBody += "Sender:" + message->getSender() + "\n";
        resBody += "Receiver:" + message->getReceiver() + "\n";
        resBody += "Subject:" + message->getSubject() + "\n";
        resBody += "Content:" + message->getContent() + "\n";

        delete message;

        sendResponse(req.getSocketId(), resBody);
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << "Error in readMessage: " << ex.what() << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief On DELETE command. Handles clients request to delete a specific message.
/// @param req Request.
void Controller::deleteMessage(Request req)
{

    try {

        IMessage *requestMessage = req.getMessage();
        std::string username = requestMessage->getSender();
        int messageNumber = requestMessage->getMessageNumber();

        bool messageDeleted = messageHandler->deleteMessage(username, messageNumber);

        std::string resBody = "";

        if (!messageDeleted)
        {
            resBody = "ERR\n";
            throw std::runtime_error("Message not found");
        }
        else
        {
            resBody = "OK\n";
        }

        delete requestMessage;

        sendResponse(req.getSocketId(), resBody);
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << "Error in deleteMessage: " << ex.what() << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/// @brief Used to send response to client.
/// @param socketId int - socket ID
/// @param resBody string - the response body.
void Controller::sendResponse(int socketId, std::string resBody)
{
    try
    {
        // BUG exception.what() => Send answer failed: Socket operation on non-socket
        if (send(socketId, resBody.c_str(), resBody.size(), 0) == -1)
        {
            perror("Send answer failed");
            throw std::runtime_error("Send answer failed");
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << "Exception while sending Response: " << ex.what() << std::endl;
    }
};
