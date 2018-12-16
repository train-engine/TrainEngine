#include "Entity.h"
#include <cmath>

namespace
{
    const float gravity = 1.3;
}

Entity::Entity(Map& rMap, std::vector<Entity*>& rEntities, EntityType entityType,
               const sf::Vector2f& position, const sf::Vector2f& dimensions, const sf::Vector2f& maxVelocity,
               float acceleration, float deceleration, float jumpForce, bool isGravityApplied,
               bool isTileCollideable, bool isEntityCollideable)
    : m_entityType(entityType),
      m_state(EntityState::Still),
      m_collisionBox(dimensions),
      m_tileReactionDot({1, 1}),
      m_isDebugBoxVisible(false),
      m_position(position),
      m_previousPosition(position),
      m_dimensions(dimensions),
      m_velocity({0, 0}),
      m_maxVelocity(maxVelocity),
      m_acceleration(acceleration),
      m_deceleration(deceleration),
      m_isTileCollideable(isTileCollideable),
      m_isEntityCollideable(isEntityCollideable),
      m_isFacingRight(true),
      m_isGravityApplied(isGravityApplied),
      m_isOnGround(false),
      m_isPressingUp(false),
      m_isPressingDown(false),
      m_isPressingLeft(false),
      m_isPressingRight(false),
      m_isPressingShift(false),
      m_jumpForce(jumpForce),
      m_defaultClimbSpeed(8),
      m_defaultDescentSpeed(6),
      m_rMap(rMap),
      m_rEntities(rEntities)
{
    m_collisionBox.setOrigin(m_dimensions / 2.0f);
    m_collisionBox.setPosition(m_position);
    m_collisionBox.setFillColor(sf::Color(255, 100, 100, 100));
    m_collisionBox.setOutlineColor(sf::Color::Black);
    m_collisionBox.setOutlineThickness(-1);

    m_tileReactionDot.setFillColor(sf::Color::Cyan);
}

// Apply collision with a pointer to a const Tile
void Entity::TileCollision(const Tile* pTile)
{
    // If the Tile is null
    if (pTile == nullptr)
    {
        return;
    }
    if (pTile->IsSolid())
    {
        switch (pTile->GetTileType())
        {
            default:
                StandardCollision(pTile);
                break;
            case TileType::LadderTop:
                LadderTopCollision(pTile);
                break;
        }
    }
}

// Apply collision with a pointer to a const Entity
void Entity::EntityCollision(const Entity* pEntity)
{
    // TODO
}

// Perform reactions with a pointer to a Tile
void Entity::TileReaction(Tile* pTile)
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

// Perform reactions with a pointer to an Entity
void Entity::EntityReaction(Entity* pEntity)
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

