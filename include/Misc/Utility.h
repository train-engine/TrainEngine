#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Graphics.hpp>

namespace Utility
{
    float GetScaleToFill(const sf::Vector2f& initialDimensions, const sf::Vector2f& fillDimensions);
    float GetScaleToFit(const sf::Vector2f& initialDimensions, const sf::Vector2f& fitDimensions);

    void SetSpriteScaleToFill(sf::Sprite& rSprite, const sf::Vector2f& fillDimensions);
    void SetSpriteScaleToFit(sf::Sprite& rSprite, const sf::Vector2f& fitDimensions);
} // namespace Utility

#endif // UTILITY_H
