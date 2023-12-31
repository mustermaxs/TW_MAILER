#include "headers/Router.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


Router::Router(Controller* controller) : controller(controller) {};

Router::~Router() { delete this->controller; };

/// @brief Executes the correct controlelr method to handle the client request.
/// @param socketId int - to send response.
/// @param buffer string - content of the request.
/// @param ip string - ip of the client.
void Router::mapRequestToController(int socketId, std::string buffer, std::string ip)
{
    std::string commandStr = buffer.substr(0, buffer.find("\n"));

    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss(buffer);

    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }
    
    Command command = Utils::mapStringToCommand(commandStr);
    IMessage*message = Message::fromLines(lines);
    
    Request request = Request(command, socketId, message, ip);

    if(controller->isBlacklisted(request.getIp()))
    {
        Response::normal(request.getSocketId(), "You're banned.");
        return;
    }
    
    if(command != Command::LOGIN && !controller->isLoggedIn(request))
    {
        controller->sendErrorResponse(request);
        return;
    }

    
    switch (command)
    {
    case Command::LOGIN:
        controller->loginUser(request, LoginMessage::fromLines(lines));
        break;
    case Command::SEND:
        controller->receiveMessage(request);
        break;
    case Command::LIST:
        controller->listMessages(request);
        break;
    case Command::READ:
        controller->readMessage(request);
        break;
    case Command::DEL:
        controller->deleteMessage(request);
        break;
    default:
        break;
    }
};