// Collision used for Tiles that have collision for all four sides
void Entity::StandardCollision(const Tile* pTile)
{
    sf::Vector2f tilePosition = pTile->GetPosition();
    sf::Vector2f tileDimensions = pTile->GetDimensions();
    // Check for Y-axis overlap
    if (m_position.y + m_dimensions.y / 2 + m_velocity.y >= tilePosition.y &&
        m_position.y - m_dimensions.y / 2 + m_velocity.y < tilePosition.y + tileDimensions.y)
    {
        if (m_position.x + m_dimensions.x / 2 > tilePosition.x &&
            m_position.x - m_dimensions.x / 2 < tilePosition.x + tileDimensions.x)
        {
            // If Entity will be above Tile
            if (m_velocity.y > 0)
            {
                m_position.y = tilePosition.y - m_dimensions.y / 2;
                m_isOnGround = true;
            }
            // If Entity will be under Tile
            else if (m_velocity.y < 0)
            {
                m_position.y = tilePosition.y + tileDimensions.y + m_dimensions.y / 2;
                m_isOnGround = false;
            }
            m_velocity.y = 0;
        }
    }
    // Check for X-axis overlap
    if (m_position.x + m_dimensions.x / 2 + m_velocity.x >= tilePosition.x &&
        m_position.x - m_dimensions.x / 2 + m_velocity.x < tilePosition.x + tileDimensions.x)
    {
        if (m_position.y + m_dimensions.y / 2 > tilePosition.y &&
            m_position.y - m_dimensions.y / 2 < tilePosition.y + tileDimensions.y)
        {
            // If Entity will be on the left side of Tile
            if (m_velocity.x > 0)
            {
                m_position.x = tilePosition.x - m_dimensions.x / 2;
            }
            // If Entity will be on the right side of Tile
            else if (m_velocity.x < 0)
            {
                m_position.x = tilePosition.x + tileDimensions.x + m_dimensions.x / 2;
            }
            m_velocity.x = 0;
        }
    }

    // Check for corner overlap
    if (m_position.x + m_dimensions.x / 2 + m_velocity.x >= tilePosition.x &&
        m_position.x - m_dimensions.x / 2 + m_velocity.x < tilePosition.x + tileDimensions.x &&
        m_position.y + m_dimensions.y / 2 + m_velocity.y >= tilePosition.y &&
        m_position.y - m_dimensions.y / 2 + m_velocity.y < tilePosition.y + tileDimensions.y)
    {
        // If Entity is going right and down
        if (m_velocity.x > 0 && m_velocity.y > 0)
        {
            if (m_position.x + m_dimensions.x / 2 + m_velocity.x - tilePosition.x >= m_position.y + m_dimensions.y / 2 + m_velocity.y - tilePosition.y)
            {
                m_position.y = tilePosition.y - m_dimensions.y / 2;
                m_isOnGround = true;
                m_velocity.y = 0;
            }
            else
            {
                m_position.x = tilePosition.x - m_dimensions.x / 2;
                m_velocity.x = 0;
            }
        }
        // If Entity is going left and down
        else if (m_velocity.x < 0 && m_velocity.y > 0)
        {
            if (tilePosition.x + tileDimensions.x - m_position.x - m_dimensions.x / 2 + m_velocity.x >= m_position.y + m_dimensions.y / 2 + m_velocity.y - tilePosition.y)
            {
                m_position.y = tilePosition.y - m_dimensions.y / 2;
                m_isOnGround = true;
                m_velocity.y = 0;
            }
            else
            {
                m_position.x = tilePosition.x + tileDimensions.x + m_dimensions.x / 2;
                m_velocity.x = 0;
            }
        }
        // If Entity is going right and up
        else if (m_velocity.x > 0 && m_velocity.y < 0)
        {
            if (m_position.x - m_dimensions.x / 2 + m_velocity.x - tilePosition.x >= tilePosition.y + tileDimensions.y - m_position.y - m_dimensions.y / 2 + m_velocity.y)
            {
                m_position.y = tilePosition.y + tileDimensions.y + m_dimensions.y / 2;
                m_velocity.y = 0;
            }
            else
            {
                m_position.x = tilePosition.x - m_dimensions.x / 2;
                m_velocity.x = 0;
            }
        }
        // If Entity is going left and up
        else if (m_velocity.x < 0 && m_velocity.y < 0)
        {
            if (tilePosition.x + tileDimensions.x - m_position.x - m_dimensions.x / 2 + m_velocity.x >= tilePosition.y + tileDimensions.y - m_position.y - m_dimensions.y / 2 + m_velocity.y)
            {
                m_position.y = tilePosition.y + tileDimensions.y + m_dimensions.y / 2;
                m_velocity.y = 0;
            }
            else
            {
                m_position.x = tilePosition.x + tileDimensions.x + m_dimensions.x / 2;
                m_velocity.x = 0;
            }
        }
    }
}

