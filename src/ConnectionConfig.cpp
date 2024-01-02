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

/// @brief Directory where messages get stored.
/// @param dirPath string - directory path.
void ConnectionConfig::setBaseDirectory(std::string dirPath)
{
    if (ConnectionConfig::baseDirectoryPath == "")
    {
        ConnectionConfig::baseDirectoryPath = dirPath[dirPath.size() - 1] != '/' ? dirPath += "/" : dirPath;
        FileHandler fileHandler = FileHandler();
        if (!fileHandler.createDirectoryIfNotExists(ConnectionConfig::baseDirectoryPath)) throw new std::invalid_argument("Directory doesn't exist.");
    }
};
std::string ConnectionConfig::getBaseDirectory()
{

    return ConnectionConfig::baseDirectoryPath;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Returns IP address of server.
/// @return string - IP adress.
std::string ConnectionConfig::getIP()
{
    return this->ip;
};

/// @brief Sets ip address (not in server! this just serves as a lookup object for other objects
/// that need information about the server config).
/// @param ip 
void ConnectionConfig::setIP(std::string ip)
{
    if (this->ip == "")
    {
        this->ip = ip;
    }
};
