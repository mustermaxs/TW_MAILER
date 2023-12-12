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

    std::cout << resBody << std::endl;

    // return resBody;
    sendResponse(req.getSocketId(), resBody);
};

void Controller::listMessages(Request req)
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
            resBody += message->getSubject() + "\n";
        }
    }
    sendResponse(req.getSocketId(), resBody);
};

void Controller::readMessage(Request req)
{
    std::string resBody = "";
    Message *requestMessage = req.getMessage();
    
    std::string username = requestMessage->getSender();
    int messageNumber = requestMessage->getMessageNumber();

    Message message = messageHandler->getMessage(username, messageNumber);
    
    resBody += message.getSender() + "\n";
    resBody += message.getReceiver() + "\n";
    resBody += message.getSubject() + "\n";
    resBody += message.getContent() + "\n";

    sendResponse(req.getSocketId(), resBody);
};

void Controller::deleteMessage(Request req){
    
        Message *requestMessage = req.getMessage();
        std::string username = requestMessage->getSender();
        int messageNumber = requestMessage->getMessageNumber();
    
        bool messageDeleted = messageHandler->deleteMessage(username, messageNumber);
    
        std::string resBody = "";
    
        if (!messageDeleted)
        {
            resBody = "ERR\n";
        }
        else
        {
            resBody = "OK\n";
        }
    
        sendResponse(req.getSocketId(), resBody);
};

void Controller::quit(){

};

void Controller::sendResponse(int socketId, std::string resBody)
{
    try
    {
    // BUG exception.what() => Send answer failed: Socket operation on non-socket
        if (send(socketId, resBody.c_str(), resBody.size(), 0) == -1)
        {
            // perror("Send answer failed");
            // throw std::runtime_error("Send answer failed");
        }
    }
    catch (const std::exception &ex)
    {
        // std::cout << "Exception: " << ex.what() << std::endl;
    }
};