// Collision used for LadderTop Tiles that have collision for all four sides
void Entity::LadderTopCollision(const Tile* pTile)
{
    sf::Vector2f tilePosition = pTile->GetPosition();
    sf::Vector2f tileDimensions = pTile->GetDimensions();
    // If Entity is going downwards
    if (m_velocity.y >= 0)
    {
        // Check for x-axis overlap
        if (m_position.x + m_dimensions.x / 2 > tilePosition.x &&
            m_position.x - m_dimensions.x / 2 < tilePosition.x + tileDimensions.x)
        {
            // If in the current tick, the Entity's bottom side is above the LadderTop,
            // but it will be inside the LadderTop in the next tick
            if (m_position.y + m_dimensions.y / 2 <= tilePosition.y &&
                m_position.y + m_dimensions.y / 2 + m_velocity.y > tilePosition.y)
            {
                if (m_isPressingDown == false)
                {
                    m_position.y = tilePosition.y - m_dimensions.y / 2;
                    m_isOnGround = true;
                    m_velocity.y = 0;
                }
                else
                {
                    m_state = EntityState::Climbing;
                }
            }
        }
    }
}

void Entity::MoveLeft()
{
    if (GetVelocity().x > 0) SetHorizVelocity(GetVelocity().x - m_deceleration);
    else SetHorizVelocity(GetVelocity().x - m_acceleration);
}

void Entity::MoveRight()
{
    if (GetVelocity().x < 0) SetHorizVelocity(GetVelocity().x + m_deceleration);
    else SetHorizVelocity(GetVelocity().x + m_acceleration);
}

void Entity::Jump()
{
    SetVertVelocity(-m_jumpForce);
}

