#include "../FileHandler/RecursiveFileHandler.h"

SearchResult RecursiveFileHandler::searchFileInDir(const std::string fileName, const fs::path &dirPath) const
{
    SearchResult searchRes;
    searchRes.fileExists = false;

    //loops through current directory and directories below
    for (auto const &dir_entry : std::filesystem::directory_iterator(dirPath))
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