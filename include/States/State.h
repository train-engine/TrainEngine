#ifndef STATE_H
#define STATE_H

#include "Core/GameEngine.h"

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
    static void resizeLayout(const sf::Vector2f& windowDimensions); // Private because State is friends with GameEngine

    // Base functions
    void baseHandleInput();

    // State functions
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderTarget& rTarget, float lag = 1) = 0;

    virtual void pause() {} ///< Called automatically before a new State is added above (ceases to be the topmost State)
    virtual void resume() {} ///< Called automatically after the State above is removed (becomes the topmost State again)

    virtual void onWindowResize() {} ///< Called automatically on State creation and on window resizing

protected:
    GameEngine& m_rGame;

    struct StateSettings
    {
        bool isCloseable;
        bool canSkipUpdates;
        sf::Color backgroundColor;
    } m_stateSettings;

    // Constructor and destructor
    explicit State(GameEngine& rGame);
    virtual ~State() {}

    // Functions
    void drawBackgroundColor(sf::RenderTarget& rTarget, sf::RenderStates states = sf::RenderStates::Default);

    // Getters
    static const sf::Vector2f& getWindowDimensions() { return s_windowDimensions; }
    static const sf::Vector2f& getWindowMousePosition() { return s_windowMousePosition; }

    // Utility functions
    sf::Vector2f getAbsolutePosition(float xRatio, float yRatio) const;
};

#endif // STATE_H
