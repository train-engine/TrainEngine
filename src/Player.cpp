#include "Player.h"

Player::Player(Map& rMap, std::vector<Entity*>& rEntities, const InputManager& inputManager,
               const sf::Vector2f& position)
    : CharacterEntity(rMap, rEntities, EntityType::Player, position, sf::Vector2f(40, 80), sf::Vector2f(10, 64), 1, 3, 18, true, true, true, 1),
      m_inputManager(inputManager)
{

}

void Player::HandleInput()
{
    // Left/Right
    if ((m_inputManager.KeyHeld(sf::Keyboard::A) && !m_inputManager.KeyHeld(sf::Keyboard::D)) ||
        (m_inputManager.TouchHeld() && m_inputManager.GetWindowMousePosition().x < static_cast<int>(m_inputManager.GetWindowDimensions().x) / 2) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::X) < -50)
    {
        m_isPressingLeft = true;
    }
    else
    {
        m_isPressingLeft = false;
    }
    if ((m_inputManager.KeyHeld(sf::Keyboard::D) && !m_inputManager.KeyHeld(sf::Keyboard::A)) ||
        (m_inputManager.TouchHeld() && m_inputManager.GetWindowMousePosition().x > static_cast<int>(m_inputManager.GetWindowDimensions().x) / 2) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::X) > 50)
    {
        m_isPressingRight = true;
    }
    else
    {
        m_isPressingRight = false;
    }

    // Up/Down
    if (m_inputManager.KeyHeld(sf::Keyboard::W) ||
        m_inputManager.JoystickButtonDown(0, 0) || m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Y) < -50)
    {
        m_isPressingUp = true;
    }
    else
    {
        m_isPressingUp = false;
    }
    if (m_inputManager.KeyHeld(sf::Keyboard::S) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Y) > 50)
    {
        m_isPressingDown = true;
    }
    else
    {
        m_isPressingDown = false;
    }

    // Shift
    if (m_inputManager.KeyHeld(sf::Keyboard::LShift) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Axis::Z) > 0)
    {
        m_isPressingShift = true;
    }
    else
    {
        m_isPressingShift = false;
    }
}
