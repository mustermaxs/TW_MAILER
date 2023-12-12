#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <exception>
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
    std::string messageStrings;
    int lineNumber = 0;
    Command mode;
    Parser *parseListCommand(std::string line);
    Parser *parseReadCommand(std::string line);
    Parser *parseDeleteCommand(std::string line);
    Parser *parseSendCommand(std::string line);

protected:

public:
    bool continueReadline = true;
    Parser();
    void reset();
    Parser *parse(const std::string line);
    std::string getString();
    void setMode(std::string mode);
    // bool isModeSet() const;
};

#endif