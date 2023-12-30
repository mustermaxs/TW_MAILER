#include "./headers/Parser.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Parses the clients input. Collects every passed input line and
/// tries to concatenate them to a structured message that can be deserialized
// by the server to a message object.
Parser::Parser()
{
    this->mode = NOT_SET;
    this->continueReadline = true;
    this->lineNumber = 0;

    this->headers =
        {
            {Command::LOGIN, {"USERNAME", "PASSWORD"}},
            {Command::LIST, {}},
            {Command::READ, {"ID"}},
            {Command::DEL, {"ID"}},
            {Command::SEND, {"RECEIVER", "SUBJECT", "MESSAGE"}},
        };
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/**
 * Used to parse client input
 * delegates parsing to designated parsing method
 * depending on the set TW Mailer command
 *
 * @param input the most recently string that was
 * acquired from the clients console
 */
Parser *Parser::parse(const std::string input)
{
    if (input == "quit" || input == "QUIT")
    {
        this->continueReadline = false;
        this->messageStrings += input;

        return this;
    }

    if (input == "LIST" || input == "READ" || input == "DEL" || input == "SEND" || input == "LOGIN")
    {
        this->setMode(input);
        this->reset();
    }

    if (this->mode == NOT_SET)
    {
        // TODO print usage
        std::cout << Color::Mod::getString(Color::FG_RED, "Invalid command.\n") << std::endl;

        return this;
    }

    return this->callDesignatedParser(input);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Calls one of the specialiced parser methods to
/// parse the client input. Collects every passed input line and
/// tries to concatenate them to a structured message that can be deserialized
// by the server to a message object.
/// @param input String - single input line
/// @return Parser pointer.
Parser *Parser::callDesignatedParser(std::string input)
{
    switch (this->mode)
    {
    case Command::LOGIN:
        return this->parseLoginCommand(input);
        break;
    case Command::LIST:
        return this->parseListCommand(input);
        break;
    case Command::READ:
        return this->parseReadCommand(input);
        break;
    case Command::DEL:
        return this->parseDeleteCommand(input);
        break;
    case Command::SEND:
        return this->parseSendCommand(input);
        break;
    default:
        throw new std::invalid_argument("Parsing mode needs to be set in advance.");
        break;
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/**
 * @brief:Resets the parser to its initial state.
 * Used when the client sends a new command.
 * e.g. when the client sends a new READ command
 **/
void Parser::reset()
{
    this->messageStrings = "";
    this->lineNumber = 0;
    this->continueReadline = true;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/**
 * Sets the TW Mailer command (READ, LIST, DEL, SEND)
 * so that the parser knows which parsing method to call
 * and process the provided strings from the clients console.
 * Gets transformed to an enum.
 */
void Parser::setMode(std::string mode)
{
    this->mode = Utils::mapStringToCommand(mode);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/**
 * @brief Returns the complete parsed and concatenated string.
 * @return string - the concatenated parsed string with structure elements.
 * (e.g. SENDER, RECEIVER, SUBJJECT, CONTENT)
 */
std::string Parser::getString()
{
    return this->messageStrings;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Parser::printInvalidNumberProvided()
{
    std::cout << Color::Mod::getString(Color::FG_RED, "Invalid argument provided. Expects a number.") << std::endl;
    std::cout << Color::Mod::getString(Color::FG_RED, "Please enter a valid number.") << std::endl;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Parser *Parser::parseLoginCommand(std::string line)
{
    std::string header = "";

    this->continueReadline = lineNumber < 2;

    this->buildCommandString(line, this->headers[Command::LOGIN]);

    return this;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


Parser *Parser::parseReadCommand(std::string line)
{
    std::string header = "";

    this->continueReadline = lineNumber < 1;

    if (lineNumber == 2 && !Utils::isConvertibleToInt(line))
    {
        this->printInvalidNumberProvided();
        this->reset();

        return this;
    }

    this->buildCommandString(line, this->headers[Command::READ]);

    return this;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Parser *Parser::parseListCommand(std::string line)
{

    this->continueReadline = false;

    this->buildCommandString(line, this->headers[Command::LIST]);

    return this;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Parser *Parser::parseDeleteCommand(std::string line)
{
    this->continueReadline = lineNumber < 1;

    if (lineNumber == 1 && !Utils::isConvertibleToInt(line))
    {
        this->printInvalidNumberProvided();
        this->reset();

        return this;
    }

    this->buildCommandString(line, this->headers[Command::DEL]);

    return this;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Parser *Parser::parseSendCommand(std::string line)
{
    std::string header = "";

    if (line == ".")
    {
        this->continueReadline = false;
        //TODO reset aufrufen?
        return this;
    }

    this->buildCommandString(line, this->headers[Command::SEND]);

    return this;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Adds headers (message structure elements) to message elements that make up a basic structure
/// so that parsing the mssage becomes easier.
/// @param line String - the line to process.
/// @param headers vector<strimgs> - the expected headers (message structure elements).
void Parser::buildCommandString(std::string &line, const std::vector<std::string> &headers)
{
    std::string header = "";
    int headerCount = headers.size();

    header = this->getCurrentHeader();

    if ((header == "SENDER:" || header == "RECEIVER:") && line.length() > 8)
    {
        this->reset();
        std::cout << Color::Mod::getString(Color::FG_RED, "Username should be max. 8 characters long.") << std::endl;

        return;
    }

    this->lineNumber++;
    this->messageStrings = this->messageStrings + header + line + "\n";
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

std::string Parser::getCurrentHeader()
{
    if (this->mode == Command::NOT_SET)
        return "";

    return (this->lineNumber > 0 && this->lineNumber < this->headers[this->mode].size() + 1)
               ? this->headers[this->mode][this->lineNumber - 1] + ":"
               : "";
};