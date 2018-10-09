#include "Player.h"

Player::Player(Map& rMap, std::vector<Entity*>& rEntities, const InputManager& inputManager,
               const sf::Vector2f& position)
    : Entity(rMap, rEntities, EntityType::Player, position, sf::Vector2f(40, 80), sf::Vector2f(10, 64), 1, 3, 18, true, true, true),
      m_inputManager(inputManager)
{

}

void Player::HandleInput()
{
    // Left/Right
    if ((m_inputManager.IsKeyHeld(sf::Keyboard::A) && !m_inputManager.IsKeyHeld(sf::Keyboard::D)) ||
        (m_inputManager.IsTouchHeld() && m_inputManager.GetWindowMousePosition().x < static_cast<int>(m_inputManager.GetWindowDimensions().x) / 2) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::X) < -50)
    {
        SetPressingLeft(true);
    }
    else
    {
        SetPressingLeft(false);
    }
    if ((m_inputManager.IsKeyHeld(sf::Keyboard::D) && !m_inputManager.IsKeyHeld(sf::Keyboard::A)) ||
        (m_inputManager.IsTouchHeld() && m_inputManager.GetWindowMousePosition().x > static_cast<int>(m_inputManager.GetWindowDimensions().x) / 2) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::X) > 50)
    {
        SetPressingRight(true);
    }
    else
    {
        SetPressingRight(false);
    }

    // Up/Down
    if (m_inputManager.IsKeyHeld(sf::Keyboard::W) ||
        m_inputManager.IsJoystickButtonDown(0, 0) || m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Y) < -50)
    {
        SetPressingUp(true);
    }
    else
    {
        SetPressingUp(false);
    }
    if (m_inputManager.IsKeyHeld(sf::Keyboard::S) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Y) > 50)
    {
        SetPressingDown(true);
    }
    else
    {
        SetPressingDown(false);
    }

    // Shift
    if (m_inputManager.IsKeyHeld(sf::Keyboard::LShift) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Axis::Z) > 0)
    {
        SetPressingShift(true);
    }
    else
    {
        SetPressingShift(false);
    }
}
