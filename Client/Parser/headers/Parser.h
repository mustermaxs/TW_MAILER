#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <exception>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <map>

#include "../../../src/headers/Commands.h"
#include "../../../src/headers/Utils.h"


class Parser
{
    std::map<std::string, Command> commandMap;
    std::string messageStrings;
    int lineNumber = 0;
    Command mode;
    Parser *parseListCommand(std::string line, bool &continueReadline);
    Parser *parseReadCommand(std::string line, bool &continueReadline);
    Parser *parseDeleteCommand(std::string line, bool &continueReadline);
    Parser *parseSendCommand(std::string line, bool &continueReadline);

protected:

public:
    Parser(std::map<std::string, Command> commandMap);
    void reset();
    Parser *parse(const std::string line, bool &continueReadline);
    std::string getString();
    void setMode(std::string mode);
    // bool isModeSet() const;
};

#endif