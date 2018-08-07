#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>

namespace Utility
{

/// Add the given value to the vector only if it does not exist already
template<typename T>
bool AddUniqueValue(std::vector<T>& rVector, const T& value)
{
    if (std::find(rVector.cbegin(), rVector.cend(), value) == rVector.cend())
    {
        rVector.emplace_back(value);
        return true;
    }
    return false;
}

/// Get scale to fill given dimensions while preserving aspect ratio
float GetScaleToFill(const sf::Vector2f& initialDimensions, const sf::Vector2f& fillDimensions);

/// Get scale to fit given dimensions (without overlapping) while preserving aspect ratio
float GetScaleToFit(const sf::Vector2f& initialDimensions, const sf::Vector2f& fitDimensions);

/// Set the given Sprite's scale to fill given dimensions while preserving aspect ratio
void SetSpriteScaleToFill(sf::Sprite& rSprite, const sf::Vector2f& fillDimensions);

/// Set the given Sprite's scale to fit given dimensions (without overlapping) while preserving aspect ratio
void SetSpriteScaleToFit(sf::Sprite& rSprite, const sf::Vector2f& fitDimensions);

}

#endif // UTILITY_H_INCLUDED
