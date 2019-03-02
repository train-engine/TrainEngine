#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <SFML/Graphics.hpp>

namespace Utility
{
    /// Add the given value to the vector only if it does not exist already
    template<typename T>
    bool AddIfUnique(std::vector<T>& rVector, const T& value)
    {
        if (std::find(rVector.cbegin(), rVector.cend(), value) == rVector.cend())
        {
            rVector.emplace_back(value);
            return true;
        }
        return false;
    }
    
    float GetScaleToFill(const sf::Vector2f& initialDimensions, const sf::Vector2f& fillDimensions);
    float GetScaleToFit(const sf::Vector2f& initialDimensions, const sf::Vector2f& fitDimensions);
    
    void SetSpriteScaleToFill(sf::Sprite& rSprite, const sf::Vector2f& fillDimensions);
    void SetSpriteScaleToFit(sf::Sprite& rSprite, const sf::Vector2f& fitDimensions);
}

#endif // UTILITY_H
