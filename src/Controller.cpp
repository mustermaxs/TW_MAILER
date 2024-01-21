#include "./headers/Controller.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Controller::Controller()
{
    this->messageHandler = new MessageHandler(new FileHandler());
    this->ldapHandler = new LdapHandler();
    this->loginAttempts = 0;

    fs::path currentPath = std::filesystem::current_path().parent_path();

    this->blackListFilePath = FileHandler().pathObjToString(currentPath) + "/configs/blacklist.txt";
};


//////////////////////////////////////////////////////////////////////v
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

        Response::normal(req.getSocketId(), resBody);
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error in receiveMessage: " << ex.what() << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");
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

        Response::normal(req.getSocketId(), resBody);
    }
    catch (const std::runtime_error &ex)
    {
        std::cerr << "Error in listMessages: " << ex.what() << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");
    }
    catch (...)
    {
        std::cerr << "Error in listMessages" << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");
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

        Response::normal(req.getSocketId(), resBody);
    }
    catch (const std::invalid_argument &ex)
    {
        std::cerr << "Error in readMessage: " << ex.what() << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");
    }
    catch (const std::runtime_error &ex)
    {
        std::cerr << "Error in readMessage: " << ex.what() << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");
    }
    catch (...)
    {
        std::cerr << "Error in readMessage" << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");
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

        Response::normal(req.getSocketId(), resBody);
    }
    catch (const std::runtime_error &ex)
    {
        std::cerr << "Error in deleteMessage: " << ex.what() << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Used to send response to client.
/// @param socketId int - socket ID
/// @param resBody string - the response body.
// OBSOLETE
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

///@brief Handles the login of a user.
///@param req Request.
///@param msg LoginMessage.
///@return true if login was successful, false otherwise.
bool Controller::loginUser(Request req, LoginMessage *msg)
{
    try
    {

        std::string username = msg->getUsername();
        std::string password = msg->getPassword();

        if (this->isBlacklisted(req.getIp()))
        {
            this->sendBannedResponse(req);

            return false;
        }

        if (this->isLoggedIn(req))
        {
            Response::normal(req.getSocketId(), "You are already logged in.");
            return true;
        }

        this->loginAttempts++;

        if (this->loginAttempts >= 3)
        {
            this->banUser(req);
            Response::normal(req.getSocketId(), "ERR");

            return false;
        }

        if (!this->ldapHandler->tryLoginUser(username, password))
        {
            Response::normal(req.getSocketId(), "ERR");
            return false;
        }

        this->username = username;
        FileHandler fileHandler = FileHandler();
        std::string rootDirectory = ConnectionConfig::getBaseDirectory();
        std::string directoryName = rootDirectory + username + "/";
        fileHandler.createDirectoryIfNotExists(directoryName);

        Response::normal(req.getSocketId(), "OK");

        return true;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error in loginUser: " << ex.what() << std::endl;

        Response::normal(req.getSocketId(), "ERR\n");

        return false;
    }
};

void Controller::banUser(Request req)
{
    this->userIsBanned = true;
    this->putIpOnBlacklist(req.getIp());
    this->loginAttempts = 0;
};

void Controller::putIpOnBlacklist(std::string ip)
{
    try
    {

        Controller::blacklistMutex.lock();
        FileHandler fh = FileHandler();

        std::string currTime = Utils::getCurrTimeAsString("%Y-%m-%d %H:%M:%S");
        std::string timeAndIp = currTime + "@" + ip;

        fh.writeToFile(this->blackListFilePath, timeAndIp);
        Controller::blacklistMutex.unlock();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error in putIpOnBlacklist: " << ex.what() << std::endl;
    }
};

bool Controller::isBlacklisted(std::string ip)
{
    try {

    Controller::blacklistMutex.lock();

    FileHandler fh = FileHandler();
    std::vector<std::string> lines = fh.readFileLines(this->blackListFilePath);

    for (auto &line : lines)
    {
        if (line.find(ip) != std::string::npos)
        {
            std::string time = line.substr(0, line.find("@"));
            std::tm tm = {};
            std::istringstream ss(time);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));

            auto currentTimeStamp = std::chrono::system_clock::now();

            // check if time is older than 1 minute
            if (std::chrono::duration_cast<std::chrono::minutes>(currentTimeStamp - timePoint).count() > 1)
            {
                Controller::blacklistMutex.unlock();
                this->removeFromBlacklist(ip);

                return false;
            }
            else
            {
                Controller::blacklistMutex.unlock();

                return true;
            }
        }
    }

    Controller::blacklistMutex.unlock();

    return false;
    
    } catch (const std::exception &ex) {
        Controller::blacklistMutex.unlock();
        std::cerr << "Error in isBlacklisted: " << ex.what() << std::endl;
        return false;
    }
};

void Controller::removeFromBlacklist(std::string ip)
{
    try {

    Controller::blacklistMutex.lock();
    FileHandler fh = FileHandler();

    std::vector<std::string> lines = fh.readFileLines(this->blackListFilePath);

    for (size_t i = 0; i < lines.size(); i++)
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

    fh.writeToFile(this->blackListFilePath, linesStr);
    Controller::blacklistMutex.unlock();
    
    }
    catch (const std::exception &ex) {
        Controller::blacklistMutex.unlock();
        std::cerr << "Error in removeFromBlacklist: " << ex.what() << std::endl;
    }
};

void Controller::sendErrorResponse(Request req)
{
    Response::normal(req.getSocketId(), "Not authorized. Must log in first.");
};

void Controller::sendBannedResponse(Request req)
{
    Response::normal(req.getSocketId(), "You are banned.");
};
