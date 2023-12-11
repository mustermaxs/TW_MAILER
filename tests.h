#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include "./src/headers/IFileHandler.h"  // Assuming IFileHandler is a declared header
#include "./src/headers/RecursiveFileHandler.h"  // Assuming RecursiveFileHandler is a declared header
#include "./src/headers/MessageHandler.h"  // Assuming MessageHandler is a declared header
#include "./src/headers/Message.h"  // Assuming Message is a declared header
#include "./Client/Parser/headers/Parser.h"  // Assuming Parser is a declared header

class Test {
public:
    Test();
    ~Test();

    void logTest(std::string testName);

    void FileHandler_FindsFilesInDir();
    void FileHandler_ReadsContentFromFile();
    void FileHandler_FindsExistingDir();
    void FileHandler_CreatesDirIfNotExists();
    void FileHandler_DeleteFile();
    void MessageHandler_CreatesMessage(Message msg);
    void MessageClass_SerializesMessage(Message msg);
    void MessageClass_DeserializesMessage(Message msg);
    void MessageHandler_GetsMessageByUserAndId(const std::string username, const int id);
    void Parser_ReadsReadCommand();
    void Parser_ParsesListCommand();
    void Parser_ParsesDeleteCommand();
    void Parser_ParsesSendCommand();

private:
    std::string testFileContent;
    std::string testFilePath;
    IFileHandler* fileHandler;
    MessageHandler* msgHandler;
    Message msg;
};

#endif  // TEST_H
