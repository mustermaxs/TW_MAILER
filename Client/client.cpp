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
#define DECO_LINE std::cout << "\n---------------------------------"  << std::endl;

int main(int argc, char **argv)
{
    int create_socket;
    char buffer[BUF];
    struct sockaddr_in address;
    int size;

    // CREATE A SOCKET
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    // INIT ADDRESS
    memset(&address, 0, sizeof(address)); // init storage with 0
    address.sin_family = AF_INET;         // IPv4

    /***************************************************************************/

    // Check if IP is provided, otherwise use localhost
    if (argc < 2)
    {
        inet_aton("127.0.0.1", &address.sin_addr);
        ConnectionConfig* config = ConnectionConfig::getInstance();
        config->setPort(std::stoi(argv[2]));
        config->setIP(std::string(argv[1]));
    }
    else
    {
        inet_aton(argv[1], &address.sin_addr);
    }
    
    // Check if both IP and port are provided
    if (argc < 3 || argv[1] == "help")
    {
        std::cerr << "Usage: ./twmailer-client <ip> <port>\n";
        return EXIT_FAILURE;
    }

    // Convert the port from string to integer
    int port = atoi(argv[2]);

    // Use the provided port instead of the PORT macro
    address.sin_port = htons(port);

    // CREATE A CONNECTION
    if (connect(create_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }

    std::cout << "Connection with server (" << inet_ntoa(address.sin_addr) << ") established\n";

    // RECEIVE DATA
    size = recv(create_socket, buffer, BUF - 1, 0);
    if (size == -1)
    {
        perror("recv error");
        return EXIT_FAILURE;
    }
    else if (size == 0)
    {
        std::cout << "Server closed remote socket\n";
    }
    else
    {
        buffer[size] = '\0';
        std::cout << buffer;
    }

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
            if (input == "quit" || input == "QUIT") break;

            // SEND DATA
            if (send(create_socket, input.c_str(), input.length(), 0) == -1)
            {
                throw std::runtime_error("Send error");
            }

            // RECEIVE FEEDBACK
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size == -1)
            {
                throw std::runtime_error("Receive error");
            }
            else if (size == 0)
            {
                std::cout << "Server closed remote socket\n";
                // break;
            }
            else
            {
                buffer[size] = '\0';
                // std::cout << "<< " << buffer << "\n";
                std::cout << buffer << "\n";

            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // CLOSES THE DESCRIPTOR
    if (create_socket != -1)
    {
        if (shutdown(create_socket, SHUT_RDWR) == -1)
        {
            perror("shutdown create_socket");
        }
        if (close(create_socket) == -1)
        {
            perror("close create_socket");
        }
    }

    return EXIT_SUCCESS;
}
