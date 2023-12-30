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


#include "MessageHandler.h"
#include "Request.h"
#include "LoginMessage.h"
#include "FileHandler.h"
#include "LdapHandler.h"

namespace fs = std::filesystem;




class Controller
{
private:
    MessageHandler* messageHandler;
    void sendResponse(int, std::string);
    LdapHandler* ldapHandler;
    std::string username;

public:
    Controller();
    ~Controller();
    bool loginUser(int, LoginMessage*); // username, pwd
    void receiveMessage(Request);
    void listMessages(Request);
    void readMessage(Request);
    void deleteMessage(Request);
    void quit();
    bool isLoggedIn(Request);
    void sendErrorResponse(Request req);

};

#endif
