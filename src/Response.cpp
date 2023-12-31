#include "headers/Response.h"

void Response::normal(int socketId, std::string resBody)
{
    try
    {
        // BUG exception.what() => Send answer failed: Socket operation on non-socket
        if (send(socketId, resBody.c_str(), resBody.size(), 0) == -1)
        {
            perror("Send answer failed");
            throw std::runtime_error("Send answer failed");
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << "Exception while sending Response: " << ex.what() << std::endl;
    }
};
