#include "./headers/Request.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Request object. Gets build from the string a clients sends to the server.
/// @param method enum Command - An enum referencing a valid command.
/// @param socketId int - the socket id.
/// @param message Message - a message object containing various informations about
/// the request.
Request::Request(Command method, int socketId, IMessage* message)
{
    this->method = method;
    this->socketId = socketId;
    this->message = message;
};

IMessage* Request::getMessage()
{
    return this->message;
};

int Request::getSocketId()
{
    return this->socketId;
};