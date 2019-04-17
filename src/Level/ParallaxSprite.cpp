#include "Level/ParallaxSprite.h"
#include <algorithm>
#include "Misc/Utility.h"

ParallaxSprite::ParallaxSprite(const sf::Texture& texture, float parallax)
    : m_sprite(texture)
    , m_parallax(parallax)
    , m_initialScale(1, 1)
{
}

void ParallaxSprite::draw(sf::RenderTarget& rTarget, sf::RenderStates states) const
{
    rTarget.draw(m_sprite, states);
}

void ParallaxSprite::SetPosition(const sf::Vector2f& position)
{
    m_initialPosition = position;
    m_sprite.setPosition(position);
}

// Scale the sprite with a vector of factors
void ParallaxSprite::SetScale(const sf::Vector2f& scale)
{
    m_initialScale = scale;
}

// Repeat the sprite's texture according to given dimensions
void ParallaxSprite::SetTextureRect(const sf::Vector2f& dimensions)
{
    m_sprite.setTextureRect(sf::IntRect(0, 1, dimensions.x, dimensions.y));
    SetRelativeOrigin(m_relativeOrigin);
}

// Set the sprite's origin relative to its dimensions (values between 0 and 1)
void ParallaxSprite::SetRelativeOrigin(const sf::Vector2f& relativeOrigin)
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
void ParallaxSprite::Update(const Camera& camera)
{
    float newDimensionsScale = (camera.GetZoom() - 1) * m_parallax + 1;
    m_sprite.setScale((m_initialScale.x - 1) + (newDimensionsScale - 1.0f) + 1.0f,
                      (m_initialScale.y - 1) + (newDimensionsScale - 1.0f) + 1.0f);

    sf::Vector2f position;
    position.x = (camera.GetPosition().x + (m_relativeOrigin.x - 0.5) * camera.GetDimensions().x - m_initialPosition.x) *
                     (1.0f - newDimensionsScale + newDimensionsScale * m_parallax) +
                 m_initialPosition.x;
    position.y = (camera.GetPosition().y + (m_relativeOrigin.y - 0.5) * camera.GetDimensions().y - m_initialPosition.y) *
                     (1.0f - newDimensionsScale + newDimensionsScale * m_parallax) +
                 m_initialPosition.y;

    m_sprite.setPosition(position);
}
