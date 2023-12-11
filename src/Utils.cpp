#include "headers/Utils.h"


enum Command Utils::mapStringToCommand(const std::string commandStr)
{
    std::map<std::string, Command> commandMap = getCommandMap();

    if (commandMap.find(commandStr) == commandMap.end())
        {
            throw new std::invalid_argument("Command unknown");
        }

    
    return Command::LIST;
};


/**
 * Helper function to check if provided string is
 * convertible to integer.
 * e.g. to get the index for the delete-command.
 * @returns true if string is convertible to int, otherwise
 * throws an invalid-argument exception.
 */
bool Utils::isConvertibleToInt(const std::string &str)
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