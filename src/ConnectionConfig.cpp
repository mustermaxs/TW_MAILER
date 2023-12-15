#include "headers/ConnectionConfig.h"


int ConnectionConfig::port = -1;
std::string ConnectionConfig::baseDirectoryPath = "";
std::string ConnectionConfig::ip = "";
bool ConnectionConfig::instanceExists = true;


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Create/return a single instance of the config.
/// @return ConnectionConfig*

ConnectionConfig *ConnectionConfig::config = NULL;

ConnectionConfig *ConnectionConfig::getInstance()
{

    if (ConnectionConfig::config == NULL)
    {
        ConnectionConfig::config = new ConnectionConfig();
    }

    return ConnectionConfig::config;
};

//////////////////////////////////////////////////////////////////////
// various getters/setters                                          //
//////////////////////////////////////////////////////////////////////

void ConnectionConfig::setPort(int port)
{
    if (this->port == -1)
    {
        this->port = port;
    }
};

int ConnectionConfig::getPort() { return this->port; };

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void ConnectionConfig::setBaseDirectory(std::string dirPath)
{
    if (ConnectionConfig::baseDirectoryPath == "")
    {
        ConnectionConfig::baseDirectoryPath = dirPath[dirPath.size() - 1] != '/' ? dirPath += "/" : dirPath;
        IFileHandler* fileHandler = new FileHandler();
        if (!fileHandler->createDirectoryIfNotExists(ConnectionConfig::baseDirectoryPath)) throw new std::invalid_argument("Directory doesn't exist.");
    }
};
std::string ConnectionConfig::getBaseDirectory()
{

    return ConnectionConfig::baseDirectoryPath;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

std::string ConnectionConfig::getIP()
{
    return this->ip;
};

void ConnectionConfig::setIP(std::string ip)
{
    if (this->ip == "")
    {
        this->ip = ip;
    }
};
