#include "headers/ConnectionConfig.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Create/return a single instance of the config.
/// @return ConnectionConfig*

ConnectionConfig* ConnectionConfig::config = NULL;

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
    if (this->baseDirectoryPath == "")
    {
        this->baseDirectoryPath = dirPath;
    }
};
std::string ConnectionConfig::getBaseDirectory()
{

    return this->baseDirectoryPath;
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
