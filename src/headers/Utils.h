#ifndef UTILS_H
#define UTILS_H

#include <exception>
#include <string>
#include <map>
#include <vector>
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
#include <vector>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iomanip>


#include "Commands.h"
#include "Color.h"

class Utils
{
public:
    Utils(){};
    static enum Command mapStringToCommand(const std::string commandStr);
    static bool isConvertibleToInt(const std::string &str);
    static std::string getCurrTimeAsString(std::string);

};

#endif