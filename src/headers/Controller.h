#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <mutex>

#include "MessageHandler.h"
#include "Request.h"
#include "LoginMessage.h"
#include "FileHandler.h"
#include "LdapHandler.h"
#include "Response.h"
#include "Utils.h"

namespace fs = std::filesystem;




class Controller
{
private:
    MessageHandler* messageHandler;
    void sendResponse(int, std::string);
    LdapHandler* ldapHandler;
    std::string username;
    int loginAttempts;
    bool userIsBanned;
    std::string blackListFilePath;

public:
    static std::mutex blacklistMutex;
    Controller();
    ~Controller();
    bool loginUser(Request, LoginMessage*); // username, pwd
    void receiveMessage(Request);
    void listMessages(Request);
    void readMessage(Request);
    void deleteMessage(Request);
    void quit();
    bool isLoggedIn(Request);
    void sendErrorResponse(Request);
    void sendBannedResponse(Request);
    void banUser(Request);
    void putIpOnBlacklist(std::string);
    bool isBlacklisted(std::string);
    void removeFromBlacklist(std::string);

};

#endif
