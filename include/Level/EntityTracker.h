#ifndef ENTITYTRACKER_H
#define ENTITYTRACKER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Level/Entity.h"

class EntityTracker final : public sf::Drawable
{
private:
    const Entity* m_trackedEntity;

    mutable sf::CircleShape m_dot;

    sf::RectangleShape m_textContainer;
    sf::Text m_lastPositionText;
    sf::Text m_lastVelocityText;
    sf::Text m_distanceTraveledText;
    sf::Text m_displacementText;
    sf::Text m_positionsCountText;

    std::vector<sf::Vector2f> m_positions;
    float m_totalDistanceTraveled;

    bool m_isDotPathVisible;
    bool m_isInfoBoxVisible;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateInfoBox();

public:
    // Constructor
    explicit EntityTracker(const sf::Font& font);

    // Functions
    void update();

    void resetTracking();
    void stopTracking();

    void outputToExcel() const;

    // Setters
    void setDotRadius(float radius) { m_dot.setRadius(radius); }
    void setDotColor(sf::Color color) { m_dot.setFillColor(color); }
    void track(const Entity& trackedEntity) { m_trackedEntity = &trackedEntity; }
    void toggleDotPathVisible() { m_isDotPathVisible = !m_isDotPathVisible; }
    void toggleInfoBoxVisible() { m_isInfoBoxVisible = !m_isInfoBoxVisible; }

    // Getters
    const sf::Vector2f& getLastPosition() const { return m_positions.back(); }
    float getDistanceTraveled() const { return m_totalDistanceTraveled; }
    float getDisplacement() const;
    bool isTracking() const { return m_trackedEntity != nullptr; }
};

#endif // ENTITYTRACKER_H
