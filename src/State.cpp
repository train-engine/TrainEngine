#include "State.h"
#include <iostream>
#include <math.h>

unsigned int State::s_orderCounter = 0;

sf::Vector2f State::s_windowDimensions(0, 0);
sf::View State::s_defaultView(sf::FloatRect(0, 0, 0, 0));
sf::Vector2f State::s_windowMousePosition(0, 0);
sf::RectangleShape State::s_backgroundColorShape(sf::Vector2f(0, 0));

State::State(GameEngine& rGame)
    : m_rGame(rGame),
      m_backgroundColor(sf::Color::White),
      m_stateSettings({true, false})
{
    m_orderCreated = s_orderCounter++;
}

// Called only by the GameEngine before derived States' HandleInput()
void State::BaseHandleInput()
{
    if (m_rGame.m_inputManager.DetectClosedEvent() && m_stateSettings.isCloseable == true)
    {
        m_rGame.Quit();
    }
    if (m_rGame.m_inputManager.DetectMouseMovedEvent() ||
        m_rGame.m_inputManager.DetectTouchBeganEvent() || m_rGame.m_inputManager.DetectTouchMovedEvent() ||
        m_rGame.m_inputManager.DetectResizedEvent())
    {
        s_windowMousePosition = static_cast<sf::Vector2f>(m_rGame.m_inputManager.GetWindowMousePosition());
    }

    // Debug
    if (m_rGame.m_inputManager.ControlKeyHeld() && m_rGame.m_inputManager.KeyDown(sf::Keyboard::Num1))
    {
        m_rGame.ToggleDebugOverlay();
    }
    if (m_rGame.m_inputManager.ControlKeyHeld() && m_rGame.m_inputManager.KeyDown(sf::Keyboard::Num2))
    {
        // Debug mode (slow tick velocity)
        if (static_cast<unsigned int>(m_rGame.GetTargetUps()) == 60)
        {
            std::cout << "Debug mode - Set UPS to 5.\n";
            m_rGame.SetTargetUps(1);
        }
        // Play mode
        else
        {
            std::cout << "Play mode - Set UPS to 60.\n";
            m_rGame.SetTargetUps(60);
        }
    }
}

// Called only by the GameEngine before derived States' Resume()
void State::BaseResume()
{

}

// Called only by the GameEngine when the window is resized
void State::BaseOnWindowResize(const sf::Vector2f &windowDimensions)
{
    s_windowDimensions = static_cast<sf::Vector2f>(windowDimensions);
    s_defaultView.reset(sf::FloatRect(0, 0, windowDimensions.x, windowDimensions.y));
    s_backgroundColorShape.setPosition(0, 0);
    s_backgroundColorShape.setSize(windowDimensions);
}

// Draw the background color
void State::DrawBackgroundColor(sf::RenderTarget& rTarget, sf::RenderStates states)
{
    s_backgroundColorShape.setFillColor(m_backgroundColor);
    rTarget.draw(s_backgroundColorShape, states);
}

// Calculates position from ratios relative to the window's dimensions
sf::Vector2f State::GetAbsolutePosition(float xRatio, float yRatio) const
{
    return sf::Vector2f(GetWindowDimensions().x * xRatio, GetWindowDimensions().y * yRatio);
}
