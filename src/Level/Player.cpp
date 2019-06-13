#include "Level/Player.h"

Player::Player(Map& rMap, std::vector<Entity*>& rEntities, const InputManager& inputManager, const sf::Vector2f& position)
    : Entity(rMap, rEntities, EntityType::Player, position, sf::Vector2f(40, 80), sf::Vector2f(10, 64), 1, 3, 18, true, true, true)
    , m_inputManager(inputManager)
    , m_inputContext(inputManager)
{
    // Keyboard bindings
    m_inputContext.bindRangeToKeyboard(this, &Player::setHorizontalInputDirection, sf::Keyboard::A, sf::Keyboard::D);
    m_inputContext.bindRangeToKeyboard(this, &Player::setVerticalInputDirection, sf::Keyboard::W, sf::Keyboard::S);
    m_inputContext.bindStateToKey(this, &Player::setPressingUp, sf::Keyboard::W);

    // Joystick bindings
    m_inputContext.bindRangeToJoystickAxis(this, &Player::setHorizontalInputDirection, 0, sf::Joystick::X);
    m_inputContext.bindRangeToJoystickAxis(this, &Player::setVerticalInputDirection, 0, sf::Joystick::Y);
    m_inputContext.bindRangeToJoystickButtons(this, &Player::setVerticalInputDirection, 0, 5, 4);
    m_inputContext.bindStateToJoystickButton(this, &Player::setPressingUp, 0, 0);
}

void Player::handleInput()
{
    m_inputContext.update();
}
