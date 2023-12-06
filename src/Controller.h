#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../MessageHandler/MessageHandler.h"
#include "Requests/Request.h"

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
