#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <vector>
#include <string>
#include <iostream>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "ISocketHandler.h"
#include "IFileHandler.h"

class SocketServer : public ISocketHandler
{
private:
    const std::string filePathUsageTxt = "./usage.txt";
    const std::string filePathWelcomeTxt = "./welcomeMsg.txt";
    const std::string filePathPortNbr = "./portConfig.txt";
    const std::string filePathWaitForConnTxt = "waitForConn.txt";

    std::string usageText;
    std::string waitForConnTxt;
    std::string welcomeMsg;

    int nbrOfClientsBeforeQueued = 10;
    struct sockaddr_in serverAddress;

    std::vector<struct sockaddr_in> clientAddresses;
    std::vector<int> clientSocketIds;
    bool abortRequested = false;
    void loadMessageTxts();

public:
    SocketServer();

    bool init();
    bool bindSocket();
    void setNbrClientsBeforeQueued(int);

    void setPort(int);
    
    void sendHelpMessage(int socketId);
    bool initAddress();
    bool createSocket() override;

    std::vector<int> getClientSocketIds();

    bool makeConnection() override;
    void startServer();
    bool startListening();
    void stopServer();

    int acceptConnectionAndGetSocketId();

    // TODO
    // void handleRequest(int);
    void checkIfSetupComplete();

    void printUsage();
    bool shouldAbortRequest();
};

#endif