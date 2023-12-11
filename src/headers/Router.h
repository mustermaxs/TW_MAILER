#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <vector>
#include <iostream>
#include "Request.h"
#include "Controller.h"

class Router
{
    Request request;

public:
    Router();
    ~Router();
    void init();
    void mapRequestToController(int socketId, std::string content);
};

#endif