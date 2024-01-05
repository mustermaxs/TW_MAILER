#include "../src/headers/SocketServer.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Server socket constructor. Sets default port
/// and loads text files for console output (help/usage messages )
SocketServer::SocketServer()
{
    this->loadMessageTxts();
};

/// @brief Sets the directory where the messages are stored. Also stores them in
/// a static COnnectionsConfig object so different entities can access it easier.
/// @param spoolDir String - Directory where messages will be stored.
void SocketServer::setSpoolDir(std::string spoolDir)
{
    FileHandler fileHandler = FileHandler();
    this->msgDirPath = spoolDir[spoolDir.size() - 1] != '/' ? spoolDir += "/" : spoolDir;
    this->msgDirPath = spoolDir[0] != '/' ? "/" + this->msgDirPath : this->msgDirPath;

    if (!fileHandler.createDirectoryIfNotExists(this->msgDirPath))
        throw new std::invalid_argument("Directory doesn't exist.");

    ConnectionConfig::setBaseDirectory(this->msgDirPath);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Loads the console texts that are going to be displayed.
void SocketServer::loadMessageTxts()
{
    try
    {
        this->usageText = IFileHandler::readFile(this->filePathUsageTxt);
        this->waitForConnTxt = IFileHandler::readFile(this->filePathWaitForConnTxt);
        this->welcomeMsg = IFileHandler::readFile(this->filePathWelcomeTxt);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error loading text files. - " << e.what() << std::endl;
    }
};

bool SocketServer::makeConnection()
{
    return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Wrapper method that initializes the server.
/// Creates the server socket, initiliazes its address, binds the socket
/// and sets the server socket to listening mode.
/// @return bool - true if successfull.
bool SocketServer::init()
{
    this->printUsage();

    if (
        this->createSocket() &&
        this->initAddress() &&
        this->bindSocket() &&
        this->startListening())
    {
        return true;
    }

    return false;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Sets a flag indicating that the current request should get aborted.
void SocketServer::setAbortRequested(bool shouldAbort) { this->abortRequested = shouldAbort; };

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Creates the server socket.
/// @return bool - true if successful.
bool SocketServer::createSocket()
{
    int reuseValue = 1;

    if ((this->socketId = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");

        return false;
    }

    if (setsockopt(this->socketId, SOL_SOCKET, SO_REUSEADDR, &reuseValue, sizeof(reuseValue)) == -1)
    {
        perror("Set socket options - reuseAddr");

        return false;
    }

    if (setsockopt(this->socketId, SOL_SOCKET, SO_REUSEPORT, &reuseValue, sizeof(reuseValue)) == -1)
    {
        perror("Set socket options - reusePort");

        return false;
    }

    return true;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Initialized server socket address.
/// @return bool - true if successful.
bool SocketServer::initAddress()
{
    try
    {
        memset(&this->serverAddress, 0, sizeof(this->serverAddress));
        this->serverAddress.sin_family = AF_INET;
        this->serverAddress.sin_addr.s_addr = INADDR_ANY;
        this->serverAddress.sin_port = htons(this->port);

        return true;
    }
    catch (...)
    {
        return false;
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Prints usage instructions to server console.
void SocketServer::printUsage()
{
    std::cout << "Usage: ./twmailer-server <port> <mail-spool-directoryname>\n";
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Sends message on how to use the application to the client.
/// @param socketId int - socket id of client.
void SocketServer::sendHelpMessage(int socketId)
{
    this->sendData(this->welcomeMsg, socketId);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool SocketServer::bindSocket()
{
    // Assign an Address with Port to Socket
    if (bind(this->socketId, (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress)) == -1)
    {
        perror("Bind error");

        return false;
    }

    return true;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

std::string SocketServer::getClientIpBySocketId(int socketId)
{
    struct sockaddr_in addr = this->clientAddresses[socketId];

    return inet_ntoa(addr.sin_addr);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void SocketServer::setPort(int _port)
{
    this->port = _port;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool SocketServer::startListening()
{
    if (listen(this->socketId, 5) == -1)
    {
        perror("Listen error");

        return false;
    }

    std::cout << this->waitForConnTxt << std::endl;

    return true;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool SocketServer::shouldAbortRequest() { return this->abortRequested; };

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Stops the socket server. Closes server socket.
void SocketServer::stopServer()
{
    if (this->socketId != -1)
    {
        if (shutdown(this->socketId, SHUT_RDWR) == -1)
            std::cerr << "Shutdown new socket" << std::endl;

        if (close(this->socketId) != -1)
            std::cerr << "Close new socket" << std::endl;

        // this->socketId = -1;
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Checks if port number was set. Throws runtime_error otherwise.
void SocketServer::checkIfSetupComplete()
{
    if (this->port < 0)
        throw new std::runtime_error("Server not set up correctly.");
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int SocketServer::acceptConnectionAndGetSocketId()
{
    int newClientSocketId;
    struct sockaddr_in newClientSocketAddr;
    socklen_t clientAddrLength = sizeof(struct sockaddr_in);

    try
    {
        newClientSocketId = accept(this->socketId, (struct sockaddr *)&newClientSocketAddr, &clientAddrLength);
    }
    catch (...)
    {
        std::cout << "Will be going to be stopping to doing accepting connections." << std::endl;

        return -1;
    }

    if ((newClientSocketId) == -1)

    {
        if (this->abortRequested)
        {
            return -1;
        }
        else
        {
            return -1;
        }
    }

    std::cout << "Client connected from " << inet_ntoa(newClientSocketAddr.sin_addr) << ":" << ntohs(newClientSocketAddr.sin_port) << "...\n";

    if (this->sendData(this->welcomeMsg, newClientSocketId) == false)
    {
        this->abortRequested = true;

        return -1;
    }

    this->clientSocketIds.push_back(newClientSocketId);

    return newClientSocketId;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Sets the max. number of possible clients in queue before
/// additional clients get rejected by the server.
/// @param nbr int - max. number of clients in queue.
void SocketServer::setNbrClientsBeforeQueued(int nbr)
{
    this->nbrOfClientsBeforeQueued = nbr;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Returns all the socket ids of all connected clients.
/// @return vector<int> - Vector of client socket ids.
std::vector<int> SocketServer::getClientSocketIds() { return this->clientSocketIds; };
