#ifndef PARALLAXSPRITE_H
#define PARALLAXSPRITE_H

#include <string>
#include <SFML/Graphics.hpp>
#include "Level/Camera.h"

// Notes for m_parallax values:
// m_parallax = 1 : sprite does not move
// m_parallax between 0 and 1 : parallax effect in the background
// m_parallax = 0 : sprite moves in sync with the view
// m_parallax < 1 : parallax effect in front of the foreground

class ParallaxSprite final : public sf::Drawable
{
private:
    sf::Sprite m_sprite;

    float m_parallax;
    sf::Vector2f m_initialPosition;
    sf::Vector2f m_initialScale;
    sf::Vector2f m_relativeOrigin;

    std::string m_resourceName;
    std::string m_positionModeString;
    std::string m_repeatTextureString;
    std::string m_scaleString;
    std::string m_offsetString;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;

public:
    // Constructor
    ParallaxSprite(const sf::Texture& texture, float parallax);

    // Functions
    void update(const Camera& camera);

    // Setters
    void setParallax(float parallax) { m_parallax = parallax; }
    void setPosition(const sf::Vector2f& position);
    void setScale(const sf::Vector2f& scale);
    void setTextureRect(const sf::Vector2f& dimensions);
    void setRelativeOrigin(const sf::Vector2f& relativeOrigin);

    void setResourceName(const std::string& resourceName) { m_resourceName = resourceName; }
    void setPositionModeString(const std::string& positionModeString) { m_positionModeString = positionModeString; }
    void setRepeatTextureString(const std::string& repeatTextureString) { m_repeatTextureString = repeatTextureString; }
    void setScaleString(const std::string& scaleString) { m_scaleString = scaleString; }
    void setOffsetString(const std::string& offsetString) { m_offsetString = offsetString; }

    // Getters
    const sf::Texture* getTexture() const { return m_sprite.getTexture(); }
    float getParallax() const { return m_parallax; }
    const sf::Vector2f& getPosition() const { return m_initialPosition; }
    const sf::Vector2f& getScale() const { return m_initialScale; }
    const sf::Vector2f& getRelativeOrigin() const { return m_relativeOrigin; }

    const std::string& getResourceName() const { return m_resourceName; }
    const std::string& getPositionModeString() const { return m_positionModeString; }
    const std::string& getRepeatTextureString() const { return m_repeatTextureString; }
    const std::string& getScaleString() const { return m_scaleString; }
    const std::string& getOffsetString() const { return m_offsetString; }
};

#endif // PARALLAXSPRITE_H
