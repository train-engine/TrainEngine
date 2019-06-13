#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class ResourceManager final
{
private:
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::Font> m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Shader> m_shaders;

    // Functions
    bool loadInitialResources();

public:
    // Constructor and destructor
    ResourceManager();
    ~ResourceManager();

    // Functions

    // Texture functions
    const sf::Texture& loadTexture(const std::string& name, const std::string& filename, const sf::IntRect& textureRect = {});
    void unloadTexture(const std::string& name);
    const sf::Texture& getTexture(const std::string& name) const;
    void setTextureRepeated(const std::string& name, bool isRepeated);
    void setTextureSmooth(const std::string& name, bool isSmooth);

    // Font functions
    const sf::Font& loadFont(const std::string& name, const std::string& filename);
    void unloadFont(const std::string& name);
    const sf::Font& getFont(const std::string& name) const;

    // SoundBuffer functions
    const sf::SoundBuffer& loadSoundBuffer(const std::string& name, const std::string& filename);
    void unloadSoundBuffer(const std::string& name);
    const sf::SoundBuffer& getSoundBuffer(const std::string& name) const;

    // Shader functions
    const sf::Shader& loadShader(const std::string& name, const std::string& filename, sf::Shader::Type type);
    void unloadShader(const std::string& name);
    const sf::Shader& getShader(const std::string& name) const;
};

#endif // RESOURCEMANAGER_H
