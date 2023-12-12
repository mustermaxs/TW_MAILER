#ifndef UTILS_H
#define UTILS_H

#include <exception>
#include <string>
#include <map>
#include <vector>

#include "Commands.h"
#include "Color.h"

class Utils
{
public:
    Utils(){};
    static enum Command mapStringToCommand(const std::string commandStr);
    static bool isConvertibleToInt(const std::string &str);

};

#endif