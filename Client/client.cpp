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
#include "../src/headers/ISocketHandler.h"
#include "../src/headers/SocketClient.h"

#define DECO_LINE std::cout << "\n---------------------------------" << std::endl;
#define PRINT_USAGE std::cerr << "Usage: ./twmailer-client <ip> <port>\n";


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

    if (!client->createSocket() || !client->makeConnection())
        return EXIT_FAILURE;

    // std::cout << client->receiveData(client->getSocketId()) << std::endl;

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

            client->sendData(input); //? nach "quit", wird resetted empty string noch gesendet?
            std::cout << client->receiveData(client->getSocketId()) << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    if (client->getSocketId() != -1)
    {
        client->closeConnection(client->getSocketId());
    }

    return EXIT_SUCCESS;
}
