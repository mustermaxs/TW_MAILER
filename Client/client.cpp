#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "./Parser/headers/Parser.h"

#define BUF 1024
#define PORT 6543

class SocketClient
{
public:
    SocketClient(const char *serverAddress = "127.0.0.1") : serverAddress_(serverAddress) {}

    void connectToServer()
    {
        createSocket();
        initAddress();
        createConnection();
    }

    void sendData(const std::string &data)
    {
        if (send(socket_, data.c_str(), data.length(), 0) == -1)
        {
            throw std::runtime_error("Send error");
        }
    }

    std::string receiveData()
    {
        char buffer[BUF];
        int size = recv(socket_, buffer, BUF - 1, 0);
        if (size == -1)
        {
            throw std::runtime_error("Receive error");
        }
        else if (size == 0)
        {
            return "Server closed remote socket";
        }
        else
        {
            buffer[size] = '\0';
            return buffer;
        }
    }

    void closeConnection()
    {
        if (shutdown(socket_, SHUT_RDWR) == -1)
        {
            perror("shutdown socket");
        }
        if (close(socket_) == -1)
        {
            perror("close socket");
        }
    }

private:
    int socket_;
    struct sockaddr_in address_;
    const char *serverAddress_;

    void createSocket()
    {
        if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Socket error");
            throw std::runtime_error("Socket creation error");
        }
    }

    void initAddress()
    {
        memset(&address_, 0, sizeof(address_));
        address_.sin_family = AF_INET;
        address_.sin_port = htons(PORT);
        inet_aton(serverAddress_, &address_.sin_addr);
    }

    void createConnection()
    {
        if (connect(socket_, (struct sockaddr *)&address_, sizeof(address_)) == -1)
        {
            perror("Connect error - no server available");
            throw std::runtime_error("Connection error");
        }

        std::cout << "Connection with server (" << inet_ntoa(address_.sin_addr) << ") established\n";
    }
};

int main()
{
    try
    {
        SocketClient socketClient;
        socketClient.connectToServer();

        std::string response = socketClient.receiveData();
        std::cout << response << "\n";

        Parser parser;
        std::string input;

        while (true)
        {
            input = "";
            while (parser.continueReadline)
            {
                std::getline(std::cin, input);
                parser.parse(input);
            }

            input = parser.getString();
            parser.reset();

            if (input == "quit")
                break;

            socketClient.sendData(input);

            std::string response = socketClient.receiveData();
            std::cout << response << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
