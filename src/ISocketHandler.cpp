#include "../src/headers/ISocketHandler.h"

ISocketHandler::~ISocketHandler(){};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Used to shutdown and close socket.
/// @param socketId int - ID of socket thats supposed to be closed.
void ISocketHandler::closeConnection(int socketId)
{
    if (shutdown(socketId, SHUT_RDWR) == -1)
    {
        perror("shutdown create_socket");
    }
    if (close(socketId) == -1)
    {
        perror("close create_socket");
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Returns socket id.
/// @return int - socket id.
int ISocketHandler::getSocketId() { return this->socketId; };

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Used to receive data from another socket.
/// @param socketId int - socket id.
/// @return string - Received data.
std::string ISocketHandler::receiveData(int socketId)
{
    try
    {
        char buffer[BUF];

        int size = recv(socketId, buffer, BUF - 1, 0);

        if (size == -1)
        {
            throw new std::runtime_error("Failed to receive message.");
            
            return "";
        }
        else if (size == 0)
        {
            std::cout << "Server closed remote socket\n";

            return "";
        }
        else
        {
            buffer[size] = '\0';

            return (std::string)buffer;
        }
    }
    catch (...)
    {
        std::cout << "Failed to receive message" << std::endl;

        return "";
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Sends data to socket with specified id.
/// @param data string - data to be sent.
/// @param socketId int - socket id.
/// @return bool - true if successfull
bool ISocketHandler::sendData(std::string data, int socketId)
{
    try
    {
        int size = 0;

        if ((size = send(socketId, data.c_str(), data.length(), 0)) == -1)
        {
            throw std::runtime_error("Send error.");

            return false;
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
};