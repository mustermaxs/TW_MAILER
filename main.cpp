#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "./src/headers/IFileHandler.h"
#include "./src/headers/RecursiveFileHandler.h"
#include "./src/headers/Message.h"
#include "./src/headers/Color.h"
#include "./src/headers/MessageHandler.h"

namespace fs = std::filesystem;

class Test
{
public:
    std::string testFileContent = "eiweck nervt";
    std::string testFilePath = "testfile.txt";
    IFileHandler *fileHandler = new RecursiveFileHandler();

    ~Test()
    {
    }

    void logTest(std::string testName)
    {
        Color::Modifier green(Color::FG_GREEN);
        Color::Modifier resetColor(Color::FG_DEFAULT);
        std::cout << green << testName << " ✓" << resetColor << std::endl;
    }

    void FileHandler_FindsFilesInDir()
    {
        std::vector<std::string> fileNames = fileHandler->getFileNamesInDir("./arschlochDirectory");

        // work of fart
        for (const auto &entry : fileNames)
        {
            bool found = false;
            std::vector<std::string> expectedFileNames = {"1", "2", "3"};

            for (const auto &expected : expectedFileNames)
            {
                found = found || expected == entry;
            }

            assert(found && "should find 1, 2, 3");
        }

        logTest(__FUNCTION__);
    };

    void FileHandler_ReadsContentFromFile()
    {
        fileHandler->writeToFile(testFilePath, testFileContent);
        fs::path p = fs::path(testFilePath);
        std::vector<std::string> plines = fileHandler->readFile(p);
        std::string firstLine = plines[0];

        assert(firstLine == testFileContent);
        logTest(__FUNCTION__);
    };

    void FileHandler_FindsExistingDir()
    {
        assert(fileHandler->dirExists("arschlochDirectory"));
        assert(fileHandler->dirExists(".vscode") == true);
        logTest(__FUNCTION__);
    };

    void FileHandler_CreatesDirIfNotExists()
    {
        assert(fileHandler->createDirectoryIfNotExists("./arschlochDirectory"));
        logTest(__FUNCTION__);
    };

    void MessageHandler_CreatesMessage()
    {
        Message msg;
        msg.setSender("sender")->setReceiver("receiver")->setSubject("subject")->setContent("content");

        MessageHandler msgHandler(fileHandler);
        std::string username = "max";

        msgHandler.createMessage(username, msg);

        std::string expectedPath = "./messages/max/";

        assert(fileHandler->dirExists("./messages/max") && "messages/max/ directory doesn't exist.");

        assert(fileHandler->searchFileInDir("1", expectedPath).fileExists && "message file should exist");
        logTest(__FUNCTION__);
    }

    void FileHandler_DeleteFile()
    {
        Message msg;
        msg.setSender("sender")->setReceiver("receiver")->setSubject("subject")->setContent("content");

        MessageHandler msgHandler(fileHandler);
        std::string username = "test";

        msgHandler.createMessage(username, msg);

        fileHandler->deleteFile("./messages/test/1");

        assert(!(fileHandler->searchFileInDir("1", "./messages/test/").fileExists) && "Failed to delete message");

        logTest(__FUNCTION__);
    }
};

int main()
{
    Test test;

    test.FileHandler_FindsFilesInDir();
    test.FileHandler_ReadsContentFromFile();
    test.FileHandler_FindsExistingDir();
    test.FileHandler_CreatesDirIfNotExists();
    test.MessageHandler_CreatesMessage();
    test.FileHandler_DeleteFile();

    return 0;
}
