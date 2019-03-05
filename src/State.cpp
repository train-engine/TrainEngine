#include "State.h"
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
void State::ResizeLayout(const sf::Vector2f& windowDimensions)
{
    s_windowDimensions = static_cast<sf::Vector2f>(windowDimensions);
    s_backgroundColorShape.setSize(windowDimensions);
}

// Called only by the GameEngine before derived States' HandleInput()
void State::BaseHandleInput()
{
    // Handle clicking on the close button
    if (m_rGame.inputManager.DetectedClosedEvent() && m_stateSettings.isCloseable == true)
    {
        m_rGame.Quit();
    }

    // Update mouse position if necessary
    if (m_rGame.inputManager.DetectedMouseMovedEvent() || // Mouse events
        m_rGame.inputManager.DetectedTouchBeganEvent() || m_rGame.inputManager.DetectedTouchMovedEvent() || // Touch events
        m_rGame.inputManager.DetectedResizedEvent()) // Resize event
    {
        s_windowMousePosition = static_cast<sf::Vector2f>(m_rGame.inputManager.GetWindowMousePosition());
    }

    // Debug overlay
    if (m_rGame.inputManager.IsControlKeyHeld() && m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Num1))
    {
        m_rGame.ToggleDebugOverlay();
    }

    // Change UPS
    if (m_rGame.inputManager.IsControlKeyHeld() && m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Num2))
    {
        // Debug mode (slow tick velocity)
        if (static_cast<unsigned int>(m_rGame.GetTargetUps()) == 60)
        {
            std::cout << "Debug mode - Set UPS to 5.\n";
            m_rGame.SetTargetUps(5);
        }
        // Play mode
        else
        {
            std::cout << "Play mode - Set UPS to 60.\n";
            m_rGame.SetTargetUps(60);
        }
    }
}

// Draw the background color
void State::DrawBackgroundColor(sf::RenderTarget& rTarget, sf::RenderStates states)
{
    s_backgroundColorShape.setFillColor(m_stateSettings.backgroundColor);
    rTarget.draw(s_backgroundColorShape, states);
}

// Calculate position from ratios relative to the window's dimensions
sf::Vector2f State::GetAbsolutePosition(float xRatio, float yRatio) const
{
    return sf::Vector2f(GetWindowDimensions().x * xRatio, GetWindowDimensions().y * yRatio);
}
