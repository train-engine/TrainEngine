#ifndef STATE_H
#define STATE_H

#include "GameEngine.h"

class State
{
private:
    friend class GameEngine;

    static unsigned int s_orderCounter;
    unsigned int m_orderCreated;

    static sf::Vector2f s_windowDimensions;
    static sf::Vector2f s_windowMousePosition;
    static sf::RectangleShape s_backgroundColorShape;

    sf::Color m_backgroundColor;

    // Static functions
    static void ResizeLayout(const sf::Vector2f& windowDimensions); // Private because State is friends with GameEngine

    // Base functions
    void BaseHandleInput();

    // State functions
    virtual void HandleInput() = 0;
    virtual void Update() = 0;
    virtual void Draw(sf::RenderTarget& rTarget, float lag = 1) = 0;

    virtual void Pause() {} ///< Called automatically before a new State is added above (ceases to be the topmost State)
    virtual void Resume() {} ///< Called automatically after the State above is removed (becomes the topmost State again)

    virtual void OnWindowResize() {} ///< Called automatically on State creation and on window resizing

protected:
    GameEngine& m_rGame;

    struct StateSettings
    {
        bool isCloseable;
        bool canSkipUpdates;
    } m_stateSettings;

    // Constructor and destructor
    explicit State(GameEngine& rGame);
    virtual ~State() {}

    // Functions
    void DrawBackgroundColor(sf::RenderTarget& rTarget, sf::RenderStates states = sf::RenderStates::Default);

    // Setters
    void SetBackgroundColor(sf::Color color) { m_backgroundColor = color; }

    // Getters
    static const sf::Vector2f& GetWindowDimensions() { return s_windowDimensions; }
    static const sf::Vector2f& GetWindowMousePosition() { return s_windowMousePosition; }

    // Utility functions
    sf::Vector2f GetAbsolutePosition(float xRatio, float yRatio) const;
};

#endif // STATE_H
