#ifndef RECURSIVEFILESCANNER_H
#define RECURSIVEFILESCANNER_H

#include "IFileHandler.h"


class FileHandler : public IFileHandler
{
public:
    SearchResult searchFileInDirRecursively(const std::string fileName, const std::string dirPath) const override;
    // std::vector<SearchResult> GetAllFilesInDirIfExists(const fs::path &dirPath) const override;
};

#endif