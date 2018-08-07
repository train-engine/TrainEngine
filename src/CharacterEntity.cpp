#include "CharacterEntity.h"
#include <array>

CharacterEntity::CharacterEntity(Map& rMap, std::vector<Entity*>& rEntities, EntityType entityType,
                                 const sf::Vector2f& position, const sf::Vector2f& dimensions, const sf::Vector2f& maxVelocity,
                                 float acceleration, float deceleration, float jumpForce, bool applyGravity,
                                 bool isTileCollideable, bool isEntityCollideable, unsigned int collisionPriority)
    : Entity(rMap, rEntities, entityType, position, dimensions, maxVelocity, acceleration, deceleration, applyGravity, isTileCollideable, isEntityCollideable, collisionPriority),
      m_tileReactionDot(sf::Vector2f(1, 1)),
      m_isPressingUp(false),
      m_isPressingDown(false),
      m_isPressingLeft(false),
      m_isPressingRight(false),
      m_isPressingShift(false),
      m_jumpForce(jumpForce),
      m_defaultClimbSpeed(8),
      m_defaultDescentSpeed(6)
{
    m_tileReactionDot.setFillColor(sf::Color::Cyan);
}

void CharacterEntity::draw(sf::RenderTarget& rTarget, sf::RenderStates states) const
{
    Entity::draw(rTarget, states);
    if (m_displayCollisionBox == true)
    {
        rTarget.draw(m_tileReactionDot, states);
    }
}

void CharacterEntity::MoveLeft()
{
    if (m_velocity.x > 0) m_velocity.x -= m_deceleration;
    else m_velocity.x -= m_acceleration;
}

void CharacterEntity::MoveRight()
{
    if (m_velocity.x < 0) m_velocity.x += m_deceleration;
    else m_velocity.x += m_acceleration;
}

void CharacterEntity::Jump()
{
    m_velocity.y = -m_jumpForce;
}

void CharacterEntity::Climb(float factor)
{
    if (m_isPressingUp == true)
    {
        m_velocity.y = -(m_defaultClimbSpeed * factor);
        m_state = EntityState::Climbing;
    }
    else
    {
        if (m_isPressingShift == true && m_isPressingUp == false)
        {
            m_velocity.y = 0;
        }
        else
        {
            m_velocity.y = m_defaultDescentSpeed * factor;
        }
        
        if (m_isOnGround == false)
        {
            m_state = EntityState::Climbing;
        }
    }
    
    // Animation
    auto it = m_animatedSprites.find(m_state);
    if (it != m_animatedSprites.cend())
    {
        if (m_velocity.y == 0 && m_state == EntityState::Climbing)
        {
            it->second.Pause();
        }
        else
        {
            it->second.Play();
        }
    }
}

// Collision used for LadderTop Tiles that have collision for all four sides
void CharacterEntity::LadderTopCollision(const Tile* const pTile)
{
    sf::Vector2f tilePosition = pTile->GetPosition();
    sf::Vector2f tileDimensions = pTile->GetDimensions();
    // If CharacterEntity is going downwards
    if (m_velocity.y >= 0)
    {
        // Check for x-axis overlap
        if (m_position.x + m_dimensions.x / 2 > tilePosition.x &&
            m_position.x - m_dimensions.x / 2 < tilePosition.x + tileDimensions.x)
        {
            // If in the current tick, the CharacterEntity's bottom side is above the LadderTop,
            // but it will be inside the LadderTop in the next tick
            if (m_position.y + m_dimensions.y / 2 <= tilePosition.y &&
                m_position.y + m_dimensions.y / 2 + m_velocity.y > tilePosition.y)
            {
                if (m_isPressingDown == false)
                {
                    m_position.y = tilePosition.y - m_dimensions.y / 2;
                    m_isOnGround = true;
                    ResetVertVelocity();
                }
                else
                {
                    m_state = EntityState::Climbing;
                }
            }
        }
    }
}

