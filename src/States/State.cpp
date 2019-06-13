#include "States/State.h"
#include <iostream>

unsigned int State::s_orderCounter = 0;

sf::Vector2f State::s_windowDimensions(0, 0);
sf::Vector2f State::s_windowMousePosition(0, 0);
sf::RectangleShape State::s_backgroundColorShape(sf::Vector2f(0, 0));

State::State(GameEngine& rGame)
    : m_orderCreated(s_orderCounter++)
    , m_rGame(rGame)
    , m_stateSettings{true, false, sf::Color::White}
{
}

// Called only by the GameEngine when the window is resized
void State::resizeLayout(const sf::Vector2f& windowDimensions)
{
    s_windowDimensions = windowDimensions;
    s_backgroundColorShape.setSize(windowDimensions);
}

// Called only by the GameEngine before derived States' HandleInput()
void State::baseHandleInput()
{
    // Handle clicking on the close button
    if (m_rGame.inputManager.detectedClosedEvent() && m_stateSettings.isCloseable == true)
    {
        m_rGame.quit();
    }

    // Update mouse position if necessary
    if (m_rGame.inputManager.detectedMouseMovedEvent() || // Mouse events
        m_rGame.inputManager.detectedTouchBeganEvent() || m_rGame.inputManager.detectedTouchMovedEvent() || // Touch events
        m_rGame.inputManager.detectedResizedEvent()) // Resize event
    {
        s_windowMousePosition = static_cast<sf::Vector2f>(m_rGame.inputManager.getWindowMousePosition());
    }

    // Debug overlay
    if (m_rGame.inputManager.isControlKeyHeld() && m_rGame.inputManager.isKeyDescending(sf::Keyboard::Num1))
    {
        m_rGame.toggleDebugOverlay();
    }

    // Change UPS
    if (m_rGame.inputManager.isControlKeyHeld() && m_rGame.inputManager.isKeyDescending(sf::Keyboard::Num2))
    {
        // Debug mode (slow tick velocity)
        if (static_cast<unsigned int>(m_rGame.getTargetUps()) == 60)
        {
            std::cout << "Debug mode - Set UPS to 5.\n";
            m_rGame.setTargetUps(5);
        }
        // Play mode
        else
        {
            std::cout << "Play mode - Set UPS to 60.\n";
            m_rGame.setTargetUps(60);
        }
    }
}

// Draw the background color
void State::drawBackgroundColor(sf::RenderTarget& rTarget, sf::RenderStates states)
{
    s_backgroundColorShape.setFillColor(m_stateSettings.backgroundColor);
    rTarget.draw(s_backgroundColorShape, states);
}

// Calculate position from ratios relative to the window's dimensions
sf::Vector2f State::getAbsolutePosition(float xRatio, float yRatio) const
{
    return sf::Vector2f(getWindowDimensions().x * xRatio, getWindowDimensions().y * yRatio);
}
