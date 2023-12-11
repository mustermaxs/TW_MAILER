#include "./headers/IFileHandler.h"

IFileHandler::~IFileHandler(){};

bool IFileHandler::comparePaths(const std::string leftPath, const std::string rightPath) const
{
    if (!ignoreCase)
    {
        return leftPath == rightPath;
    }

    std::string lcLeftPath(leftPath.size(), '\0');
    std::string lcRightPath(rightPath.size(), '\0');

    // transform strings to lower case
    std::transform(leftPath.begin(), leftPath.end(), lcLeftPath.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::transform(rightPath.begin(), rightPath.end(), lcRightPath.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    return lcLeftPath == lcRightPath;
};


// reads file content into vector of strings for
// easier string handling
std::vector<std::string> IFileHandler::readFileLines(const std::string filePath)
{
    std::string line;
    std::vector<std::string> lines;
    fs::path path = fs::path(filePath);

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




std::string IFileHandler::pathObjToString(fs::path &path)
{
    return path.u8string();
};




bool IFileHandler::dirExists(fs::path &path)
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
    try
    {
        if (this->dirExists(dirName))
        {
            return true;
        }

        fs::path dirPathObj = fs::path(dirName);

        return fs::create_directory(dirPathObj);
    }
    catch (const fs::filesystem_error &ex)
    {
        std::cout << "Failed to create directory " << dirName << ex.what() << std::endl;

        return false;
    }
};




bool IFileHandler::writeToFile(const std::string fileName, const std::string &content)
{
    try
    {
        std::ofstream file;

        file.open(fileName);
        file << content;
        file.close();

        return true;
    }
    catch (...)
    {
        std::cout << "Failed to write to file " << fileName << std::endl;

        return false;
    }
};



// returns file names stored under provided directory path
// in vector of strings
std::vector<std::string> IFileHandler::getFileNamesInDir(const std::string dirName)
{
    std::vector<std::string> fileNames;

    for (const auto &entry : fs::directory_iterator(dirName))
    {
        fileNames.push_back(entry.path().filename().u8string());
    }

    return fileNames;
};




bool IFileHandler::deleteFile(const std::string pathName)
{
    try
    {
        fs::path pathObj = fs::path(pathName);

        return fs::remove(pathObj);
    }
    catch (...)
    {
        std::cout << "Failed to delete file" << pathName << std::endl;
        
        return false;
    }
};

// TODO Write to file
// TODO Create directory