#include "ResourceManager.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "FileManager.h"

// Create defaults to use when an unloaded resource is referenced,
// and load resources loaded for the entire duration of the program
ResourceManager::ResourceManager()
{
    LoadTexture("missingTexture", "res/images/missing_texture.png");
    LoadFont("fallbackFont", "res/fonts/roboto_mono/RobotoMono-Regular.ttf");
    LoadSoundBuffer("error", "res/sounds/error.ogg");
    
    LoadInitialResources();
}

ResourceManager::~ResourceManager()
{

}

// Load resources loaded on startup
bool ResourceManager::LoadInitialResources()
{
    #if defined(SFML_SYSTEM_ANDROID)
        std::istringstream inf(FileManager::ReadTxtFromAssets("data/initial_resources.txt"));
    #else
        std::ifstream inputFile(FileManager::ResourcePath() + "data/initial_resources.txt");
        if (!inputFile)
        {
            std::cout << "ResourceManager error: Unable to open \"data/initial_resources.txt\".\n"
                         "Initial resources loading failed.\n\n";
            return false;
        }
    #endif

    std::string line;
    while (std::getline(inputFile, line))
    {
        // Ignore empty lines or those starting with '#'
        if (line == "" || line[0] == '#')
        {
            continue;
        }

        // Read resource name
        std::string name = line.substr(0, line.find(' '));
        line.erase(0, line.find(' ') + 1);

        // Read quoted filename
        size_t firstDelimPos = line.find('"');
        size_t lastDelimPos = line.find('"', firstDelimPos + 1);
        std::string filename = line.substr(firstDelimPos + 1, lastDelimPos - (firstDelimPos + 1));

        // Load resource
        if (filename.find("images") != std::string::npos)
        {
            LoadTexture(name, filename);
        }
        else if (filename.find("fonts") != std::string::npos)
        {
            LoadFont(name, filename);
        }
        else if (filename.find("sounds") != std::string::npos)
        {
            LoadSoundBuffer(name, filename);
        }
        else
        {
            std::cout << "ResourceManager error: Unable to deduce resource type for \"" << name << "\" from filename \"" << filename << "\"\n";
        }
    }
    #if !defined(SFML_SYSTEM_ANDROID)
        inputFile.close();
    #endif
    std::cout << "Initial resources successfully loaded.\n\n";
    return true;
}

// Texture functions

// Load a Texture and bind it to the map if the key is available, and return a reference to the const loaded Texture
const sf::Texture& ResourceManager::LoadTexture(const std::string& name, const std::string& filename, const sf::IntRect& textureRect)
{
    // If a Texture is already loaded at the specified key, return the existing Texture
    std::unordered_map<std::string, sf::Texture>::const_iterator it = m_textures.find(name);
    if (it != m_textures.cend())
    {
        return it->second;
    }
    // Otherwise, load the Texture
    sf::Texture texture;
    if (!texture.loadFromFile(FileManager::ResourcePath() + filename, textureRect))
    {
        std::cout << "ResourceManager error: Failed to load texture \"" << name << "\" from file \"" << filename << "\".\n";
        return m_textures.at("missingTexture");
    }
    m_textures.emplace(name, std::move(texture));
    return m_textures.at(name);
}

