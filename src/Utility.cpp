#include <math.h>
#include <SFML/Graphics.hpp>

namespace Utility
{

float GetScaleToFill(const sf::Vector2f& initialDimensions, const sf::Vector2f& fillDimensions)
{
    return fmax(fillDimensions.x / initialDimensions.x, fillDimensions.y / initialDimensions.y);
}

float GetScaleToFit(const sf::Vector2f& initialDimensions, const sf::Vector2f& fitDimensions)
{
    return fmin(fitDimensions.x / initialDimensions.x, fitDimensions.y / initialDimensions.y);
}

void SetSpriteScaleToFill(sf::Sprite& rSprite, const sf::Vector2f& fillDimensions)
{
    float scale = GetScaleToFill(static_cast<sf::Vector2f>(rSprite.getTexture()->getSize()), fillDimensions);
    rSprite.setScale(scale, scale);
}

void SetSpriteScaleToFit(sf::Sprite& rSprite, const sf::Vector2f& fitDimensions)
{
    float scale = GetScaleToFit(static_cast<sf::Vector2f>(rSprite.getTexture()->getSize()), fitDimensions);
    rSprite.setScale(scale, scale);
}

}
