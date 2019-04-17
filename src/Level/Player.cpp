#include "Player.h"

Player::Player(Map& rMap, std::vector<Entity*>& rEntities, const InputManager& inputManager, const sf::Vector2f& position)
    : Entity(rMap, rEntities, EntityType::Player, position, sf::Vector2f(40, 80), sf::Vector2f(10, 64), 1, 3, 18, true, true, true)
    , m_inputManager(inputManager)
    , m_inputContext(inputManager)
{
    m_inputContext.BindRangeToKeyboard(this, &Player::SetHorizontalDirection, sf::Keyboard::A, sf::Keyboard::D);
    m_inputContext.BindRangeToKeyboard(this, &Player::SetVerticalDirection, sf::Keyboard::S, sf::Keyboard::W);
    m_inputContext.BindStateToKey(this, &Player::SetPressingShift, sf::Keyboard::LShift);

    m_inputContext.BindRangeToJoystickAxis(this, &Player::SetHorizontalDirection, 0, sf::Joystick::X);
    m_inputContext.BindRangeToJoystickAxis(this, &Player::SetVerticalDirection, 0, sf::Joystick::Y);
    m_inputContext.BindStateToJoystickAxis(this, &Player::SetPressingShift, 0, sf::Joystick::Z, 0.0);

    // Test
    m_inputContext.BindRangeToMouseScroll(this, &Player::SetHorizontalDirection, sf::Mouse::HorizontalWheel);
    m_inputContext.BindRangeToVerticalMouseMovement(this, &Player::SetHorizontalDirection);
}

void Player::HandleInput()
{
    m_inputContext.Update();
    // Left/Right
    if ((m_inputManager.IsKeyHeld(sf::Keyboard::A) && !m_inputManager.IsKeyHeld(sf::Keyboard::D)) || // Keyboard
        (m_inputManager.IsTouchHeld() && // Touch
         m_inputManager.GetWindowMousePosition().x < static_cast<int>(m_inputManager.GetWindowDimensions().x) / 2) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::X) < -50) // Joystick
    {
        //SetPressingLeft(true);
    }
    else
    {
        //SetPressingLeft(false);
    }
    if ((m_inputManager.IsKeyHeld(sf::Keyboard::D) && !m_inputManager.IsKeyHeld(sf::Keyboard::A)) || // Keyboard
        (m_inputManager.IsTouchHeld() && // Touch
         m_inputManager.GetWindowMousePosition().x > static_cast<int>(m_inputManager.GetWindowDimensions().x) / 2) ||
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::X) > 50) // Joystick
    {
        //SetPressingRight(true);
    }
    else
    {
        //SetPressingRight(false);
    }

    // Up/Down
    if (m_inputManager.IsKeyHeld(sf::Keyboard::W) || // Keyboard
        m_inputManager.IsJoystickButtonDescending(0, 0) || // Joystick
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Y) < -50)
    {
        //SetPressingUp(true);
    }
    else
    {
        //SetPressingUp(false);
    }
    if (m_inputManager.IsKeyHeld(sf::Keyboard::S) || // Keyboard
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Y) > 50) // Joystick
    {
        //SetPressingDown(true);
    }
    else
    {
        //SetPressingDown(false);
    }

    // Shift
    if (m_inputManager.IsKeyHeld(sf::Keyboard::LShift) || // Keyboard
        m_inputManager.GetJoystickAxisPosition(0, sf::Joystick::Axis::Z) > 0) // Joystick
    {
        //SetPressingShift(true);
    }
    else
    {
        //SetPressingShift(false);
    }
}
