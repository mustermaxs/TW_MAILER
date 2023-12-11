#include "./headers/Parser.h"

bool isConvertibleToInt(const std::string &str)
{
    try
    {
        size_t pos;
        std::stoi(str, &pos);
        return pos == str.length();
    }
    catch (const std::invalid_argument &)
    {
        return false;
    }
    catch (const std::out_of_range &)
    {
        return false;
    }
};

Parser::Parser()
{
    this->commandMap =
        {
            {"LIST", Command::LIST},
            {"READ", Command::READ},
            {"DELETE", Command::DELETE},
            {"SEND", Command::SEND}};
};

/**
 * @class Parser
 * @brief Parses user input and performs corresponding actions.
 */
Parser *Parser::parse(const std::string input, bool &continueReadline)
{
    if (input == "quit")
    {
        continueReadline = false;

        return this;
    }

    switch (this->mode)
    {
    case Command::LIST:
        return this->parseListCommand(input, continueReadline);
        break;
    case Command::READ:
        return this->parseReadCommand(input, continueReadline);
        break;
    case Command::DELETE:
        return this->parseDeleteCommand(input, continueReadline);
        break;
    case Command::SEND:
        return this->parseSendCommand(input, continueReadline);
        break;
    default:
        throw new std::invalid_argument("Parsing mode needs to be set in advance.");
        break;
    }

    return this;
};

void Parser::setMode(std::string mode)
{
    if (this->commandMap.find(mode) == this->commandMap.end())
    {
        throw new std::invalid_argument("Command mode unknown");
    }

    this->mode = this->commandMap[mode];
};

std::string Parser::getString()
{
    return this->messageStrings;
};

/*
READ\n
<Username>\n
<Message-Number>\n
*/
Parser *Parser::parseReadCommand(std::string line, bool &continueReadline)
{
    if (lineNumber > 2)
    {
        continueReadline = false;
        lineNumber = 0;

        return this;
    }

    if (lineNumber == 2 && !isConvertibleToInt(line))
    {
        throw new std::invalid_argument("Should be number");
    }

    this->messageStrings += line + "\n";
    this->lineNumber++;

    return this;
};

/*
LIST\n
<Username>\n
*/
Parser *Parser::parseListCommand(std::string line, bool &continueReadline)
{
    if (lineNumber > 1)
    {
        continueReadline = false;

        return this;
    }
    this->messageStrings += line + "\n";
    this->lineNumber++;

    return this;
};

/*
DEL\n
<Username>\n
<Message-Number>\n
*/
Parser *Parser::parseDeleteCommand(std::string line, bool &continueReadline)
{
    if (lineNumber > 2)
    {
        continueReadline = false;

        return this;
    }
    if (lineNumber == 2 && !isConvertibleToInt(line))
    {
        throw new std::invalid_argument("Should be number");
    }

    this->messageStrings += line + "\n";
    this->lineNumber++;

    return this;
};

/*
SEND\n
<Sender>\n
<Receiver>\n
<Subject (max. 80 chars)>\n
<message (multi-line; no length restrictions)\n>
.\n
*/
Parser *Parser::parseSendCommand(std::string line, bool &continueReadline)
{
    if (line == ".")
    {
        continueReadline = false;
        this->messageStrings += line + "\n";

        return this;
    }

    this->messageStrings += line + "\n";

    return this;
};