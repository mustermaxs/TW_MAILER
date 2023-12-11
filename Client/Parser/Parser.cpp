#include "./headers/Parser.h"


/**
 * Helper function to check if provided string is
 * convertible to integer.
 * e.g. to get the index for the delete-command.
 * @returns true if string is convertible to int, otherwise
 * throws an invalid-argument exception.
 */
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
 * Used to parse client input
 * delegates parsing to designated parsing method
 * depending on the set TW Mailer command
 * 
 * @param input the most recently string that was
 * acquired from the clients console
 * 
 * @param continueReadline is a reference used to signal
 * the executing while-loop in the clients main method when
 * its supposed to stop reading input from the command line
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



/**
 * Sets the TW Mailer command (READ, LIST, DELETE, SEND)
 * so that the parser knows which parsing method to call
 * and process the provided strings from the clients console.
 * Gets transformed to an enum.
 */
void Parser::setMode(std::string mode)
{
    if (this->commandMap.find(mode) == this->commandMap.end())
    {
        throw new std::invalid_argument("Command mode unknown");
    }

    this->mode = this->commandMap[mode];
};



/**
 * Returns the complete parsed and concatenated string.
 */
std::string Parser::getString()
{
    return this->messageStrings;
};




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