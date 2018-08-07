#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class ResourceManager
{
private:   
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::Font> m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Shader> m_shaders;

    // Functions
    bool LoadInitialResources();
    
public:
    // Constructor and destructor
    ResourceManager();
    ~ResourceManager();

    // Functions
    
    // Texture functions
    const sf::Texture& LoadTexture(const std::string& name, const std::string& filename, const sf::IntRect& textureRect = {});
    void UnloadTexture(const std::string& name);
    const sf::Texture& GetTexture(const std::string& name) const;
    void SetTextureRepeated(const std::string& name, bool isRepeated);
    void SetTextureSmooth(const std::string& name, bool isSmooth);

    // Font functions
    const sf::Font& LoadFont(const std::string& name, const std::string& filename);
    void UnloadFont(const std::string& name);
    const sf::Font& GetFont(const std::string& name) const;

    // SoundBuffer functions
    const sf::SoundBuffer& LoadSoundBuffer(const std::string& name, const std::string& filename);
    void UnloadSoundBuffer(const std::string& name);
    const sf::SoundBuffer& GetSoundBuffer(const std::string& name) const;

    // Shader functions
    const sf::Shader& LoadShader(const std::string& name, const std::string& filename, sf::Shader::Type type);
    void UnloadShader(const std::string& name);
    const sf::Shader& GetShader(const std::string& name) const;
};

#endif // RESOURCEMANAGER_H_INCLUDED
