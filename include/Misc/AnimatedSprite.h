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
    bool m_isFlippable;
    bool m_isFlipped;
    bool m_isPlaying;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void SetTextureRect();

public:
    // Constructor
    AnimatedSprite(const sf::Texture& texture, const sf::Vector2u& frameDimensions, unsigned int frameCount, float frameDuration = 5,
                   bool isLoopingEnabled = true, bool isFlippable = true);

    // Functions
    void Update();

    // Setters
    void SetPosition(const sf::Vector2f& position);
    void SetRotation(float angle);
    void SetFrameDuration(float frameDuration);
    void SetIsFlipped(bool isFlipped) { m_isFlipped = isFlipped; }
    void Play() { m_isPlaying = true; }
    void Pause() { m_isPlaying = false; }
    void Restart();
    void Stop();

    // Getters
    bool IsPlaying() const { return m_isPlaying; }
};

#endif // ANIMATEDSPRITE_H
