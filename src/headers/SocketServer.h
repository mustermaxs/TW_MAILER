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
#include <mutex>
#include <thread>
#include <map>

#include "ISocketHandler.h"
#include "IFileHandler.h"
#include "FileHandler.h"
#include "ConnectionConfig.h"

class SocketServer : public ISocketHandler
{
private:
    const std::string filePathUsageTxt = "/home/mustermax/vscode_projects/TW_MAILER/configs/usage.txt";
    const std::string filePathWelcomeTxt = "/home/mustermax/vscode_projects/TW_MAILER/configs/welcomeMsg.txt";
    const std::string filePathPortNbr = "/home/mustermax/vscode_projects/TW_MAILER/configs/portConfig.txt";
    const std::string filePathWaitForConnTxt = "/home/mustermax/vscode_projects/TW_MAILER/configs/waitForConn.txt";

    std::string usageText;
    std::string waitForConnTxt;
    std::string welcomeMsg;

    std::string msgDirPath = "../../messages/";
    int nbrOfClientsBeforeQueued = 10;
    struct sockaddr_in serverAddress;

    std::map<int, struct sockaddr_in> clientAddresses;
    std::vector<int> clientSocketIds;
    std::mutex clientSocketsMutex;

    bool abortRequested = false;
    void loadMessageTxts();

public:
    SocketServer();

    bool init();
    bool createSocket() override;
    bool initAddress();
    bool makeConnection() override;
    bool bindSocket();
    bool startListening();
    void stopServer();
    int acceptConnectionAndGetSocketId();

    void setNbrClientsBeforeQueued(int);
    void setSpoolDir(std::string);
    void setAbortRequested(bool);
    void setPort(int);
    bool shouldAbortRequest();

    void sendHelpMessage(int socketId);
    void printUsage();

    std::vector<int> getClientSocketIds();
    void checkIfSetupComplete();
    std::string getClientIpBySocketId(int socketId);

};

#endif