#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Graphics.hpp>

namespace Utility
{
    float getScaleToFill(const sf::Vector2f& initialDimensions, const sf::Vector2f& fillDimensions);
    float getScaleToFit(const sf::Vector2f& initialDimensions, const sf::Vector2f& fitDimensions);

    void setSpriteScaleToFill(sf::Sprite& rSprite, const sf::Vector2f& fillDimensions);
    void setSpriteScaleToFit(sf::Sprite& rSprite, const sf::Vector2f& fitDimensions);
} // namespace Utility

#endif // UTILITY_H
