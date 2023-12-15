#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "../src/headers/Router.h"
#include "../src/headers/ConnectionConfig.h"
#include "../src/headers/RecursiveFileHandler.h"
#include "../src/headers/SocketServer.h"

#define BUF 4096
#define PORT 6543

int abortRequested = 0;
int create_socket = -1;
int new_socket = -1;

void signalHandler(int sig);
bool sendWelcomeMessage(int socketId);

SocketServer* server = new SocketServer();

int main(int argc, char **argv)
{

    if (signal(SIGINT, signalHandler) == SIG_ERR)
    {
        std::cerr << "Signal can not be registered\n";
        return EXIT_FAILURE;
    }

    if (argc != 3)
    {
        std::cout << "Usage: ./twmailer-server <port> <mail-spool-directoryname>\n";
        return EXIT_FAILURE;
    }

    std::string spoolDir = argv[2];
    int port = atoi(argv[1]);
    server->setPort(port);

    server->init();
    Router router = Router();

    while (!server->shouldAbortRequest())
    {

        int clientSocketId = server->acceptConnectionAndGetSocketId();
        std::string buffer;
        int size;

        do
        {
            buffer = server->receiveData(clientSocketId);
            router.mapRequestToController(new_socket, buffer);

        } while (buffer != "quit" && !server->shouldAbortRequest());

        if (clientSocketId == -1)
        {
            server->stopServer();
        }
    }

    server->stopServer();

    return EXIT_SUCCESS;
}

void signalHandler(int sig)
{
    if (sig == SIGINT)
    {
        int serverSocketId = server->getSocketId();
        std::cout << "Abort requested...\n";
        abortRequested = 1;

        // Shutdown and close sockets if necessary
        // TODO in eigene Servermethode auslagern
        for (const int clientSocketId : server->getClientSocketIds())
        {
            if (clientSocketId != -1)
            {
                if (shutdown(clientSocketId, SHUT_RDWR) == -1)
                {
                    perror("Shutdown new_socket");
                }
                if (close(clientSocketId) == -1)
                {
                    perror("Close new_socket");
                }

                new_socket = -1;
            }
        }

        if (serverSocketId!= -1)
        {
            if (shutdown(serverSocketId, SHUT_RDWR) == -1)
            {
                perror("Shutdown create_socket");
            }
            if (close(serverSocketId) == -1)
            {
                perror("Close create_socket");
            }
        }
    }
    else
    {
        exit(sig);
    }
}

bool sendWelcomeMessage(int socketId)
{
    IFileHandler *fileHandler = new FileHandler();
    std::string welcomeMessage = fileHandler->readFile("../Server/hellomsg.txt");

    delete fileHandler;

    if (send(new_socket, welcomeMessage.c_str(), welcomeMessage.length(), 0) == -1)
    {
        perror("Send failed");
        return false;
    }
    return true;
}