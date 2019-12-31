#include "Misc/Utility.h"
#include <SFML/Graphics.hpp>

namespace Utility
{
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
