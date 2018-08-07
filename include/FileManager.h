#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Config.hpp>

namespace FileManager
{
    int GetFileCount(const std::string& directory);
    std::vector<std::string> GetFilenamesInDirectory(const std::string& directory);

    #if defined(SFML_SYSTEM_ANDROID)
        std::string ReadTxtFromAssets(const std::string& filename);
    #endif
}

#endif // FILEMANAGER_H_INCLUDED
