#include "../FileHandler/IFileHandler.h"

IFileHandler::~IFileHandler(){};

bool IFileHandler::comparePaths(const std::string leftPath, const std::string rightPath) const
{
    if (!ignoreCase)
    {
        return leftPath == rightPath;
    }

    // initialize lower case strings
    std::string lcLeftPath(leftPath.size(), '\0');
    std::string lcRightPath(rightPath.size(), '\0');

    // transform strings to lower case
    std::transform(leftPath.begin(), leftPath.end(), lcLeftPath.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::transform(rightPath.begin(), rightPath.end(), lcRightPath.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    return lcLeftPath == lcRightPath;
};

std::vector<std::string> IFileHandler::ReadFile(fs::path &filePath)
{
    std::string line;
    std::string path = filePath.u8string();
    std::vector<std::string> lines;

    std::ifstream fileStream;
    fileStream.open(path);

    if (fileStream.is_open())
    {
        while (std::getline(fileStream, line))
        {
            lines.push_back(line);
        }

        fileStream.close();
    }

    return lines;
};

std::string IFileHandler::pathObjToString(fs::path& path)
{
    return path.u8string();
};

bool IFileHandler::dirExists(fs::path& path)
{
    return fs::is_directory(path);
};

bool IFileHandler::dirExists(const std::string path)
{
    fs::path p = fs::path(path);

    return fs::is_directory(p);
};

bool IFileHandler::createDirectoryIfNotExists(std::string dirName)
{
    // ? wo root directory deklarieren
}

// TODO Write to file
// TODO Create directory