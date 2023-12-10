#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


struct SearchResult
{
    bool fileExists;
    fs::directory_entry path;
};

class IFileHandler
{

public:
    bool ignoreCase = false;
    virtual ~IFileHandler();
    // virtual std::vector<SearchResult> GetAllFilesInDirIfExists(const fs::path &dirPath) const = 0;
    virtual SearchResult searchFileInDir(const std::string fileName, const std::string dirPath) const = 0;
    bool comparePaths(const std::string leftPath, const std::string rightPath) const;
    std::vector<std::string> readFile(const std::string filePath);
    bool dirExists(const std::string dirName);
    bool dirExists(fs::path& dirName);
    std::string pathObjToString(fs::path& path);
    bool createDirectoryIfNotExists(std::string dirName);
    bool writeToFile(const std::string fileName, const std::string& content);
    std::vector<std::string> getFileNamesInDir(const std::string dirName);
    bool deleteFile(const std::string pathName);
};



#endif