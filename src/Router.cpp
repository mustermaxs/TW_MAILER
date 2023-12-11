#include "headers/Router.h"


void Router::mapRequestToController(int socketId, std::string buffer)
{
    std::string commandStr = buffer.substr(0, buffer.find("\n"));

    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss(buffer);

    
    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }

    Message message;
    message.fromFile(lines);

    Command command = Utils::mapStringToCommand(commandStr);

    Request request = Request(command, socketId, message);

    Controller controller = Controller();
    
    switch (command)
    {
    case Command::SEND:
        controller.receiveMessage(request);
        break;
    case Command::LIST:
        controller.listMessages(request);
        break;
    // case Command::READ:
    //     controller.readMessage(request);
    //     break;
    // case Command::DEL:
    //     controller.deleteMessage(request);
    //     break;


    default:
        break;
    }
};