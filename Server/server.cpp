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

#define BUF 4096
#define PORT 6543

int abortRequested = 0;
int create_socket = -1;
int new_socket = -1;

void signalHandler(int sig);
bool sendWelcomeMessage(int socketId);


int main(int argc, char **argv) {
    socklen_t addrlen;
    struct sockaddr_in address, cliaddress;
    int reuseValue = 1;

    // Signal Handler
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        std::cerr << "Signal can not be registered\n";
        return EXIT_FAILURE;
    }

    // Create a Socket
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    // Set Socket Options
    if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEADDR, &reuseValue, sizeof(reuseValue)) == -1) {
        perror("Set socket options - reuseAddr");
        return EXIT_FAILURE;
    }

    if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEPORT, &reuseValue, sizeof(reuseValue)) == -1) {
        perror("Set socket options - reusePort");
        return EXIT_FAILURE;
    }

    // Init Address
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;

    if (argc != 3) {
        std::cout << "Usage: ./twmailer-server <port> <mail-spool-directoryname>\n";
        return EXIT_FAILURE;
    }

    std::string spoolDir = argv[2];
    int port = atoi(argv[1]);

    ConnectionConfig* config = ConnectionConfig::getInstance();
    config->setPort(port);
    config->setBaseDirectory(spoolDir);
    address.sin_port = htons(config->getPort());

    


    // Assign an Address with Port to Socket
    if (bind(create_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Bind error");
        return EXIT_FAILURE;
    }

    // Allow Connection Establishing
    if (listen(create_socket, 5) == -1) {
        perror("Listen error");
        return EXIT_FAILURE;
    }

    while (!abortRequested) {
        std::cout << "Waiting for connections...\n";

        // Accepts Connection Setup
        addrlen = sizeof(struct sockaddr_in);
        if ((new_socket = accept(create_socket, (struct sockaddr *)&cliaddress, &addrlen)) == -1) {
            if (abortRequested) {
                perror("Accept error after aborted");
            } else {
                perror("Accept error");
            }
            break;
        }

        // Start Client Communication--
        std::cout << "Client connected from " << inet_ntoa(cliaddress.sin_addr) << ":" << ntohs(cliaddress.sin_port) << "...\n";

        // Client Communication
        char buffer[BUF];
        int size;

        // Send welcome message
        if(sendWelcomeMessage(new_socket) == false)
        continue;


        do {
            size = recv(new_socket, buffer, BUF - 1, 0);
            if (size == -1) {
                perror("Recv error");
                break;
            }

            if (size == 0) {
                std::cout << "Client closed remote socket\n";
                break;
            }


            // init Request
            std::string bufferStr(buffer);
            Router router = Router();
            router.mapRequestToController(new_socket, bufferStr);
            

            buffer[size] = '\0';
            std::cout << "Message received: " << "\n" << buffer << "\n";


        } while (strcmp(buffer, "quit") != 0 && !abortRequested);

        // Close client socket
        if (new_socket != -1) {
            if (shutdown(new_socket, SHUT_RDWR) == -1) {
                perror("Shutdown new_socket");
            }
            if (close(new_socket) == -1) {
                perror("Close new_socket");
            }
            new_socket = -1;
        }
    }

    // Close the server socket
    if (create_socket != -1) {
        if (shutdown(create_socket, SHUT_RDWR) == -1) {
            perror("Shutdown create_socket");
        }
        if (close(create_socket) == -1) {
            perror("Close create_socket");
        }
    }

    return EXIT_SUCCESS;
}

void signalHandler(int sig) {
    if (sig == SIGINT) {
        std::cout << "Abort requested...\n";
        abortRequested = 1;

        // Shutdown and close sockets if necessary
        if (new_socket != -1) {
            if (shutdown(new_socket, SHUT_RDWR) == -1) {
                perror("Shutdown new_socket");
            }
            if (close(new_socket) == -1) {
                perror("Close new_socket");
            }
            new_socket = -1;
        }

        if (create_socket != -1) {
            if (shutdown(create_socket, SHUT_RDWR) == -1) {
                perror("Shutdown create_socket");
            }
            if (close(create_socket) == -1) {
                perror("Close create_socket");
            }
        }
    } else {
        exit(sig);
    }
}

bool sendWelcomeMessage(int socketId)
{
    std::string welcomeMessage = "Welcome to the TW Mailer Server!\r\n";
    
    welcomeMessage +=  "Please enter your commands:\r\n";

    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|------------ Commands: ----------|\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|---------Send a message:---------|\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|  SEND                           |\r\n";
    welcomeMessage += "|  <Sender>                       |\r\n";
    welcomeMessage += "|  <Receiver>                     |\r\n";
    welcomeMessage += "|  <Subject>                      |\r\n";
    welcomeMessage += "|  <Message>                      |\r\n";
    welcomeMessage += "|  .                              |\r\n";
    welcomeMessage += "|  (End the message with '.\\n')   |\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|--------List all messages:-------|\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|  LIST                           |\r\n";
    welcomeMessage += "|  <Username>                     |\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|---------Read a message:---------|\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|  READ                           |\r\n";
    welcomeMessage += "|  <Username>                     |\r\n";
    welcomeMessage += "|  <Message-Number>               |\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|--------Delete a message:--------|\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|  DEL                            |\r\n";
    welcomeMessage += "|  <Username>                     |\r\n";
    welcomeMessage += "|  <Message-Number>               |\r\n";
    welcomeMessage += "|---------------------------------|\r\n";
    welcomeMessage += "|---------Quit the server:--------|\r\n";
    welcomeMessage += "|  QUIT                           |\r\n";
    welcomeMessage += "|---------------------------------|\r\n\r\n";
    welcomeMessage += "| Enter your command:\r\n\r\n";

    if (send(new_socket, welcomeMessage.c_str(), welcomeMessage.length(), 0) == -1) {
        perror("Send failed");
        return false;
    }
    return true;
}