// Remove a Texture from the Texture map
void ResourceManager::UnloadTexture(const std::string& name)
{
    std::unordered_map<std::string, sf::Texture>::const_iterator it = m_textures.find(name);
    if (it != m_textures.cend())
    {
        m_textures.erase(it);
    }
    else
    {
        std::cout << "ResourceManager error: Tried unloading already unloaded or inexistent texture \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded Texture
const sf::Texture& ResourceManager::GetTexture(const std::string& name) const
{
    std::unordered_map<std::string, sf::Texture>::const_iterator it = m_textures.find(name);
    if (it != m_textures.cend())
    {
        return it->second;
    }
    // If the Texture is not found, return the default Texture
    std::cout << "ResourceManager error: Tried accessing unloaded or inexistent texture \"" << name << "\".\n";
    return m_textures.at("missingTexture");
}

// Set a Texture's isRepeated value
void ResourceManager::SetTextureRepeated(const std::string& name, bool isRepeated)
{
    std::unordered_map<std::string, sf::Texture>::iterator it = m_textures.find(name);
    if (it != m_textures.end())
    {
        it->second.setRepeated(isRepeated);
    }
    else
    {
        std::cout << "ResourceManager error: Tried accessing unloaded or inexistent texture \"" << name << "\".\n";
    }
}

// Set a Texture's isSmooth value
void ResourceManager::SetTextureSmooth(const std::string& name, bool isSmooth)
{
    std::unordered_map<std::string, sf::Texture>::iterator it = m_textures.find(name);
    if (it != m_textures.end())
    {
        it->second.setRepeated(isSmooth);
    }
    else
    {
        std::cout << "ResourceManager error: Tried accessing unloaded or inexistent texture \"" << name << "\".\n";
    }
}

// Font functions

// Load a Font and bind it to the map if the key is available, and return a reference to the const loaded Font
const sf::Font& ResourceManager::LoadFont(const std::string& name, const std::string& filename)
{
    // If a Font is already loaded at the specified key, return the existing Font
    std::unordered_map<std::string, sf::Font>::const_iterator it = m_fonts.find(name);
    if (it != m_fonts.cend())
    {
        return it->second;
    }
    // Otherwise, load the Font
    sf::Font font;
    if (!font.loadFromFile(FileManager::ResourcePath() + filename))
    {
        std::cout << "ResourceManager error: Failed to load font \"" << name << "\" from file \"" << filename << "\".\n";
        return m_fonts.at("fallbackFont");
    }
    m_fonts.emplace(name, std::move(font));
    return m_fonts.at(name);
}

// Remove a Font from the Font map
void ResourceManager::UnloadFont(const std::string& name)
{
    std::unordered_map<std::string, sf::Font>::const_iterator it = m_fonts.find(name);
    if (it != m_fonts.cend())
    {
        m_fonts.erase(it);
    }
    else
    {
        std::cout << "ResourceManager error: Tried unloading already unloaded or inexistent font \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded Font
const sf::Font& ResourceManager::GetFont(const std::string& name) const
{
    std::unordered_map<std::string, sf::Font>::const_iterator it = m_fonts.find(name);
    if (it != m_fonts.cend())
    {
        return it->second;
    }
    // If the Font is not found, return the default Font
    std::cout << "ResourceManager error: Tried accessing unloaded or inexistent font \"" << name << "\".\n";
    return m_fonts.at("fallbackFont");
}

// SoundBuffer functions

// Load a SoundBuffer and bind it to the map if the key is available, and return a reference to the const loaded SoundBuffer
const sf::SoundBuffer& ResourceManager::LoadSoundBuffer(const std::string& name, const std::string& filename)
{
    // If a SoundBuffer is already loaded at the specified key, return the existing SoundBuffer
    std::unordered_map<std::string, sf::SoundBuffer>::const_iterator it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.cend())
    {
        return it->second;
    }
    // Otherwise, load the SoundBuffer
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(FileManager::ResourcePath() + filename))
    {
        std::cout << "ResourceManager error: Failed to load soundbuffer \"" << name << "\" from file \"" << filename << "\".\n";
        return m_soundBuffers.at("error");
    }
    m_soundBuffers.emplace(name, std::move(soundBuffer));
    return m_soundBuffers.at(name);
}

// Remove a SoundBuffer from the SoundBuffer map
void ResourceManager::UnloadSoundBuffer(const std::string& name)
{
    std::unordered_map<std::string, sf::SoundBuffer>::const_iterator it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.cend())
    {
        m_soundBuffers.erase(it);
    }
    else
    {
        std::cout << "ResourceManager error: Tried unloading already unloaded or inexistent soundbuffer \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded SoundBuffer
const sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& name) const
{
    std::unordered_map<std::string, sf::SoundBuffer>::const_iterator it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.cend())
    {
        return it->second;
    }
    // If the SoundBuffer is not found, return the default SoundBuffer
    std::cout << "ResourceManager error: Tried accessing unloaded or inexistent soundbuffer \"" << name << "\".\n";
    return m_soundBuffers.at("error");
}

// Shader functions

// Load a Shader and bind it to the map if the key is available, and return a reference to the const loaded Shader
const sf::Shader& ResourceManager::LoadShader(const std::string& name, const std::string& filename, sf::Shader::Type type)
{
    // If a Shader is already loaded at the specified key, return the existing Shader
    std::unordered_map<std::string, sf::Shader>::const_iterator it = m_shaders.find(name);
    if (it != m_shaders.cend())
    {
        return it->second;
    }
    // Otherwise, load the Shader
    m_shaders[name];
    if (!m_shaders.at(name).loadFromFile(FileManager::ResourcePath() + filename, type))
    {
        std::cout << "ResourceManager error: Failed to load shader \"" << name << "\" from file \"" << filename << "\".\n";
        m_shaders.erase(name);
        return m_shaders.at("ADDDEFAULTSHADER");
    }
    return m_shaders.at(name);
}

// Remove a Shader from the Shader map
void ResourceManager::UnloadShader(const std::string& name)
{
    std::unordered_map<std::string, sf::Shader>::const_iterator it = m_shaders.find(name);
    if (it != m_shaders.cend())
    {
        m_shaders.erase(it);
    }
    else
    {
        std::cout << "ResourceManager error: Tried unloading already unloaded or inexistent shader \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded Shader
const sf::Shader& ResourceManager::GetShader(const std::string& name) const
{
    std::unordered_map<std::string, sf::Shader>::const_iterator it = m_shaders.find(name);
    if (it != m_shaders.cend())
    {
        return it->second;
    }
    // If the SoundBuffer is not found, return the default SoundBuffer
    std::cout << "ResourceManager error: Tried accessing unloaded or inexistent shader \"" << name << "\".\n";
    return m_shaders.at("ADDDEFAULTSHADER");
}
