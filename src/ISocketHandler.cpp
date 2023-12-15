#include "../src/headers/ISocketHandler.h"

ISocketHandler::~ISocketHandler(){};

void ISocketHandler::closeConnection(int socketId)
{
    if (shutdown(this->socketId, SHUT_RDWR) == -1)
    {
        perror("shutdown create_socket");
    }
    if (close(this->socketId) == -1)
    {
        perror("close create_socket");
    }
};

int ISocketHandler::getSocketId() { return this->socketId; };

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

bool ISocketHandler::sendData(std::string data, int socketId)
{
    try
    {
        char buffer[BUF];
        int size = 0;

        if (size = send(socketId, data.c_str(), data.length(), 0) == -1)
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