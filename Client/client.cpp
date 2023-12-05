#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF 1024
#define PORT 6543

int main(int argc, char **argv) {
    int create_socket;
    char buffer[BUF];
    struct sockaddr_in address;
    int size;

    // CREATE A SOCKET
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    // INIT ADDRESS
    memset(&address, 0, sizeof(address)); // init storage with 0
    address.sin_family = AF_INET;         // IPv4
    address.sin_port = htons(PORT);

    if (argc < 2) {
        inet_aton("127.0.0.1", &address.sin_addr);
    } else {
        inet_aton(argv[1], &address.sin_addr);
    }

    // CREATE A CONNECTION
    if (connect(create_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }

    std::cout << "Connection with server (" << inet_ntoa(address.sin_addr) << ") established\n";

    // RECEIVE DATA
    size = recv(create_socket, buffer, BUF - 1, 0);
    if (size == -1) {
        perror("recv error");
        return EXIT_FAILURE;
    } else if (size == 0) {
        std::cout << "Server closed remote socket\n";
    } else {
        buffer[size] = '\0';
        std::cout << buffer;
    }

    try {
        std::string input;
        while (true) {
            std::cout << ">> ";
            std::getline(std::cin, input);

            if (input == "quit") {
                break;
            }

            // SEND DATA
            if (send(create_socket, input.c_str(), input.length(), 0) == -1) {
                throw std::runtime_error("Send error");
            }

            // RECEIVE FEEDBACK
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size == -1) {
                throw std::runtime_error("Receive error");
            } else if (size == 0) {
                std::cout << "Server closed remote socket\n";
                break;
            } else {
                buffer[size] = '\0';
                std::cout << "<< " << buffer << "\n";
                if (std::string(buffer) != "OK") {
                    throw std::runtime_error("Server error occurred, abort");
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // CLOSES THE DESCRIPTOR
    if (create_socket != -1) {
        if (shutdown(create_socket, SHUT_RDWR) == -1) {
            perror("shutdown create_socket");
        }
        if (close(create_socket) == -1) {
            perror("close create_socket");
        }
    }

    return EXIT_SUCCESS;
}
