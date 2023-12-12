#include "./headers/Parser.h"

Parser::Parser()
{
    this->mode = NOT_SET;
    this->continueReadline = true;
    this->lineNumber = 0;
};

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
    if (input == "quit")
    {
        this->continueReadline = false;
        this->messageStrings += input;

        return this;
    }

    if (input == "LIST" || input == "READ" || input == "DEL" || input == "SEND")
    {
        this->setMode(input);
        this->reset();
    }

    if (this->mode == NOT_SET)
    {
        Color::Modifier red(Color::FG_RED);
        Color::Modifier resetColor(Color::FG_DEFAULT);
        //TODO print usage
        std::cout << red << "Invalid command.\n" << std::endl;
        
        return this;
    }

    switch (this->mode)
    {
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

    return this;
};

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

/**
 * Returns the complete parsed and concatenated string.
 */
std::string Parser::getString()
{
    return this->messageStrings;
};

Parser *Parser::parseReadCommand(std::string line)
{
    try
    {

        std::string header = "";
        std::vector<std::string> headers = {"SENDER", "ID"};

        this->continueReadline = lineNumber < 2;

        if (lineNumber == 2 && !Utils::isConvertibleToInt(line))
        {
            Color::Modifier red(Color::FG_RED);
            Color::Modifier resetColor(Color::FG_DEFAULT);

            std::cout << red << "Invalid argument provided. Expects a number" << std::endl;
            std::cout << red << "Please enter a valid number" << resetColor << std::endl;

            this->reset();
            return this;
        }

        header = (lineNumber > 0 && lineNumber < headers.size() + 1)
                     ? headers[lineNumber - 1] + ":"
                     : "";

        this->messageStrings = this->messageStrings + header + line + "\n";
        this->lineNumber++;

        return this;
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
    };
}

Parser *Parser::parseListCommand(std::string line)
{

    std::string header = "";
    std::vector<std::string> headers = {"SENDER"};

    this->continueReadline = lineNumber < 1;

    header = (lineNumber > 0 && lineNumber < headers.size() + 1)
                 ? headers[lineNumber - 1] + ":"
                 : "";

    this->messageStrings = this->messageStrings + header + line + "\n";
    this->lineNumber++;

    return this;
};

Parser *Parser::parseDeleteCommand(std::string line)
{
    try
    {

        std::string header = "";
        std::vector<std::string> headers = {"SENDER", "ID"};

        this->continueReadline = lineNumber < 2;

        if (lineNumber == 2 && !Utils::isConvertibleToInt(line))
        {
            Color::Modifier red(Color::FG_RED);
            Color::Modifier resetColor(Color::FG_DEFAULT);

            std::cout << red << "Invalid argument provided. Expects a number" << std::endl;
            std::cout << red << "Please enter a valid number" << resetColor << std::endl;

            this->reset();
            return this;
        }

        header = (lineNumber > 0 && lineNumber < headers.size() + 1)
                     ? headers[lineNumber - 1] + ":"
                     : "";

        this->messageStrings = this->messageStrings + header + line + "\n";
        this->lineNumber++;

        return this;
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
    }
};

Parser *Parser::parseSendCommand(std::string line)
{
    std::string header = "";
    std::vector<std::string> headers = {"SENDER", "RECEIVER", "SUBJECT", "MESSAGE"};

    if (line == ".")
    {
        this->continueReadline = false;
        this->messageStrings += line + "\n";

        return this;
    }

    header = (lineNumber > 0 && lineNumber < headers.size() + 1)
                 ? headers[lineNumber - 1] + ":"
                 : "";

    this->messageStrings = this->messageStrings + header + line + "\n";
    this->lineNumber++;

    return this;
};
