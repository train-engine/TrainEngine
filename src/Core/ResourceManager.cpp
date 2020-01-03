#include "Core/ResourceManager.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Core/FileManager.h"

// Create defaults to use when an unloaded resource is referenced,
// and load resources loaded for the entire duration of the program
ResourceManager::ResourceManager()
{
    // The sf::Context below is unused but its existence is necessary to make OpenGL calls without having
    // an active window, which is the case here when loading textures before the window has been created
    sf::Context context;

    loadTexture("missingTexture", "res/images/missing_texture.png");
    loadFont("fallbackFont", "res/fonts/roboto_mono/RobotoMono-Regular.ttf");
    loadSoundBuffer("error", "res/sounds/error.ogg");

    loadInitialResources();
}

ResourceManager::~ResourceManager()
{
}

// Load resources loaded on startup
bool ResourceManager::loadInitialResources()
{
    static const std::string initialResourcesFilename = "data/initial_resources.txt";
#if defined(SFML_SYSTEM_ANDROID)
    std::istringstream inputFile(FileManager::readTxtFromAssets(initialResourcesFilename));
#else
    std::ifstream inputFile(FileManager::resourcePath() + initialResourcesFilename);
#endif

    if (inputFile)
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            // Ignore empty lines or those starting with '#'
            if (line.empty() || line.front() == '#')
            {
                continue;
            }

            // Read resource name
            std::string name = line.substr(0, line.find(' '));

            // Read quoted filename
            std::size_t firstDelimPos = line.find('"', name.size() + 1);
            std::size_t lastDelimPos = line.find('"', firstDelimPos + 1);
            std::string filename = line.substr(firstDelimPos + 1, lastDelimPos - (firstDelimPos + 1));

            // Load resource
            if (filename.find("images") != std::string::npos)
            {
                loadTexture(name, filename);
            }
            else if (filename.find("fonts") != std::string::npos)
            {
                loadFont(name, filename);
            }
            else if (filename.find("sounds") != std::string::npos)
            {
                loadSoundBuffer(name, filename);
            }
            else if (filename.find("shaders") != std::string::npos)
            {
                // Parse extension from filename
                std::size_t lastPeriodPos = filename.rfind('.');
                std::string extension;
                if (lastPeriodPos != std::string::npos)
                {
                    extension = filename.substr(lastPeriodPos + 1);
                }

                // Deduce shader type from extension
                if (extension == "vert")
                {
                    loadShader(name, filename, sf::Shader::Vertex);
                }
                else if (extension == "geom")
                {
                    loadShader(name, filename, sf::Shader::Geometry);
                }
                else if (extension == "frag")
                {
                    loadShader(name, filename, sf::Shader::Fragment);
                }
                else
                {
                    std::cerr << "ResourceManager error: Unable to deduce shader type for filename \"" << filename
                              << "\" from unknown extension \"" << extension << "\" (extension should be .vert, .geom or .frag)\n";
                }
            }
            else
            {
                std::cerr << "ResourceManager error: Unable to deduce resource type for \"" << name << "\" from filename \"" << filename
                          << "\"\n";
            }
        }

        std::cout << "Initial resources successfully loaded.\n\n";
        return true;
    }

    std::cerr << "ResourceManager error: Unable to open \"" << initialResourcesFilename << "\".\n"
              << "Initial resources loading failed.\n\n";
    return false;
}

// Texture functions

// Load a texture and bind it to the map if the key is available, and return a reference to the const loaded texture
const sf::Texture& ResourceManager::loadTexture(const std::string& name, const std::string& filename, const sf::IntRect& textureRect)
{
    // If a texture is already loaded at the specified key, return the existing texture
    auto it = m_textures.find(name);
    if (it != m_textures.cend())
    {
        return it->second;
    }

    // Otherwise, load the texture
    sf::Texture texture;
    if (!texture.loadFromFile(FileManager::resourcePath() + filename, textureRect))
    {
        std::cerr << "ResourceManager error: Failed to load texture \"" << name << "\" from file \"" << filename << "\".\n";
        return m_textures.at("missingTexture");
    }
    m_textures.emplace(name, std::move(texture));
    return m_textures.at(name);
}

// Remove a texture from the texture map
void ResourceManager::unloadTexture(const std::string& name)
{
    auto it = m_textures.find(name);
    if (it != m_textures.cend())
    {
        m_textures.erase(it);
    }
    else
    {
        std::cerr << "ResourceManager error: Tried unloading already unloaded or nonexistent texture \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded texture
const sf::Texture& ResourceManager::getTexture(const std::string& name) const
{
    auto it = m_textures.find(name);
    if (it != m_textures.cend())
    {
        return it->second;
    }

    // If the texture is not found, return the default texture
    std::cerr << "ResourceManager error: Tried accessing unloaded or nonexistent texture \"" << name << "\".\n";
    return m_textures.at("missingTexture");
}

// Set a texture's isRepeated value
void ResourceManager::setTextureRepeated(const std::string& name, bool isRepeated)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
    {
        it->second.setRepeated(isRepeated);
    }
    else
    {
        std::cerr << "ResourceManager error: Tried accessing unloaded or nonexistent texture \"" << name << "\".\n";
    }
}

// Set a texture's isSmooth value
void ResourceManager::setTextureSmooth(const std::string& name, bool isSmooth)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
    {
        it->second.setRepeated(isSmooth);
    }
    else
    {
        std::cerr << "ResourceManager error: Tried accessing unloaded or nonexistent texture \"" << name << "\".\n";
    }
}

