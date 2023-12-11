#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

class CommandParser
{
private:
    bool continueRead;

public:
    CommandParser();
    ~CommandParser();
    void read();
};

#endif
/*
__client__

    IParser parser;
string firstLine = getline() // ist der command
    if (firstline == "LIST") parser.setMode("LIST");

    std::string lineMessage;
while (continueRead)
{

    string line = getline(std::cin, lineMessage);
    parser.parse(line, &continueRead);
}

lineMessage = parser.getRequest();


CommandParser::read()
{
    std::string lineMessage;
    while (continueRead)
    {
        string line = getline(std::cin, lineMessage);
        this.parse(line, &continueRead);
    }
}

CommandParser::parse(string line, bool &continueRead)
{
    // if command done
    continueRead = false;
}


__parser__

*/