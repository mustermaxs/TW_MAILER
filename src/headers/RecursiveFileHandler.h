#ifndef RECURSIVEFILESCANNER_H
#define RECURSIVEFILESCANNER_H

#include "IFileHandler.h"


class RecursiveFileHandler : public IFileHandler
{
public:
    SearchResult searchFileInDir(const std::string fileName, const std::string dirPath) const override;
    // std::vector<SearchResult> GetAllFilesInDirIfExists(const fs::path &dirPath) const override;
};

#endif