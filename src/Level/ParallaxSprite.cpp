#include "Level/ParallaxSprite.h"
#include <algorithm>
#include "Misc/Utility.h"

ParallaxSprite::ParallaxSprite(const sf::Texture& texture, float parallax)
    : m_sprite(texture)
    , m_parallax(parallax)
    , m_initialScale(1, 1)
{
}

void ParallaxSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void ParallaxSprite::setPosition(const sf::Vector2f& position)
{
    m_initialPosition = position;
    m_sprite.setPosition(position);
}

// Scale the sprite with a vector of factors
void ParallaxSprite::setScale(const sf::Vector2f& scale)
{
    m_initialScale = scale;
}

// Repeat the sprite's texture according to given dimensions
void ParallaxSprite::setTextureRect(const sf::Vector2f& dimensions)
{
    m_sprite.setTextureRect(sf::IntRect(0, 1, dimensions.x, dimensions.y));
    setRelativeOrigin(m_relativeOrigin);
}

// Set the sprite's origin relative to its dimensions (values between 0 and 1)
void ParallaxSprite::setRelativeOrigin(const sf::Vector2f& relativeOrigin)
{
    m_relativeOrigin = relativeOrigin;

    // Clamp
    m_relativeOrigin.x = std::max(m_relativeOrigin.x, 0.0f);
    m_relativeOrigin.x = std::min(m_relativeOrigin.x, 1.0f);
    m_relativeOrigin.y = std::max(m_relativeOrigin.y, 0.0f);
    m_relativeOrigin.y = std::min(m_relativeOrigin.y, 1.0f);

    sf::Vector2f textureRectDimensions = sf::Vector2f(m_sprite.getTextureRect().width - m_sprite.getTextureRect().left,
                                                      m_sprite.getTextureRect().height - m_sprite.getTextureRect().top);
    m_sprite.setOrigin(textureRectDimensions.x * m_relativeOrigin.x, textureRectDimensions.y * m_relativeOrigin.y);
}

// Apply parallax scrolling effect
void ParallaxSprite::update(const Camera& camera)
{
    float newDimensionsScale = (camera.getZoom() - 1) * m_parallax + 1;
    m_sprite.setScale((m_initialScale.x - 1) + (newDimensionsScale - 1.0f) + 1.0f,
                      (m_initialScale.y - 1) + (newDimensionsScale - 1.0f) + 1.0f);

    sf::Vector2f position;
    position.x = (camera.getPosition().x + (m_relativeOrigin.x - 0.5f) * camera.getDimensions().x - m_initialPosition.x) *
                     (1.0 - newDimensionsScale + newDimensionsScale * m_parallax) +
                 m_initialPosition.x;
    position.y = (camera.getPosition().y + (m_relativeOrigin.y - 0.5f) * camera.getDimensions().y - m_initialPosition.y) *
                     (1.0 - newDimensionsScale + newDimensionsScale * m_parallax) +
                 m_initialPosition.y;

    m_sprite.setPosition(position);
}
