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

int FileManager::getFileCount(const std::string& directory)
{
#if !defined(SFML_SYSTEM_WINDOWS) // MSVC does not support dirent.h
    DIR* directoryStream = opendir(directory.c_str());
    dirent* currentFile;
    int fileCount = -2;

    if (directoryStream == nullptr)
    {
        std::cout << "Failed to open directory: " << directory << '\n';
        return 0;
    }

    while ((currentFile = readdir(directoryStream)))
    {
        ++fileCount;
    }

    closedir(directoryStream);
    return fileCount;
#else
    return 0;
#endif
}

std::vector<std::string> FileManager::getFilenamesInDirectory(const std::string& directory)
{
#if !defined(SFML_SYSTEM_WINDOWS) // MSVC does not support dirent.h
    std::vector<std::string> filenames;
    DIR* directoryStream = opendir(directory.c_str());
    dirent* currentFile;

    if (directoryStream == nullptr)
    {
        std::cout << "Failed to open directory: " << directory << '\n';
        return filenames;
    }

    while ((currentFile = readdir(directoryStream)))
    {
        filenames.push_back(currentFile->d_name);
    }

    closedir(directoryStream);
    return filenames;
#else
    return {};
#endif
}

#if defined(SFML_SYSTEM_ANDROID)
/// Read a given compressed text file from the assets directory on Android
std::string FileManager::readTxtFromAssets(const std::string& filename)
{
    ANativeActivity* nativeActivity = sf::getNativeActivity();
    AAssetManager* assetManager = nativeActivity->assetManager;

    AAsset* file = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
    std::size_t fileLength = AAsset_getLength(file);

    char* buffer = new char[fileLength + 1];

    AAsset_read(file, buffer, fileLength);
    buffer[fileLength] = '\0';

    std::string text = buffer;
    delete[] buffer;

    return text;
}
#endif
