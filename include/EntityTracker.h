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

    bool m_displayDots;
    bool m_displayInfoBox;

    // Functions
    void UpdateInfoBox();

public:
    // Constructor
    EntityTracker(const sf::Font& font);

    // Functions
    void Update();
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;

    void ResetTracking();
    void StopTracking();

    void OutputToExcel() const;

    // Setters
    void SetDotRadius(float radius) {m_dot.setRadius(radius);}
    void SetDotColor(sf::Color color) {m_dot.setFillColor(color);}
    void Track(const Entity& trackedEntity) {m_pTrackedEntity = &trackedEntity;}
    void ToggleDotsDisplay() {m_displayDots = !m_displayDots;}
    void ToggleInfoBoxDisplay() {m_displayInfoBox = !m_displayInfoBox;}

    // Getters
    const sf::Vector2f& GetLastPosition() const {return m_positions.back();}
    float GetDistanceTraveled() const {return m_totalDistanceTraveled;}
    float GetDisplacement() const;
    bool IsTracking() const {return m_pTrackedEntity != nullptr;}
};

#endif // ENTITYTRACKER_H_INCLUDED
