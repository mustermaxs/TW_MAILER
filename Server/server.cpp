#include <iostream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <mutex>

#include "../src/headers/Router.h"
#include "../src/headers/ConnectionConfig.h"
#include "../src/headers/FileHandler.h"
#include "../src/headers/SocketServer.h"

/*
CLIENT:
The client functionality is implemented using the SocketClient which inherits from and Parser classes.
SocketClient manages the underlying TCP/IP socket operations, including socket creation, establishing connections with the server,
and handling data transmission. The Parser class interprets and structures the user's input, supporting commands like SEND, LIST, READ, DEL, and QUIT


SERVER:
The server is responsible for handling incoming connections, managing client sessions, and routing requests to the appropriate controllers.
The server uses a SocketServer class, which encapsulates the logic for creating a socket, initializing the address, binding the socket,
and starting to listen for incoming connections.
The router is responsible for routing it to the appropriate controller method (currently there is a single controller taking care of
all the operations).
The Controller itself is responsible for sending the response back to the client.
The Controller class uses the MessageHandler (leveraging the FileHandler) class to handle all the filesystem operations relevant to Messages.

*/

#define BUF 4096

int abortRequested = 0;
int create_socket = -1;
int new_socket = -1;
int ldapVersion = LDAP_VERSION3;

bool LDAPauthenticate(std::string username, std::string password);
void signalHandler(int sig);
bool sendWelcomeMessage(int socketId);
void handleClient(int clientSocketId);

SocketServer *server = new SocketServer();
std::map<int, std::thread> threads;

int main(int argc, char **argv)
{
    try
    {
        if (signal(SIGINT, signalHandler) == SIG_ERR)
        {
            std::cerr << "Signal can not be registered\n";
            return EXIT_FAILURE;
        }

        // check if port and spool directory are given
        if (argc != 3)
        {
            server->printUsage();
            return EXIT_FAILURE;
        }

        // check if argv[1] is int
        for (size_t i = 0; i < strlen(argv[1]); i++)
        {
            if (!isdigit(argv[1][i]))
            {
                std::cerr << "Port must be an integer\n";
                server->printUsage();
                return EXIT_FAILURE;
            }
        }

        int port = atoi(argv[1]);
        server->setPort(port);

        std::string spoolDir = argv[2];

        if (!server->init())
        {
            std::cerr << "Server could not be initialized\n";
            return EXIT_FAILURE;
        }

        server->setSpoolDir(spoolDir);

        while (!server->shouldAbortRequest())
        {
            int clientSocketId = server->acceptConnectionAndGetSocketId();
            if (clientSocketId == -1)
                break;

            if (clientSocketId != -1)
            {
                threads[clientSocketId] = std::thread(handleClient, clientSocketId);
                threads[clientSocketId].detach();
            }
        }

        return EXIT_SUCCESS;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void handleClient(int clientSocketId)
{

    Controller *controller = new Controller();
    Router router = Router(controller);

    std::string buffer;

    while ((buffer != "quit" || buffer != "QUIT") && !server->shouldAbortRequest())
    {
        buffer = server->receiveData(clientSocketId);

        if (buffer.size() == 0)
            break;

        router.mapRequestToController(clientSocketId, buffer, server->getClientIpBySocketId(clientSocketId));
    }

    if (clientSocketId != -1)
    {
        server->closeConnection(clientSocketId);
        close(clientSocketId);
        // threads.erase(clientSocketId);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void signalHandler(int sig)
{
    if (sig == SIGINT)
    {
        int serverSocketId = server->getSocketId();
        std::cout << "Abort requested, closing server.\n";
        server->setAbortRequested(true);
        // Shutdown and close sockets if necessary

        for (int clientSocketId : server->getClientSocketIds())
        {
            if (clientSocketId != -1)
            {
                if (shutdown(clientSocketId, SHUT_RDWR) == -1)
                {
                    // perror("Shutdown new_socket");
                }
                if (close(clientSocketId) == -1)
                {
                    // perror("Close new_socket");
                }

                threads[clientSocketId].join();

                clientSocketId = -1;
            }
        }

        if (serverSocketId != -1)
        {
            if (shutdown(serverSocketId, SHUT_RDWR) == -1)
            {
                // perror("Shutdown create_socket");
            }
            if (close(serverSocketId) == -1)
            {
                // perror("Close create_socket");
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
        // perror("Send failed");
        
        return false;
    }
    return true;
}
