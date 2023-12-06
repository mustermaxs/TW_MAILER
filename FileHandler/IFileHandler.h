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
    virtual SearchResult searchFileInDir(const std::string fileName, const fs::path &dirPath) const = 0;
    bool comparePaths(const std::string leftPath, const std::string rightPath) const;
    std::vector<std::string> ReadFile(fs::path& filePath);
    bool dirExists(const std::string dirName);
    bool dirExists(fs::path& dirName);
    std::string pathObjToString(fs::path& path);
    bool createDirectoryIfNotExists(std::string dirName);




};

#endif