#include "../src/headers/SocketClient.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

SocketClient::SocketClient(std::string IP, int Port)
    : ip(IP), port(Port){};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


void SocketClient::closeConnection()
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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

std::string SocketClient::receiveData()
{
    try
    {
        char buffer[BUF];

        int size = recv(this->socketId, buffer, BUF - 1, 0);

        if (size == -1)
        {
            throw new std::runtime_error("Failed to receive message.");
        }
        else if (size == 0)
        {
            std::cout << "Server closed remote socket\n";

            return "";
        }
        else
        {
            buffer[size] = '\0';

            return buffer;
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

bool SocketClient::createSocket()
{
    try
    {
        if ((this->socketId = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            throw new std::runtime_error("Failed to create socket.");
        }

        return true;
    }
    catch (...)
    {
        std::cout << "Failed to create socket" << std::endl;

        return false;
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool SocketClient::makeConnection()
{
    try
    {
        memset(&this->address, 0, sizeof(this->address));
        this->address.sin_family = AF_INET;
        this->address.sin_port = htons(port);
        inet_aton(IPADDRESS, &address.sin_addr);

        if (connect(socketId, (struct sockaddr *)&address, sizeof(address)) == -1)
        {
            throw new std::runtime_error("Failed to connect to server.");
        }

        std::cout << "Connection with server (" << inet_ntoa(address.sin_addr) << ") established\n";

        return true;
    }
    catch (...)
    {
        std::cout << "Failed to connect to server" << std::endl;

        return false;
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool SocketClient::sendData(std::string data)
{
    try
    {
        char buffer[BUF];
        int size = 0;

        if (size = send(this->socketId, data.c_str(), data.length(), 0) == -1)
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

int SocketClient::getSocketId() { return this->socketId; };