void Entity::Climb(float factor)
{
    if (m_isPressingUp == true)
    {
        SetVertVelocity(-m_defaultClimbSpeed * factor);
        m_state = EntityState::Climbing;
    }
    else
    {
        if (m_isPressingShift == true && m_isPressingUp == false)
        {
            SetVertVelocity(0);
        }
        else
        {
            SetVertVelocity(m_defaultDescentSpeed * factor);
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
        if (GetVelocity().y == 0 && m_state == EntityState::Climbing)
        {
            it->second.Pause();
        }
        else
        {
            it->second.Play();
        }
    }
}

void Entity::draw(sf::RenderTarget& rTarget, sf::RenderStates states) const
{
    auto it = m_animatedSprites.find(m_state);
    if (it != m_animatedSprites.cend())
    {
        rTarget.draw(it->second, states);
    }
    else
    {
        rTarget.draw(m_defaultSprite, states);
    }
    if (m_isDebugBoxVisible == true)
    {
        rTarget.draw(m_collisionBox, states);
        rTarget.draw(m_tileReactionDot, states);
    }
}

// Apply deceleration
void Entity::ApplyDeceleration()
{
    if (m_velocity.x >= m_deceleration) m_velocity.x -= m_deceleration;
    else if (m_velocity.x <= -m_deceleration) m_velocity.x += m_deceleration;
    else m_velocity.x = 0;
}

// Apply gravity to the Entity
void Entity::ApplyGravity()
{
    if (m_isGravityApplied == true) m_velocity.y += gravity;
}

// Cap the Entity's velocity
void Entity::MaxVelocityCap()
{
    if (m_velocity.x >= m_maxVelocity.x) m_velocity.x = m_maxVelocity.x;
    else if (m_velocity.x <= -m_maxVelocity.x) m_velocity.x = -m_maxVelocity.x;
    if (m_velocity.y >= m_maxVelocity.y) m_velocity.y = m_maxVelocity.y;
    else if (m_velocity.y <= -m_maxVelocity.y) m_velocity.y = -m_maxVelocity.y;
}

// Apply collision with the edges of the Map
void Entity::MapEdgeCollision(bool isHorizCollisionEnabled, bool isVertCollisionEnabled)
{
    // If Map is null
    if (m_rMap.IsNull())
    {
        return;
    }
    
    if (isHorizCollisionEnabled == true)
    {
        if (m_position.x - m_dimensions.x / 2 + m_velocity.x <= 0)
        {
            m_position.x = 0 + m_dimensions.x / 2;
            m_velocity.x = 0;
        }
        else if (m_position.x + m_dimensions.x / 2 + m_velocity.x >= m_rMap.GetBounds().x)
        {
            m_position.x = m_rMap.GetBounds().x - m_dimensions.x / 2;
            m_velocity.x = 0;
        }
    }
    if (isVertCollisionEnabled == true)
    {
        if (m_position.y - m_dimensions.y / 2 + m_velocity.y <= 0)
        {
            m_position.y = 0 + m_dimensions.y / 2;
            m_velocity.y = 0;
            m_isOnGround = false;
        }
        else if (m_position.y + m_dimensions.y / 2 + m_velocity.y >= m_rMap.GetBounds().y)
        {
            m_position.y = m_rMap.GetBounds().y - m_dimensions.y / 2;
            m_velocity.y = 0;
            m_isOnGround = true;
        }
    }
}

// Perform collision checks and resolutions with Tiles, Map edge, and Entities
void Entity::PerformCollisions()
{
    // Determine the range of positions where the Entity could be in the next tick
    float biggestAxis = std::fmax(m_dimensions.x, m_dimensions.y);
    float highestVelocity = std::fmax(m_velocity.x, m_velocity.y);
    float range = (biggestAxis + highestVelocity) / m_rMap.GetTileSize();
    
    // Collision with Tiles
    if (m_isTileCollideable == true)
    {
        // Loop through Tiles in the same direction as the Entity's velocity to fix collision problems
        sf::Vector2u positionIndex = m_rMap.CoordsToTileIndex(m_position);
        if (m_velocity.x >= 0)
        {
            for (int i = -range - 1; i <= range + 1; i++)
            {
                if (m_velocity.y >= 0)
                {
                    for (int j = -range - 1; j <= range + 1; j++)
                    {
                        TileCollision(m_rMap.GetKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
                    }
                }
                else
                {
                    for (int j = range + 1; j >= -range - 1; j--)
                    {
                        TileCollision(m_rMap.GetKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
                    }
                }
            }
        }
        else
        {
            for (int i = range + 1; i >= -range - 1; i--)
            {
                if (m_velocity.y >= 0)
                {
                    for (int j = -range - 1; j <= range + 1; j++)
                    {
                        TileCollision(m_rMap.GetKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
                    }
                }
                else
                {
                    for (int j = range + 1; j >= -range - 1; j--)
                    {
                        TileCollision(m_rMap.GetKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
                    }
                }
            }
        }
    }
    
    MapEdgeCollision();
    
    // Collision with Entities
    if (m_isEntityCollideable == true)
    {
        for (auto& rEntity : m_rEntities)
        {
            if (rEntity != this)
            {
                EntityCollision(rEntity);
            }
        }
    }
}

void Entity::Update()
{
    m_previousPosition = m_position;

    // Movement
    ApplyGravity();
    if (m_isPressingLeft == true)
    {
        MoveLeft();
        m_isFacingRight = false;
    }
    if (m_isPressingRight == true)
    {
        MoveRight();
        m_isFacingRight = true;
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
    std::array<sf::Vector2f, 5> tileReactionPoints =
    {
        sf::Vector2f(GetPosition().x + GetVelocity().x, GetPosition().y + GetVelocity().y), // Center
        sf::Vector2f(GetPosition().x + GetVelocity().x, GetPosition().y + GetDimensions().y / 2.75 + GetVelocity().y), // Near bottom
        sf::Vector2f(GetPosition().x + GetVelocity().x, GetPosition().y - GetDimensions().y / 2.75 - GetVelocity().y), // Near top
        sf::Vector2f(GetPosition().x - GetDimensions().x / 2.75 + GetVelocity().x, GetPosition().y + GetVelocity().y), // Near left
        sf::Vector2f(GetPosition().x + GetDimensions().x / 2.75 + GetVelocity().x, GetPosition().y + GetVelocity().y)  // Near right
    };
    // Cycle through the possible points to do a TileReaction on a Tile on one of those points, if found
    for (unsigned int i = 0; i < tileReactionPoints.size(); i++)
    {
        Tile* pTile = m_rMap.GetTilePtr(m_rMap.CoordsToTileIndex(tileReactionPoints[i]), MapLayer::Solid);
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

    SetPosition(GetPosition() + GetVelocity());

    // States
    if (m_state != EntityState::Climbing || m_isOnGround == true)
    {
        if ((m_isPressingLeft == true || m_isPressingRight == true) && m_isOnGround == true)
        {
            m_state = EntityState::Running;
        }
        else if (m_isOnGround == false)
        {
            if (GetVelocity().y >= 0)
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
        it->second.Update(!m_isFacingRight);
        it->second.Play();
    }

    m_collisionBox.setPosition(GetPosition());
}

void Entity::Interpolate(float lag)
{
    sf::Vector2f position = m_previousPosition + (m_position - m_previousPosition) * lag;
    auto it = m_animatedSprites.find(m_state);
    if (it != m_animatedSprites.cend())
    {
        it->second.SetPosition(position);
    }
    else
    {
        m_defaultSprite.setPosition(position);
    }
}

void Entity::AddAnimation(EntityState targetState, const AnimatedSprite& animatedSprite)
{
    auto it = m_animatedSprites.find(targetState);
    // If animation is already set to a state, overwrite it
    if (it != m_animatedSprites.end())
    {
        it->second = animatedSprite;
        return;
    }
    // Otherwise, add it to the map of animations
    m_animatedSprites.emplace(targetState, animatedSprite);

    m_animatedSprites.at(targetState).SetPosition(m_position);
}

void Entity::AddAnimation(EntityState targetState, AnimatedSprite&& animatedSprite)
{
    auto it = m_animatedSprites.find(targetState);
    // If animation is already set to a state, overwrite it
    if (it != m_animatedSprites.end())
    {
        it->second = std::move(animatedSprite);
        return;
    }
    // Otherwise, add it to the map of animations
    m_animatedSprites.emplace(targetState, std::move(animatedSprite));

    m_animatedSprites.at(targetState).SetPosition(m_position);
}

void Entity::SetDefaultSpriteTexture(const sf::Texture& texture)
{
    m_defaultSprite.setTexture(texture);
    m_defaultSprite.setOrigin(static_cast<sf::Vector2f>(texture.getSize()) / 2.0f);
    m_defaultSprite.setPosition(m_position);
}

std::string Entity::GetEntityTypeString(EntityType entityType)
{
    static const std::unordered_map<EntityType, std::string> entityTypeStrings = {{EntityType::Player, "Player"}};
    
    const auto it = entityTypeStrings.find(entityType);
    if (it != entityTypeStrings.cend())
    {
        return it->second;
    }
    return "Unknown EntityType";
}

std::vector<std::string> Entity::GetTextureNames(EntityType entityType)
{
    static const std::unordered_map<EntityType, std::vector<std::string>> entityTextures = {{EntityType::Player, {"characterStill", "characterRunning", "characterClimbing", "characterJumping", "characterFalling"}}};

    const auto it = entityTextures.find(entityType);
    if (it != entityTextures.cend())
    {
        return it->second;
    }
    return {};
}

// Functions used for getting Entity's pixel position bounds

float Entity::GetLeftPixelPosition() const
{
    return m_position.x - m_dimensions.x / 2;
}

float Entity::GetRightPixelPosition() const
{
    return m_position.x + m_dimensions.x / 2 - 1; // Solve off-by-one error caused by pixel width
}

float Entity::GetTopPixelPosition() const
{
    return m_position.y - m_dimensions.y / 2;
}

float Entity::GetBottomPixelPosition() const
{
    return m_position.y + m_dimensions.y / 2 - 1; // Solve off-by-one error caused by pixel width
}
