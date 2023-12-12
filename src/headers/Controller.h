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


#include "MessageHandler.h"
#include "Request.h"
#include "Message.h"
#include "RecursiveFileHandler.h"


class Controller
{
private:
    MessageHandler* messageHandler;
    void sendResponse(int*, std::string);

public:
    Controller();
    ~Controller();
    void receiveMessage(Request);
    void listMessages(Request);
    void readMessage(Request);
    void deleteMessage(Request);
    void quit();
};

#endif
