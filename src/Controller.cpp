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

    resBody += messagesCount + "\n";

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

    Message *requestMessage = req.getMessage();
    std::string username = requestMessage->getReceiver();
    int messageNumber;

    std::string resBody = "";

    // sendResponse(req.socketId, resBody);
};

void Controller::deleteMessage(Request req){

};

void Controller::quit(){

};

void Controller::sendResponse(int *socketId, std::string resBody)
{
    int resBodySize = resBody.length();
    try
    {
    // BUG exception.what() => Send answer failed: Socket operation on non-socket
        if (send(*(socketId), resBody.c_str(), resBodySize, 0) == -1)
        {
            perror("Send answer failed");
            throw std::runtime_error("Send answer failed");
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
};
