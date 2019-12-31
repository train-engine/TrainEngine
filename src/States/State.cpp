#include "States/State.h"
#include <iostream>

unsigned int State::s_orderCounter = 0;

sf::Vector2f State::s_windowDimensions(0, 0);
sf::Vector2f State::s_windowMousePosition(0, 0);
sf::RectangleShape State::s_backgroundColorShape(sf::Vector2f(0, 0));

State::State(GameEngine& game)
    : m_orderCreated(s_orderCounter++)
    , m_game(game)
    , m_stateSettings{true, false, sf::Color::White}
{
}

// Called only by the GameEngine when the window is resized
void State::resizeLayout(const sf::Vector2f& windowDimensions)
{
    s_windowDimensions = windowDimensions;
    s_backgroundColorShape.setSize(windowDimensions);
}

// Called only by the GameEngine before derived States' handleInput()
void State::baseHandleInput()
{
    // Handle clicking on the close button
    if (m_game.inputManager.detectedClosedEvent() && m_stateSettings.isCloseable == true)
    {
        m_game.quit();
    }

    // Update mouse position if necessary
    if (m_game.inputManager.detectedMouseMovedEvent() || // Mouse events
        m_game.inputManager.detectedTouchBeganEvent() || m_game.inputManager.detectedTouchMovedEvent() || // Touch events
        m_game.inputManager.detectedResizedEvent()) // Resize event
    {
        s_windowMousePosition = static_cast<sf::Vector2f>(m_game.inputManager.getWindowMousePosition());
    }

    // Debug overlay
    if (m_game.inputManager.isControlKeyHeld() && m_game.inputManager.isKeyDescending(sf::Keyboard::Num1))
    {
        m_game.toggleDebugOverlay();
    }

    // Change UPS
    if (m_game.inputManager.isControlKeyHeld() && m_game.inputManager.isKeyDescending(sf::Keyboard::Num2))
    {
        // Debug mode (slow tick velocity)
        if (static_cast<unsigned int>(m_game.getTargetUps()) == 60)
        {
            std::cout << "Debug mode - Set UPS to 5.\n";
            m_game.setTargetUps(5);
        }
        // Play mode
        else
        {
            std::cout << "Play mode - Set UPS to 60.\n";
            m_game.setTargetUps(60);
        }
    }
}

// Draw the background color
void State::drawBackgroundColor(sf::RenderTarget& target, sf::RenderStates states)
{
    s_backgroundColorShape.setFillColor(m_stateSettings.backgroundColor);
    target.draw(s_backgroundColorShape, states);
}

// Calculate position from ratios relative to the window's dimensions
sf::Vector2f State::getAbsolutePosition(float xRatio, float yRatio)
{
    return sf::Vector2f(getWindowDimensions().x * xRatio, getWindowDimensions().y * yRatio);
}