// Font functions

// Load a font and bind it to the map if the key is available, and return a reference to the const loaded font
const sf::Font& ResourceManager::loadFont(const std::string& name, const std::string& filename)
{
    // If a font is already loaded at the specified key, return the existing font
    auto it = m_fonts.find(name);
    if (it != m_fonts.cend())
    {
        return it->second;
    }

    // Otherwise, load the font
    sf::Font font;
    if (!font.loadFromFile(FileManager::resourcePath() + filename))
    {
        std::cerr << "ResourceManager error: Failed to load font \"" << name << "\" from file \"" << filename << "\".\n";
        return m_fonts.at("fallbackFont");
    }
    m_fonts.emplace(name, std::move(font));
    return m_fonts.at(name);
}

// Remove a font from the font map
void ResourceManager::unloadFont(const std::string& name)
{
    auto it = m_fonts.find(name);
    if (it != m_fonts.cend())
    {
        m_fonts.erase(it);
    }
    else
    {
        std::cerr << "ResourceManager error: Tried unloading already unloaded or nonexistent font \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded font
const sf::Font& ResourceManager::getFont(const std::string& name) const
{
    auto it = m_fonts.find(name);
    if (it != m_fonts.cend())
    {
        return it->second;
    }

    // If the font is not found, return the default font
    std::cerr << "ResourceManager error: Tried accessing unloaded or nonexistent font \"" << name << "\".\n";
    return m_fonts.at("fallbackFont");
}

// SoundBuffer functions

// Load a sound buffer and bind it to the map if the key is available, and return a reference to the const loaded sound buffer
const sf::SoundBuffer& ResourceManager::loadSoundBuffer(const std::string& name, const std::string& filename)
{
    // If a sound buffer is already loaded at the specified key, return the existing sound buffer
    auto it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.cend())
    {
        return it->second;
    }

    // Otherwise, load the sound buffer
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(FileManager::resourcePath() + filename))
    {
        std::cerr << "ResourceManager error: Failed to load sound buffer \"" << name << "\" from file \"" << filename << "\".\n";
        return m_soundBuffers.at("error");
    }
    m_soundBuffers.emplace(name, std::move(soundBuffer));
    return m_soundBuffers.at(name);
}

// Remove a sound buffer from the sound buffer map
void ResourceManager::unloadSoundBuffer(const std::string& name)
{
    auto it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.cend())
    {
        m_soundBuffers.erase(it);
    }
    else
    {
        std::cerr << "ResourceManager error: Tried unloading already unloaded or nonexistent sound buffer \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded sound buffer
const sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& name) const
{
    auto it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.cend())
    {
        return it->second;
    }

    // If the sound buffer is not found, return the default sound buffer
    std::cerr << "ResourceManager error: Tried accessing unloaded or nonexistent sound buffer \"" << name << "\".\n";
    return m_soundBuffers.at("error");
}

// Shader functions

// Load a shader and bind it to the map if the key is available, and return a reference to the const loaded shader
const sf::Shader& ResourceManager::loadShader(const std::string& name, const std::string& filename, sf::Shader::Type type)
{
    // If a shader is already loaded at the specified key, return the existing shader
    auto it = m_shaders.find(name);
    if (it != m_shaders.cend())
    {
        return it->second;
    }

    // Otherwise, load the shader
    m_shaders[name];
    if (!m_shaders.at(name).loadFromFile(FileManager::resourcePath() + filename, type))
    {
        std::cerr << "ResourceManager error: Failed to load shader \"" << name << "\" from file \"" << filename << "\".\n";
        m_shaders.erase(name);
        return m_shaders.at("ADDDEFAULTSHADER");
    }
    return m_shaders.at(name);
}

// Remove a Shader from the shader map
void ResourceManager::unloadShader(const std::string& name)
{
    auto it = m_shaders.find(name);
    if (it != m_shaders.cend())
    {
        m_shaders.erase(it);
    }
    else
    {
        std::cerr << "ResourceManager error: Tried unloading already unloaded or nonexistent shader \"" << name << "\".\n";
    }
}

// Return a reference to a const loaded shader
const sf::Shader& ResourceManager::getShader(const std::string& name) const
{
    auto it = m_shaders.find(name);
    if (it != m_shaders.cend())
    {
        return it->second;
    }

    // If the shader is not found, return the default shader
    std::cerr << "ResourceManager error: Tried accessing unloaded or nonexistent shader \"" << name << "\".\n";
    return m_shaders.at("ADDDEFAULTSHADER");
}
