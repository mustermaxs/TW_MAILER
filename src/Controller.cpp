#include "./headers/Controller.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Controller::Controller()
{
    this->messageHandler = new MessageHandler(new FileHandler());
    this->ldapHandler = new LdapHandler();
    this->loginAttempts = 0;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Controller::~Controller()
{
    delete messageHandler;
};

std::mutex Controller::blacklistMutex;

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
        std::string username = this->username;
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
        std::cerr << "Error in listMessages" << std::endl;

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

        std::string username = this->username;
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
    catch (const std::invalid_argument &ex)
    {
        std::cerr << "Error in readMessage: " << ex.what() << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
    catch (const std::runtime_error &ex)
    {
        std::cerr << "Error in readMessage: " << ex.what() << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
    catch (...)
    {
        std::cerr << "Error in readMessage" << std::endl;

        sendResponse(req.getSocketId(), "ERR\n");
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief On DELETE command. Handles clients request to delete a specific message.
/// @param req Request.
void Controller::deleteMessage(Request req)
{

    try
    {

        IMessage *requestMessage = req.getMessage();
        std::string username = this->username;
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
    catch (const std::runtime_error &ex)
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

bool Controller::isLoggedIn(Request req)
{
    if (this->username != "")
        return true;
    else
        return false;
};

bool Controller::loginUser(Request req, LoginMessage *msg)
{
    std::string username = msg->getUsername();
    std::string password = msg->getPassword();

    if (this->isBlacklisted(req.getIp()))
    {
        this->sendBannedResponse(req);

        return false;
    }
    if (this->loginAttempts >= 3)
    {
        this->banUser(req);
        this->sendResponse(req.getSocketId(), "ERR");

        return false;
    }

    if (!this->ldapHandler->tryLoginUser(username, password))
    {
        this->sendResponse(req.getSocketId(), "ERR");
        return false;
    }

    this->username = username;

    this->loginAttempts++;
    this->sendResponse(req.getSocketId(), "OK");

    return true;
};

void Controller::banUser(Request req)
{
    this->userIsBanned = true;
    this->putIpOnBlacklist(req.getIp());
    this->loginAttempts = 0;
};

void Controller::putIpOnBlacklist(std::string ip)
{
    Controller::blacklistMutex.lock();
    FileHandler fh = FileHandler();

    auto currentTimeStamp = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimeStamp);

    std::tm *localTime = std::localtime(&currentTime);
    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    std::string timeAndIp = ss.str() + "@" + ip;

    fh.writeToFile("./blacklist.txt", timeAndIp);
    Controller::blacklistMutex.unlock();
};

bool Controller::isBlacklisted(std::string ip)
{
    Controller::blacklistMutex.lock();

    FileHandler fh = FileHandler();
    std::vector<std::string> lines = fh.readFileLines("./blacklist.txt");

    for (auto &line : lines)
    {
        if (line.find(ip) != std::string::npos)
        {
            std::string time = line.substr(0, 19);
            std::tm tm = {};
            std::istringstream ss(time);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));

            auto currentTimeStamp = std::chrono::system_clock::now();

            // check if time is older than 1 minute
            if (std::chrono::duration_cast<std::chrono::minutes>(currentTimeStamp - timePoint).count() < 1)
            {
                this->removeFromBlacklist(ip);

                return true;
            }
        }
    }

    Controller::blacklistMutex.unlock();

    return false;
};

void Controller::removeFromBlacklist(std::string ip)
{
    Controller::blacklistMutex.lock();
    FileHandler fh = FileHandler();
    std::vector<std::string> lines = fh.readFileLines("./blacklist.txt");

    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].find(ip) != std::string::npos)
        {
            lines.erase(lines.begin() + i);
        }
    }

    std::string linesStr = "";
    for (auto &line : lines)
    {
        linesStr += line + "\n";
    }

    fh.writeToFile("./blacklist.txt", linesStr);
    Controller::blacklistMutex.unlock();
};

void Controller::sendErrorResponse(Request req)
{
    this->sendResponse(req.getSocketId(), "Not authorized. Must log in first.");
};

void Controller::sendBannedResponse(Request req)
{
    this->sendResponse(req.getSocketId(), "You are banned.");
};
