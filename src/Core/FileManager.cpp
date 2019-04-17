#include "Core/FileManager.h"
#include <iostream>
#include <SFML/System.hpp>
#if !defined(SFML_SYSTEM_WINDOWS) // MSVC does not support dirent.h
#include <dirent.h>
#endif
#if defined(SFML_SYSTEM_ANDROID)
#include <SFML/System/NativeActivity.hpp>
#include <android/asset_manager.h>
#include <android/native_activity.h>
#endif

int FileManager::GetFileCount(const std::string& directory)
{
#if !defined(SFML_SYSTEM_WINDOWS) // MSVC does not support dirent.h
    DIR* pDirectoryStream = opendir(directory.c_str());
    dirent* pCurrentFile;
    int fileCount = -2;

    if (pDirectoryStream == nullptr)
    {
        std::cout << "Failed to open directory: " << directory << '\n';
        return 0;
    }

    while ((pCurrentFile = readdir(pDirectoryStream)))
    {
        ++fileCount;
    }

    closedir(pDirectoryStream);
    return fileCount;
#else
    return 0;
#endif
}

std::vector<std::string> FileManager::GetFilenamesInDirectory(const std::string& directory)
{
#if !defined(SFML_SYSTEM_WINDOWS) // MSVC does not support dirent.h
    std::vector<std::string> filenames;
    DIR* pDirectoryStream = opendir(directory.c_str());
    dirent* pCurrentFile;

    if (pDirectoryStream == nullptr)
    {
        std::cout << "Failed to open directory: " << directory << '\n';
        return filenames;
    }

    while ((pCurrentFile = readdir(pDirectoryStream)))
    {
        filenames.push_back(pCurrentFile->d_name);
    }

    closedir(pDirectoryStream);
    return filenames;
#else
    return {};
#endif
}

#if defined(SFML_SYSTEM_ANDROID)
/// Read a given compressed text file from the assets directory on Android
std::string FileManager::ReadTxtFromAssets(const std::string& filename)
{
    ANativeActivity* pNativeActivity = sf::getNativeActivity();
    AAssetManager* pAssetManager = pNativeActivity->assetManager;

    AAsset* pFile = AAssetManager_open(pAssetManager, filename.c_str(), AASSET_MODE_BUFFER);
    std::size_t fileLength = AAsset_getLength(pFile);

    char* buffer = new char[fileLength + 1];

    AAsset_read(pFile, buffer, fileLength);
    buffer[fileLength] = '\0';

    std::string text = buffer;
    delete[] buffer;

    return text;
}
#endif
