#include "./headers/RecursiveFileHandler.h"

SearchResult RecursiveFileHandler::searchFileInDir(const std::string fileName, const std::string dirPath) const
{
    SearchResult searchRes;
    searchRes.fileExists = false;
    fs::path dir = fs::path(dirPath);

    for (auto const &dir_entry : std::filesystem::directory_iterator(dir))
    {
        if (dir_entry.is_regular_file())
        {
            if (this->comparePaths(fileName, dir_entry.path().filename()))
            {

                searchRes.fileExists = true;
                searchRes.path = dir_entry;

                break;
            }
        }
    }

    return searchRes;
};