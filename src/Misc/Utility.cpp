#include "Misc/Utility.h"
#include <cmath>
#include <SFML/Graphics.hpp>

namespace Utility
{
    /// Get scale to fill given dimensions while preserving aspect ratio
    float GetScaleToFill(const sf::Vector2f& initialDimensions, const sf::Vector2f& fillDimensions)
    {
        return std::fmax(fillDimensions.x / initialDimensions.x, fillDimensions.y / initialDimensions.y);
    }

    /// Get scale to fit given dimensions (without overlapping) while preserving aspect ratio
    float GetScaleToFit(const sf::Vector2f& initialDimensions, const sf::Vector2f& fitDimensions)
    {
        return std::fmin(fitDimensions.x / initialDimensions.x, fitDimensions.y / initialDimensions.y);
    }

    /// Set the given Sprite's scale to fill given dimensions while preserving aspect ratio
    void SetSpriteScaleToFill(sf::Sprite& rSprite, const sf::Vector2f& fillDimensions)
    {
        float scale = GetScaleToFill(static_cast<sf::Vector2f>(rSprite.getTexture()->getSize()), fillDimensions);
        rSprite.setScale(scale, scale);
    }

    /// Set the given Sprite's scale to fit given dimensions (without overlapping) while preserving aspect ratio
    void SetSpriteScaleToFit(sf::Sprite& rSprite, const sf::Vector2f& fitDimensions)
    {
        float scale = GetScaleToFit(static_cast<sf::Vector2f>(rSprite.getTexture()->getSize()), fitDimensions);
        rSprite.setScale(scale, scale);
    }
} // namespace Utility
