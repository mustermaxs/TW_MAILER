#ifndef COMMANDS_H
#define COMMANDS_H

#include <map>
#include <string>
#include <iostream>

enum Command
{
    LIST,
    READ,
    DEL,
    SEND,
    NOT_SET // used as default value until client uses one of the other commands
};

inline std::map<std::string, Command> &getCommandMap()
{
    static std::map<std::string, Command> commandMap = {
        {"LIST", Command::LIST},
        {"READ", Command::READ},
        {"DEL", Command::DEL},
        {"SEND", Command::SEND}};
    return commandMap;
};

#endif