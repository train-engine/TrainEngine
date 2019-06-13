#include "Misc/Utility.h"
#include <cmath>
#include <SFML/Graphics.hpp>

namespace Utility
{
    /// Get scale to fill given dimensions while preserving aspect ratio
    float getScaleToFill(const sf::Vector2f& initialDimensions, const sf::Vector2f& fillDimensions)
    {
        return std::fmax(fillDimensions.x / initialDimensions.x, fillDimensions.y / initialDimensions.y);
    }

    /// Get scale to fit given dimensions (without overlapping) while preserving aspect ratio
    float getScaleToFit(const sf::Vector2f& initialDimensions, const sf::Vector2f& fitDimensions)
    {
        return std::fmin(fitDimensions.x / initialDimensions.x, fitDimensions.y / initialDimensions.y);
    }

    /// Set the given Sprite's scale to fill given dimensions while preserving aspect ratio
    void setSpriteScaleToFill(sf::Sprite& sprite, const sf::Vector2f& fillDimensions)
    {
        float scale = getScaleToFill(static_cast<sf::Vector2f>(sprite.getTexture()->getSize()), fillDimensions);
        sprite.setScale(scale, scale);
    }

    /// Set the given Sprite's scale to fit given dimensions (without overlapping) while preserving aspect ratio
    void setSpriteScaleToFit(sf::Sprite& sprite, const sf::Vector2f& fitDimensions)
    {
        float scale = getScaleToFit(static_cast<sf::Vector2f>(sprite.getTexture()->getSize()), fitDimensions);
        sprite.setScale(scale, scale);
    }
} // namespace Utility
