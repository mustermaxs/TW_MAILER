#include "./headers/IFileHandler.h"
// TODO Write to file
// TODO Create directory

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

IFileHandler::~IFileHandler(){};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Checks if two paths are the same.
/// @param leftPath first path
/// @param rightPath second path
/// @return True if both paths are the same.
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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief reads file content into vector of strings for
/// @param filePath File path to read contents from
/// @return Vector of strings where each string element
/// represents a single line of the file content.
std::vector<std::string> IFileHandler::readFileLines(const std::string filePath)
{
    std::string line;
    std::vector<std::string> lines;
    fs::path path = fs::path(filePath);

    std::ifstream fileStream;
    fileStream.open(filePath);

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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Reads the content of a file and returns it as a string.
/// @param filePath string - file path.
/// @return string - file content.
std::string IFileHandler::readFile(const std::string filePath)
{
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        throw new std::runtime_error("Failed to read file " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Converts filesystem path object to string
/// @param path filesystem::path object
/// @return String
std::string IFileHandler::pathObjToString(fs::path &path)
{
    return path.u8string();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Checks if provided directory exists.
/// @param path Directory path as filesyste::path object
/// @return True if the provided directory path exists.
bool IFileHandler::dirExists(fs::path &path)
{
    return fs::is_directory(path);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Checks if provided directory exists.
/// @param path Directory path as string.
/// @return True if the provided directory path exists.
bool IFileHandler::dirExists(const std::string path)
{
    fs::path p = fs::path(path);

    return fs::is_directory(p);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Creates a directory if it doesn't exist already.
/// @param dirName String - path to object
/// @return True if directory exists.
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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Write string to file.
/// @param newFilePath String - path to file.
/// @param content String - file content.
/// @return True if attempt to write to file succeded.
bool IFileHandler::writeToFile(const std::string newFilePath, const std::string &content)
{
    try
    {
        std::ofstream file(newFilePath);
        if (!file.is_open())
        {
            throw std::runtime_error("File not opened: " + newFilePath);
        }
        if (!file)
        {
            throw std::runtime_error("Failed to open file for writing: " + newFilePath);
        }

        file << content;

        if (!file)
        {
            throw std::runtime_error("Failed to write to file: " + newFilePath);
        }

        file.close();

        return true;
    }
    catch (const std::exception &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;

        return false;
    }
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Returns file names stored under provided directory path.
/// @param dirName String - Directory path.
/// @return Vector of strings representing the file names.
std::vector<std::string> IFileHandler::getFileNamesInDir(const std::string dirName)
{
    std::vector<std::string> fileNames;
    fs::path dirPathObj = fs::path(dirName);

    if (!this->dirExists(dirPathObj))
        throw new std::invalid_argument("Directory doesn't exist");

    for (const auto &entry : fs::directory_iterator(dirName))
    {
        fileNames.push_back(entry.path().filename().u8string());
    }

    return fileNames;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/// @brief Deletes file if it exists.
/// @param pathName String - file path.
/// @return True if the file was deleted successfully.
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

