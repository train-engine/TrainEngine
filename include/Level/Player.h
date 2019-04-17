#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "InputContext.h"
#include "InputManager.h"

class Player final : public Entity
{
private:
    const InputManager& m_inputManager;
    InputContext m_inputContext;

public:
    // Constructor
    Player(Map& rMap, std::vector<Entity*>& rEntities, const InputManager& inputManager, const sf::Vector2f& position);

    // Functions
    virtual void HandleInput() override;
};

#endif // PLAYER_H
