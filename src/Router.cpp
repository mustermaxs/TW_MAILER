#include "Router.h"

Router::Router() {};

void Router::mapRequestToController(int socketId, std::string buffer)
{
    std::string command = buffer.substr(0, buffer.find("\n"));

    
    

    // this->request = new Request(command, socketId, buffer);


};