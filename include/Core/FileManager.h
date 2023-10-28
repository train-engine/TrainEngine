#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <SFML/Config.hpp>

namespace FileManager
{
#if defined(SFML_SYSTEM_MACOS) || defined(SFML_SYSTEM_IOS)
    std::string resourcePath();
#else
    inline std::string resourcePath()
    {
        return "";
    }
#endif

    int getFileCount(const std::string& directory);
    std::vector<std::string> getFilenamesInDirectory(const std::string& directory);

#if defined(SFML_SYSTEM_ANDROID)
    std::string readTxtFromAssets(const std::string& filename);
#endif
} // namespace FileManager

#endif // FILEMANAGER_H
