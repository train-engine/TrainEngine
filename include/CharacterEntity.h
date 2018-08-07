#ifndef CHARACTERENTITY_H_INCLUDED
#define CHARACTERENTITY_H_INCLUDED

#include "Entity.h"

class CharacterEntity : public Entity
{
protected:
    sf::RectangleShape m_tileReactionDot;
    
    bool m_isPressingUp;
    bool m_isPressingDown;
    bool m_isPressingLeft;
    bool m_isPressingRight;
    bool m_isPressingShift;

    float m_jumpForce;
    float m_defaultClimbSpeed;
    float m_defaultDescentSpeed;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    virtual void MoveLeft();
    virtual void MoveRight();
    virtual void Jump();
    virtual void Climb(float factor = 1);

    // TileCollision functions
    virtual void LadderTopCollision(const Tile* const pTile) override;

    // EntityCollision functions
    // ---

    // Actions with Tiles
    void TileReaction(Tile* const pTile);

    // TileReaction functions
    // ---

    // Actions with Entities
    void EntityReaction(Entity* const pEntity);

    // EntityReaction functions
    virtual void PlayerReaction(Entity* const pEntity) {}

public:
    // Constructor and destructor
    CharacterEntity(Map& rMap, std::vector<Entity*>& rEntities, EntityType entityType,
                    const sf::Vector2f& position, const sf::Vector2f& dimensions, const sf::Vector2f& maxVelocity = {10, 64},
                    float acceleration = 1, float deceleration = 3, float jumpForce = 18, bool applyGravity = true,
                    bool isTileCollideable = false, bool isEntityCollideable = false, unsigned int collisionPriority = 0);
    virtual ~CharacterEntity() override {}

    // Functions
    virtual void Update() override;
};

#endif // CHARACTERENTITY_H_INCLUDED
