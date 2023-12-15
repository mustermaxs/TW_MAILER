#ifndef CONNECTIONCONFIG_H
#define CONNECTIONCONFIG_H

#include <string>
#include <exception>

#include "IFileHandler.h"
#include "FileHandler.h"

// serves as a lookup object for other entities that need
// information about the server config (like IP address, socket id etc.)
class ConnectionConfig
{
private:
    static ConnectionConfig *config;
    static int port;
    static std::string ip;
    static std::string baseDirectoryPath;
    static bool instanceExists;

protected:
    ConnectionConfig(){};

public:
    static ConnectionConfig *getInstance();
    void setIP(std::string);
    std::string getIP();
    static void setBaseDirectory(std::string);
    static std::string getBaseDirectory();
    void setPort(int);
    int getPort();
};

#endif