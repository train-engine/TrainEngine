#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <SFML/Config.hpp>

namespace FileManager
{
#if defined(SFML_SYSTEM_MACOS)
    std::string ResourcePath();
#else
    inline std::string ResourcePath() { return ""; }
#endif

    int GetFileCount(const std::string& directory);
    std::vector<std::string> GetFilenamesInDirectory(const std::string& directory);

#if defined(SFML_SYSTEM_ANDROID)
    std::string ReadTxtFromAssets(const std::string& filename);
#endif
} // namespace FileManager

#endif // FILEMANAGER_H
