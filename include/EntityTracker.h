#ifndef ENTITYTRACKER_H_INCLUDED
#define ENTITYTRACKER_H_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>
#include "Entity.h"

class EntityTracker final : public sf::Drawable
{
private:
    const Entity* m_pTrackedEntity;

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
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void UpdateInfoBox();

public:
    // Constructor
    EntityTracker(const sf::Font& font);

    // Functions
    void Update();

    void ResetTracking();
    void StopTracking();

    void OutputToExcel() const;

    // Setters
    void SetDotRadius(float radius) {m_dot.setRadius(radius);}
    void SetDotColor(sf::Color color) {m_dot.setFillColor(color);}
    void Track(const Entity& trackedEntity) {m_pTrackedEntity = &trackedEntity;}
    void ToggleDotPathVisible() {m_isDotPathVisible = !m_isDotPathVisible;}
    void ToggleInfoBoxVisible() {m_isInfoBoxVisible = !m_isInfoBoxVisible;}

    // Getters
    const sf::Vector2f& GetLastPosition() const {return m_positions.back();}
    float GetDistanceTraveled() const {return m_totalDistanceTraveled;}
    float GetDisplacement() const;
    bool IsTracking() const {return m_pTrackedEntity != nullptr;}
};

#endif // ENTITYTRACKER_H_INCLUDED
