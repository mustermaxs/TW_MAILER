#ifndef CONNECTIONCONFIG_H
#define CONNECTIONCONFIG_H

#include <string>
#include <exception>

class ConnectionConfig
{
private:
    static ConnectionConfig *config;
    int port = -1;
    std::string ip = "";
    std::string baseDirectoryPath = "";
    bool instanceExists;

protected:
    ConnectionConfig() :ip(""), baseDirectoryPath(""), port(-1), instanceExists(true) {};

public:
    static ConnectionConfig *getInstance();
    void setIP(std::string);
    std::string getIP();
    void setBaseDirectory(std::string);
    std::string getBaseDirectory();
    void setPort(int);
    int getPort();
};

#endif