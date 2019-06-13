#ifndef PLAYER_H
#define PLAYER_H

#include "Core/InputManager.h"
#include "Level/Entity.h"

class Player final : public Entity
{
private:
    const InputManager& m_inputManager;
    InputContext m_inputContext;

public:
    // Constructor
    Player(Map& map, std::vector<Entity*>& entities, const InputManager& inputManager, const sf::Vector2f& position);

    // Functions
    virtual void handleInput() override;
};

#endif // PLAYER_H