// Perform reactions with a const pointer to a Tile
void CharacterEntity::TileReaction(Tile* const pTile)
{
    if (pTile == nullptr)
    {
        m_state = EntityState::Still;
        return;
    }
    switch (pTile->GetTileType())
    {
    case TileType::Ladder:
    case TileType::LadderTop:
        Climb();
        break;
    case TileType::Vine:
        Climb(0.75);
        break;
    default:
        m_state = EntityState::Still;
        break;
    }
}

// Perform reactions with a const pointer to an Entity
void CharacterEntity::EntityReaction(Entity* const pEntity)
{
    switch (pEntity->GetEntityType())
    {
    case EntityType::Player:
        PlayerReaction(pEntity);
        break;
    default:
        break;
    }
}

void CharacterEntity::Update()
{
    m_previousPosition = m_position;

    // Movement
    ApplyGravity();
    if (m_isPressingLeft == true)
    {
        MoveLeft();
        m_isGoingRight = false;
    }
    if (m_isPressingRight == true)
    {
        MoveRight();
        m_isGoingRight = true;
    }
    if (m_isPressingLeft == false && m_isPressingRight == false)
    {
        ApplyDeceleration();
    }
    if (m_isPressingUp == true && m_isOnGround == true)
    {
        Jump();
    }

    // Max velocity
    MaxVelocityCap();
    
    // For falling off a ledge
    m_isOnGround = false;
    
    // Preliminary collision checking (TEMPORARY - TO BE FIXED WITH DOUBLE BUFFER)
    //PerformCollisions();
    
    // Reactions with Tiles
    std::array<sf::Vector2f, 5> tileReactionPoints = {sf::Vector2f(m_position.x + m_velocity.x, m_position.y + m_velocity.y), // Center
                                                      sf::Vector2f(m_position.x + m_velocity.x, m_position.y + m_dimensions.y / 2.75 + m_velocity.y), // Near bottom
                                                      sf::Vector2f(m_position.x + m_velocity.x, m_position.y - m_dimensions.y / 2.75 - m_velocity.y), // Near top
                                                      sf::Vector2f(m_position.x - m_dimensions.x / 2.75 + m_velocity.x, m_position.y + m_velocity.y), // Near left
                                                      sf::Vector2f(m_position.x + m_dimensions.x / 2.75 + m_velocity.x, m_position.y + m_velocity.y)}; // Near right
    // Cycle through the possible points to do a TileReaction on a Tile on one of those points, if found
    for (unsigned int i = 0; i < tileReactionPoints.size(); i++)
    {
        Tile* const pTile = m_rMap.GetTilePtr(m_rMap.CoordsToTileIndex(tileReactionPoints[i]), MapLayer::Solid);
        if (pTile != nullptr)
        {
            TileReaction(pTile);
            m_tileReactionDot.setPosition(tileReactionPoints[i]);
            break;
        }
        // If no valid Tile is found, perform TileReaction on first point (equivalent to nullptr) anyway (to reset Entity state)
        if (i == tileReactionPoints.size() - 1)
        {
            m_tileReactionDot.setPosition(tileReactionPoints[0]);
            TileReaction(nullptr);
        }
    }
    
    // Reactions with Entities
    // ---
    
    // Collisions
    PerformCollisions();

    m_position += m_velocity;
    
    // States
    if (m_state != EntityState::Climbing || m_isOnGround == true)
    {
        if ((m_isPressingLeft == true || m_isPressingRight == true) && m_isOnGround == true)
        {
            m_state = EntityState::Running;
        }
        else if (m_isOnGround == false)
        {
            if (m_velocity.y >= 0)
            {
                m_state = EntityState::Falling;
            }
            else
            {
                m_state = EntityState::Jumping;
            }
        }
        else
        {
            m_state = EntityState::Still;
        }
    }
    
    // Animations
    auto it = m_animatedSprites.find(m_state);
    if (it != m_animatedSprites.cend())
    {
        it->second.Update(!m_isGoingRight);
        it->second.Play();
    }
    
    m_collisionBox.setPosition(m_position);
}
