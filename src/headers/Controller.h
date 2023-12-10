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

class MessageHandler;

class Controller
{
private:
    MessageHandler messageHandler;

public:
    Controller();

    void ReceiveMessage(Request req);
    void ListMessages(Request req);
    void ReadMessage(Request req);
    void DeleteMessages(Request req);
    void Quit();
};

#endif
