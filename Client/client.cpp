#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "./Parser/headers/Parser.h"
#include "../src/headers/ConnectionConfig.h"

#define BUF 4096
#define IPADDRESS "127.0.0.1"
#define DECO_LINE std::cout << "\n---------------------------------" << std::endl;
#define PRINT_USAGE std::cerr << "Usage: ./twmailer-client <ip> <port>\n";

class SocketClient
{
private:
    int socketId;
    std::string ip;
    struct sockaddr_in address;
    int port;

public:
    SocketClient(std::string, int);
    bool connectToServer();
    bool sendData(const std::string data);
    std::string receiveData();
    void closeConnection();
    bool createSocket();
    int getSocketId() { return this->socketId; };
};

SocketClient::SocketClient(std::string ip, int port)
    : ip(ip), port(port), socketId(socketId){};

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

bool SocketClient::connectToServer()
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

int main(int argc, char **argv)
{
    if (argc < 3 || argv[1] == "help")
    {
        PRINT_USAGE;

        return EXIT_FAILURE;
    }

    std::string ip = (std::string)argv[1];
    int port = std::stoi(argv[2]);
    SocketClient *client = new SocketClient(ip, port);

    if (!client->createSocket() || !client->connectToServer())
        return EXIT_FAILURE;

    std::cout << client->receiveData() << std::endl;

    try
    {
        std::string input;

        while (true)
        {
            input = "";
            Parser *parser = new Parser();

            while (parser->continueReadline)
            {
                std::cout << parser->getCurrentHeader();
                std::getline(std::cin, input);
                parser->parse(input);
            }

            input = parser->getString();
            parser->reset();
            DECO_LINE;

            if (input == "quit" || input == "QUIT")
            {
                break;
            }

            client->sendData(input);
            std::cout << client->receiveData() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // CLOSES THE DESCRIPTOR
    if (client->getSocketId() != -1)
    {
        client->closeConnection();
    }

    return EXIT_SUCCESS;
}
