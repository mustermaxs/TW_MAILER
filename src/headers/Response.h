#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <exception>


class Response
{
private:
    Response(){};
    ~Response(){};

public:
    static void normal(int, std::string);
};

#endif