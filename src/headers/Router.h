#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <vector>
#include <iostream>

#include "Controller.h"
#include "Request.h"
#include "Utils.h"

class Router
{

public:
    Router() {};
    ~Router();
    void init();
    void mapRequestToController(int socketId, std::string content);
};

#endif