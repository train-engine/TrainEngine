#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <SFML/Graphics.hpp>

namespace Utility
{
    /// Get scale to fill given dimensions while preserving aspect ratio
    template<typename T>
    float getScaleToFill(const sf::Vector2<T>& initialDimensions, const sf::Vector2<T>& fillDimensions)
    {
        return std::max(static_cast<float>(fillDimensions.x) / initialDimensions.x, static_cast<float>(fillDimensions.y) / initialDimensions.y);
    }

    /// Get scale to fit given dimensions (without overlapping) while preserving aspect ratio
    template<typename T>
    float getScaleToFit(const sf::Vector2<T>& initialDimensions, const sf::Vector2<T>& fitDimensions)
    {
        return std::min(static_cast<float>(fitDimensions.x) / initialDimensions.x, static_cast<float>(fitDimensions.y) / initialDimensions.y);
    }

    void setSpriteScaleToFill(sf::Sprite& sprite, const sf::Vector2f& fillDimensions);
    void setSpriteScaleToFit(sf::Sprite& sprite, const sf::Vector2f& fitDimensions);
} // namespace Utility

#endif // UTILITY_H
