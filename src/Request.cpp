#include "./headers/Request.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Request object. Gets build from the string a clients sends to the server.
/// @param method enum Command - An enum referencing a valid command.
/// @param socketId int - the socket id.
/// @param message Message - a message object containing various informations about
/// the request.
Request::Request(Command method, int socketId, IMessage* message, std::string ip)
{
    this->method = method;
    this->socketId = socketId;
    this->message = message;
    this->ip = ip;
};

IMessage* Request::getMessage()
{
    return this->message;
};

int Request::getSocketId()
{
    return this->socketId;
};

std::string Request::getIp()
{
    return this->ip;
};
