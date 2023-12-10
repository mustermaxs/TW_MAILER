#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <cstdlib>

#include "./src/headers/IFileHandler.h"
#include "./src/headers/RecursiveFileHandler.h"
#include "./src/headers/Message.h"
#include "./src/headers/Color.h"
#include "./src/headers/MessageHandler.h"

namespace fs = std::filesystem;

template <typename T>
void customAssert(bool check, const std::string &failMessage, const std::string &testName)
{
    if (!check)
    {
        Color::Modifier red(Color::FG_RED);
        Color::Modifier resetColor(Color::FG_DEFAULT);

        std::cerr << red << "[TEST FAILED]: " << testName << ":" << failMessage << resetColor << std::endl;
        std::abort();
    }
};

class Test
{
public:
    std::string testFileContent = "eiweck nervt\nso sehr";
    std::string testFilePath = "testfile.txt";
    IFileHandler *fileHandler = new RecursiveFileHandler();
    MessageHandler *msgHandler;
    Message msg;

    Test()
    {
        /* SETUP Message instance */
        Message msg;
        std::string sender = "max";
        std::string receiver = "eiweck";
        std::string subject = "Don't forget";
        std::string content = "I'm an idiot!";

        msg.setSender(sender)->setReceiver(receiver)->setSubject(subject)->setContent(content);
        this->msg = msg;

        /* SETUP MessageHandler */
        this->msgHandler = new MessageHandler(fileHandler);
    }
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

        for (const auto &entry : fileNames)
        {
            bool found = false;
            std::vector<std::string> expectedFileNames = {"1", "2", "3"};

            for (const auto &expected : expectedFileNames)
            {
                found = found || expected == entry;
            }

            customAssert<std::string>(found == true, "should find 1, 2, 3", __FUNCTION__);
        }

        logTest(__FUNCTION__);
    };

    void FileHandler_ReadsContentFromFile()
    {
        std::vector<std::string> vTestFileContent = {
            "eiweck nervt",
            "so sehr"
        };
        
        std::vector<std::string> plines = fileHandler->readFileLines(testFilePath);

        for (int i = 0; i < plines.size(); i++)
        {
            customAssert<std::string>(plines[i] == vTestFileContent[i], "Expected: "+vTestFileContent[i]+"\nProvided: "+plines[i], __FUNCTION__);
        }

        logTest(__FUNCTION__);
    };

    void FileHandler_FindsExistingDir()
    {
        customAssert<std::string>(fileHandler->dirExists("arschlochDirectory") == true, "directory should exist", __FUNCTION__);
        customAssert<std::string>((fileHandler->dirExists(".vscode") == true), "directory should exist", __FUNCTION__);
        logTest(__FUNCTION__);
    };

    void FileHandler_CreatesDirIfNotExists()
    {
        customAssert<std::string>(fileHandler->dirExists("arschlochDirectory") == true, "directory should exist", __FUNCTION__);
        logTest(__FUNCTION__);
    };

    void MessageHandler_CreatesMessage(Message msg)
    {
        MessageHandler msgHandler(fileHandler);
        std::string username = msg.getReceiver();
        msgHandler.saveMessage(username, msg);

        std::string expectedPath = "./messages/" + msg.getReceiver();

        customAssert<std::string>(fileHandler->dirExists(expectedPath) == true, " directory doesn't exist.", __FUNCTION__);

        customAssert<std::string>(fileHandler->searchFileInDir("1", expectedPath).fileExists == true, "message file should exist", __FUNCTION__);

        logTest(__FUNCTION__);
    }

    void FileHandler_DeleteFile()
    {
        Message msg;
        msg.setSender("test")->setReceiver("test")->setSubject("test")->setContent("test");

        MessageHandler msgHandler(fileHandler);
        std::string username = "test";

        msgHandler.saveMessage(username, msg);

        fileHandler->deleteFile("./messages/test/1");

        customAssert<std::string>(fileHandler->searchFileInDir("1", "./messages/test/").fileExists == false, "Failed to delete message", __FUNCTION__);

        logTest(__FUNCTION__);
    };

    void MessageClass_SerializesMessage(Message msg)
    {
        std::string expectedContent =
            "SENDER:" + msg.getSender() + "\n" +
            "RECEIVER:" + msg.getReceiver() + "\n" +
            "SUBJECT:" + msg.getSubject() + "\n" +
            "MESSAGE:" + msg.getContent() + "\n";

        std::string serizaliedMsg = msg.toString();

        customAssert<std::string>(expectedContent == serizaliedMsg, "Failed to serialize message.", __FUNCTION__);
        logTest(__FUNCTION__);
    };

    void MessageClass_DeserializesMessage(Message msg)
    {
        std::vector<std::string> expectedContent =
            {"SENDER:max",
             "RECEIVER:eiweck",
             "SUBJECT:Don't forget",
             "MESSAGE:I'm an idiot!"};

        Message deserializedMsg = Message::fromString(expectedContent);

        customAssert<std::string>(deserializedMsg.getSender() == msg.getSender(), "Failed to deserialize message sender.", __FUNCTION__);
        customAssert<std::string>(deserializedMsg.getReceiver() == msg.getReceiver(), "Failed to deserialize message receiver.", __FUNCTION__);
        customAssert<std::string>(deserializedMsg.getSubject() == msg.getSubject(), "Failed to deserialize message subject.", __FUNCTION__);
        customAssert<std::string>(deserializedMsg.getContent() == msg.getContent(), "Failed to deserialize message content.", __FUNCTION__);

        logTest(__FUNCTION__);
    };

    void MessageHandler_GetsMessageByUserAndId(const std::string username, const int id)
    {
        MessageHandler msgHandler(fileHandler);
        Message msg = msgHandler.getMessage(username, id);

        customAssert<std::string>(msg.getSender() == "max", "Failed to get message sender by user and id", __FUNCTION__);
        customAssert<std::string>(msg.getReceiver() == "eiweck", "Failed to get message receiver by user and id", __FUNCTION__);
        customAssert<std::string>(msg.getSubject() == "Don't forget", "Failed to get message subject by user and id", __FUNCTION__);
        customAssert<std::string>(msg.getContent() == "I'm an idiot!", "Failed to get message content by user and id", __FUNCTION__);

        logTest(__FUNCTION__);
    };
};

int main()
{
    Test test;

    test.FileHandler_FindsFilesInDir();
    test.FileHandler_ReadsContentFromFile();
    test.FileHandler_FindsExistingDir();
    test.FileHandler_CreatesDirIfNotExists();
    test.MessageHandler_CreatesMessage(test.msg);
    test.FileHandler_DeleteFile();
    test.MessageClass_SerializesMessage(test.msg);
    test.MessageClass_DeserializesMessage(test.msg);
    test.MessageHandler_GetsMessageByUserAndId("eiweck", 1);

    return 0;
}