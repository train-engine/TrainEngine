#include "Level/Player.h"
#include <iostream>


Player::Player(Map& rMap, std::vector<Entity*>& rEntities, const InputManager& inputManager, const sf::Vector2f& position)
    : Entity(rMap, rEntities, EntityType::Player, position, sf::Vector2f(40, 80), sf::Vector2f(10, 64), 1, 3, 18, true, true, true)
    , m_inputManager(inputManager)
    , m_inputContext(inputManager)
{
    // Keyboard bindings
    m_inputContext.BindRangeToKeyboard(this, &Player::SetHorizontalInputDirection, sf::Keyboard::A, sf::Keyboard::D);
    m_inputContext.BindRangeToKeyboard(this, &Player::SetVerticalInputDirection, sf::Keyboard::W, sf::Keyboard::S);
    m_inputContext.BindStateToKey(this, &Player::SetPressingUp, sf::Keyboard::W);

    // Joystick bindings
    m_inputContext.BindRangeToJoystickAxis(this, &Player::SetHorizontalInputDirection, 0, sf::Joystick::X);
    m_inputContext.BindRangeToJoystickAxis(this, &Player::SetVerticalInputDirection, 0, sf::Joystick::Y);
    m_inputContext.BindRangeToJoystickButtons(this, &Player::SetVerticalInputDirection, 0, 5, 4);
    m_inputContext.BindStateToJoystickButton(this, &Player::SetPressingUp, 0, 0);
}

void Player::HandleInput()
{
    m_inputContext.Update();
}
