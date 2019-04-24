#include "Level/Player.h"
#include <iostream>


Player::Player(Map& rMap, std::vector<Entity*>& rEntities, const InputManager& inputManager, const sf::Vector2f& position)
    : Entity(rMap, rEntities, EntityType::Player, position, sf::Vector2f(40, 80), sf::Vector2f(7, 64), 1, 3, 18, true, true, true)
    , m_inputManager(inputManager)
    , m_inputContext(inputManager)
{
    m_inputContext.BindRangeToKeyboard(this, &Player::SetHorizontalDirection, sf::Keyboard::A, sf::Keyboard::D);
    m_inputContext.BindRangeToKeyboard(this, &Player::SetVerticalDirection, sf::Keyboard::W, sf::Keyboard::S);
    m_inputContext.BindStateToKey(this, &Player::SetIsPressingUp, sf::Keyboard::W);

    m_inputContext.BindRangeToJoystickAxis(this, &Player::SetHorizontalDirection, 0, sf::Joystick::X);
    m_inputContext.BindRangeToJoystickAxis(this, &Player::SetVerticalDirection, 0, sf::Joystick::Y);
    m_inputContext.BindStateToJoystickButton(this, &Player::SetIsPressingUp, 0, 0);
}

void Player::HandleInput()
{
    m_inputContext.Update();
}