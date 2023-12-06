#ifndef RECURSIVEFILESCANNER_H
#define RECURSIVEFILESCANNER_H

#include "../FileHandler/IFileHandler.h"


class RecursiveFileHandler : public IFileHandler
{
public:
    SearchResult searchFileInDir(const std::string fileName, const fs::path &dirPath) const override;
    // std::vector<SearchResult> GetAllFilesInDirIfExists(const fs::path &dirPath) const override;
};

#endif