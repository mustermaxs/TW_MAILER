#include "../src/headers/SocketServer.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

SocketServer::SocketServer()
{
    this->port = std::stoi(IFileHandler::readFile(this->filePathPortNbr));
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void SocketServer::loadMessageTxts()
{
    this->usageText = IFileHandler::readFile(this->filePathUsageTxt);
    this->waitForConnTxt = IFileHandler::readFile(this->filePathWaitForConnTxt);
    this->welcomeMsg = IFileHandler::readFile(this->filePathWelcomeTxt);
};

bool SocketServer::makeConnection()
{
    return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

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

bool SocketServer::initAddress()
{
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;

    return false;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void SocketServer::printUsage()
{
    std::cout << "Usage: ./twmailer-server <port> <mail-spool-directoryname>\n";
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

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

void SocketServer::setPort(int _port)
{
    this->port = _port;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool SocketServer::startListening()
{
    if (listen(this->socketId, this->nbrOfClientsBeforeQueued) == -1)
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

void SocketServer::stopServer()
{
    if (this->socketId != -1)
    {
        if (shutdown(this->socketId, SHUT_RDWR) == -1)
            std::cerr << "Shutdown new socket" << std::endl;

        if (close(this->socketId) != -1)
            std::cerr << "Close new socket" << std::endl;

        this->socketId = -1;
    }
};

void SocketServer::checkIfSetupComplete()
{
    if (!this->port > -1)
        throw new std::runtime_error("Server not set up correctly.");
};

void SocketServer::startServer(){
    // #facade - multiple method calls here
};

int SocketServer::acceptConnectionAndGetSocketId()
{
    int newClientSocketId = -1;
    struct sockaddr_in newClientSocketAddr;
    socklen_t clientAddrLength = sizeof(struct sockaddr_in);

    if ((newClientSocketId = accept(this->socketId, (struct sockaddr *)&newClientSocketAddr, &clientAddrLength)))
    {
        if (this->abortRequested)
        {
            perror("Accept error after aborted");

            return -1;
        }
        else
        {
            perror("Accept error");

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

// void SocketServer::handleRequest(int clientSocketId)
// {

//     int size = -1;
// };
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void SocketServer::setNbrClientsBeforeQueued(int nbr)
{
    this->nbrOfClientsBeforeQueued = nbr;
};

std::vector<int> SocketServer::getClientSocketIds() { return this->clientSocketIds; };

// void SocketServer::closeConnection(int clientSocketId)
// {

// };

