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
    std::map<Command, std::vector<std::string>> headers;
    int lineNumber = 0;
    Command mode;
    Parser *parseLoginCommand(std::string);
    Parser *parseListCommand(std::string);
    Parser *parseReadCommand(std::string);
    Parser *parseDeleteCommand(std::string);
    Parser *parseSendCommand(std::string);
    void printInvalidNumberProvided();
    void buildCommandString(std::string &, const std::vector<std::string> &);
    Parser *callDesignatedParser(std::string);

protected:
public:
    bool continueReadline = true;
    Parser();
    void reset();
    Parser *parse(const std::string);
    std::string getString();
    void setMode(std::string);
    std::string getCurrentHeader();
    // bool isModeSet() const;
};

#endif