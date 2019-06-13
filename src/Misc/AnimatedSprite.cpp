#include "Misc/AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const sf::Texture& texture, const sf::Vector2u& frameDimensions, unsigned int frameCount)
    : m_sprite(texture)
    , m_frameDimensions(frameDimensions)
    , m_spriteSheetDimensions(texture.getSize())
    , m_tickCounter(0)
    , m_frameDuration(1)
    , m_currentFrameIndex(0)
    , m_totalFrames(frameCount)
    , m_isLoopingEnabled(true)
    , m_isFlipped(false)
    , m_isPlaying(false)
{
    setTextureRect();
    m_sprite.setOrigin(static_cast<sf::Vector2f>(m_frameDimensions) / 2.0f);
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void AnimatedSprite::setTextureRect()
{
    unsigned int framesPerRow = m_spriteSheetDimensions.x / m_frameDimensions.x;
    if (framesPerRow == 0) // If the Texture's dimensions are too small for the requested frame dimensions
    {
        return;
    }

    unsigned int xPosition = m_currentFrameIndex % framesPerRow;
    unsigned int yPosition = m_currentFrameIndex / framesPerRow;
    m_sprite.setTextureRect(sf::IntRect(static_cast<int>(xPosition * m_frameDimensions.x),
                                        static_cast<int>(yPosition * m_frameDimensions.y),
                                        m_frameDimensions.x,
                                        m_frameDimensions.y));
}

void AnimatedSprite::update()
{
    if (m_isPlaying == false)
    {
        return;
    }

    if (m_tickCounter >= m_frameDuration)
    {
        m_tickCounter -= m_frameDuration;
        if (m_currentFrameIndex < m_totalFrames - 1)
        {
            m_currentFrameIndex++;
            setTextureRect();
        }
        else if (m_currentFrameIndex == m_totalFrames - 1)
        {
            if (m_isLoopingEnabled == true)
            {
                m_currentFrameIndex = 0;
                setTextureRect();
            }
            else
            {
                m_isPlaying = false; // If the animation is set to not repeat
            }
        }
    }
    m_tickCounter++;
}

void AnimatedSprite::restart()
{
    m_isPlaying = true;
    m_tickCounter = 0;
    m_currentFrameIndex = 0;
}

void AnimatedSprite::stop()
{
    m_isPlaying = false;
    m_tickCounter = 0;
    m_currentFrameIndex = 0;
}

void AnimatedSprite::setPosition(const sf::Vector2f& position)
{
    m_sprite.setPosition(position);
}

void AnimatedSprite::setRotation(float angle)
{
    m_sprite.setRotation(angle);
}

void AnimatedSprite::setFrameDuration(float frameDuration)
{
    m_frameDuration = frameDuration;
}

void AnimatedSprite::setLoopingEnabled(bool isLoopingEnabled)
{
    m_isLoopingEnabled = isLoopingEnabled;
}

void AnimatedSprite::setFlipped(bool isFlipped)
{
    m_isFlipped = isFlipped;

    if (m_isFlipped == true && m_sprite.getScale().x > 0)
    {
        m_sprite.setScale(-1, 1);
    }
    else if (m_isFlipped == false && m_sprite.getScale().x < 0)
    {
        m_sprite.setScale(1, 1);
    }
}
