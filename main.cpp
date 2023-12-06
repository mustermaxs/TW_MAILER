#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "./FileHandler/IFileHandler.h"
#include "./FileHandler/RecursiveFileHandler.h"

namespace fs = std::filesystem;


int main()
{
    fs::path dirPath = fs::path("./");
    RecursiveFileHandler fileScanner = RecursiveFileHandler();
    
    assert(fileScanner.searchFileInDir("main.cpp", dirPath).fileExists == true);
    assert(fileScanner.dirExists("arschlochDirectory") == false);
    assert(fileScanner.dirExists(".vscode") == true);
    // assert(fileScanner.pathObjToString(dirPath) == "./FileHandler/IFileHandler.cpp");


    return 0;
}

