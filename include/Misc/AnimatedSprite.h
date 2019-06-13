#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <SFML/Graphics.hpp>

class AnimatedSprite final : public sf::Drawable
{
private:
    sf::Sprite m_sprite;

    sf::Vector2u m_frameDimensions;
    sf::Vector2u m_spriteSheetDimensions;

    float m_tickCounter;
    float m_frameDuration;
    unsigned int m_currentFrameIndex;
    unsigned int m_totalFrames;
    bool m_isLoopingEnabled;
    bool m_isFlipped;
    bool m_isPlaying;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setTextureRect();

public:
    // Constructor
    AnimatedSprite(const sf::Texture& texture, const sf::Vector2u& frameDimensions, unsigned int frameCount);

    // Functions
    void update();
    void play() { m_isPlaying = true; }
    void pause() { m_isPlaying = false; }
    void restart();
    void stop();

    // Setters
    void setPosition(const sf::Vector2f& position);
    void setRotation(float angle);
    void setFrameDuration(float frameDuration);
    void setLoopingEnabled(bool isLoopingEnabled);
    void setFlipped(bool isFlipped);

    // Getters
    bool isPlaying() const { return m_isPlaying; }
};

#endif // ANIMATEDSPRITE_H
