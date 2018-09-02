#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include "GameEngine.h"

struct StateSettings
{
    bool isCloseable;
    bool canSkipUpdates;
};

class State
{
private:
    friend class GameEngine;

    static unsigned int s_orderCounter;
    unsigned int m_orderCreated;

    sf::Vector2f m_windowDimensions;
    sf::View m_defaultView;
    sf::Vector2f m_windowMousePosition;

    sf::RectangleShape m_backgroundColor;

    // Base functions
    void BaseHandleInput();
    void BaseResume();
    void BaseOnWindowResize();

    // State functions
    virtual void HandleInput() = 0;
    virtual void Update() = 0;
    virtual void Draw(sf::RenderTarget& rTarget, float lag = 1) = 0;

    virtual void Pause() {}
    virtual void Resume() {}

    virtual void OnWindowResize() {}

protected:
    GameEngine& m_rGame;

    StateSettings m_stateSettings;

    // Constructor and destructor
    State(GameEngine& rGame);
    virtual ~State() {}

    // Functions
    void DrawBackgroundColor(sf::RenderTarget& rTarget, sf::RenderStates states);

    // Setters
    void SetBackgroundColor(sf::Color color) {m_backgroundColor.setFillColor(color);}

    // Getters
    const sf::Vector2f& GetWindowDimensions() const {return m_windowDimensions;}
    const sf::View& GetDefaultView() const {return m_defaultView;}
    const sf::Vector2f& GetWindowMousePosition() const {return m_windowMousePosition;}

    // Utility functions
    sf::Vector2f GetAbsolutePosition(float xRatio, float yRatio) const;
};

#endif // STATE_H_INCLUDED
