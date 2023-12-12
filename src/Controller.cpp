#include "./headers/Controller.h"

Controller::Controller()
{
    this->messageHandler = new MessageHandler(new RecursiveFileHandler());
};

Controller::~Controller()
{
    delete messageHandler;
};

void Controller::receiveMessage(Request req)
{
    try
    {
        Message *requestMessage = req.getMessage();
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

void Controller::listMessages(Request req)
{
    try
    {
        Message *requestMessage = req.getMessage();
        std::string username = requestMessage->getSender();

        std::vector<Message *> *messages = messageHandler->getMessagesByUsername(username);

        int messagesCount = messages->size();

        std::string resBody = "";

        std::string messagesCountStr = std::to_string(messagesCount);

        resBody += messagesCountStr + "\n";

        if (messagesCount)
        {

            for (const Message *message : *messages)
            {
                resBody += "ID: " + std::to_string(message->getMessageNumber()) + " | Subject: " + message->getSubject() + "\n";
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
};

void Controller::readMessage(Request req)
{
    try
    {
        std::string resBody = "";
        Message *requestMessage = req.getMessage();

        std::string username = requestMessage->getSender();
        int messageNumber = requestMessage->getMessageNumber();

        Message *message = messageHandler->getMessage(username, messageNumber);

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

void Controller::deleteMessage(Request req)
{

    try {

        Message *requestMessage = req.getMessage();
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
