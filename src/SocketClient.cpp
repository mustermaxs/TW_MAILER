#include "../src/headers/SocketClient.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

SocketClient::SocketClient(std::string IP, int Port)
    : ip(IP), port(Port){};


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
        inet_aton(this->ip.c_str(), &address.sin_addr);

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
        int size = 0;

        if ((size = send(this->socketId, data.c_str(), data.length(), 0)) == -1)
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