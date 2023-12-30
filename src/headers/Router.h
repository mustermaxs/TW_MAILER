#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <vector>
#include <iostream>

#include "Controller.h"
#include "Request.h"
#include "Utils.h"
#include "Message.h"
#include "Controller.h"

class Router
{
private:
    Controller *controller;

public:
    Router(Controller *);
    ~Router();
    void init();
    void mapRequestToController(int, std::string);
};

#endif