#include "Level/Entity.h"
#include <cmath>

namespace
{
    const float gravity = 1.3;
} // namespace

Entity::Entity(Map& map, std::vector<Entity*>& entities, EntityType entityType, const sf::Vector2f& position,
               const sf::Vector2f& dimensions, const sf::Vector2f& maxVelocity, float acceleration, float deceleration, float jumpForce,
               bool isGravityApplied, bool isTileCollideable, bool isEntityCollideable)
    : m_entityType(entityType)
    , m_state(EntityState::Still)
    , m_collisionBox(dimensions)
    , m_tileReactionDot({1, 1})
    , m_isDebugBoxVisible(false)
    , m_position(position)
    , m_previousPosition(position)
    , m_dimensions(dimensions)
    , m_maxVelocity(maxVelocity)
    , m_acceleration(acceleration)
    , m_deceleration(deceleration)
    , m_isTileCollideable(isTileCollideable)
    , m_isEntityCollideable(isEntityCollideable)
    , m_isFacingRight(true)
    , m_isGravityApplied(isGravityApplied)
    , m_isOnGround(false)
    , m_inputDirection(0.0, 0.0)
    , m_isPressingUp(false)
    , m_jumpForce(jumpForce)
    , m_defaultClimbSpeed(8)
    , m_defaultDescentSpeed(6)
    , m_map(map)
    , m_entities(entities)
{
    m_collisionBox.setOrigin(m_dimensions / 2.0f);
    m_collisionBox.setPosition(m_position);
    m_collisionBox.setFillColor(sf::Color(255, 100, 100, 100));
    m_collisionBox.setOutlineColor(sf::Color::Black);
    m_collisionBox.setOutlineThickness(-1);

    m_tileReactionDot.setFillColor(sf::Color::Cyan);
}

// Apply collision with a pointer to a const Tile
void Entity::tileCollision(const Tile* tile)
{
    if (tile == nullptr)
    {
        return;
    }

    if (tile->isSolid())
    {
        switch (tile->getTileType())
        {
        default:
            standardCollision(tile);
            break;
        case TileType::LadderTop:
            ladderTopCollision(tile);
            break;
        }
    }
}

// Apply collision with a pointer to a const Entity
void Entity::entityCollision(const Entity* entity)
{
    // TODO
}

// Perform reactions with a pointer to a Tile
void Entity::tileReaction(Tile* tile)
{
    if (tile == nullptr)
    {
        m_state = EntityState::Still;
        return;
    }

    switch (tile->getTileType())
    {
    case TileType::Ladder:
    case TileType::LadderTop:
        climb();
        break;
    case TileType::Vine:
        climb(0.75);
        break;
    default:
        m_state = EntityState::Still;
        break;
    }
}

// Perform reactions with a pointer to an Entity
void Entity::entityReaction(Entity* entity)
{
    switch (entity->getEntityType())
    {
    case EntityType::Player:
        playerReaction(entity);
        break;
    default:
        break;
    }
}

// Collision used for Tiles that have collision for all four sides
void Entity::standardCollision(const Tile* tile)
{
    sf::Vector2f tilePosition = tile->getPosition();
    sf::Vector2f tileDimensions = tile->getDimensions();
    // Check for Y-axis overlap
    if (m_position.y + m_dimensions.y / 2 + m_velocity.y >= tilePosition.y &&
        m_position.y - m_dimensions.y / 2 + m_velocity.y < tilePosition.y + tileDimensions.y)
    {
        if (m_position.x + m_dimensions.x / 2 > tilePosition.x && m_position.x - m_dimensions.x / 2 < tilePosition.x + tileDimensions.x)
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
        if (m_position.y + m_dimensions.y / 2 > tilePosition.y && m_position.y - m_dimensions.y / 2 < tilePosition.y + tileDimensions.y)
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
            if (m_position.x + m_dimensions.x / 2 + m_velocity.x - tilePosition.x >=
                m_position.y + m_dimensions.y / 2 + m_velocity.y - tilePosition.y)
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
            if (tilePosition.x + tileDimensions.x - m_position.x - m_dimensions.x / 2 + m_velocity.x >=
                m_position.y + m_dimensions.y / 2 + m_velocity.y - tilePosition.y)
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
            if (m_position.x - m_dimensions.x / 2 + m_velocity.x - tilePosition.x >=
                tilePosition.y + tileDimensions.y - m_position.y - m_dimensions.y / 2 + m_velocity.y)
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
            if (tilePosition.x + tileDimensions.x - m_position.x - m_dimensions.x / 2 + m_velocity.x >=
                tilePosition.y + tileDimensions.y - m_position.y - m_dimensions.y / 2 + m_velocity.y)
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

    // Set is on ground even if vertical velocity is zero
    // Check for strict X-axis overlap
    if (m_position.x + m_dimensions.x / 2 > tilePosition.x &&
        m_position.x - m_dimensions.x / 2 < tilePosition.x + tileDimensions.x)
    {
        if (m_position.y == tilePosition.y - m_dimensions.y / 2)
        {
            m_isOnGround = true;
        }
    }
}

// Collision used for LadderTop Tiles that have collision for all four sides
void Entity::ladderTopCollision(const Tile* tile)
{
    sf::Vector2f tilePosition = tile->getPosition();
    sf::Vector2f tileDimensions = tile->getDimensions();
    // If Entity is going downwards
    if (m_velocity.y >= 0)
    {
        // Check for x-axis overlap
        if (m_position.x + m_dimensions.x / 2 > tilePosition.x && m_position.x - m_dimensions.x / 2 < tilePosition.x + tileDimensions.x)
        {
            // If in the current tick, the Entity's bottom side is above the LadderTop,
            // but it will be inside the LadderTop in the next tick
            if (m_position.y + m_dimensions.y / 2 <= tilePosition.y && m_position.y + m_dimensions.y / 2 + m_velocity.y > tilePosition.y)
            {
                // If not going down
                if (m_inputDirection.y <= 0)
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
            // Set is on ground
            else if (m_position.y == tilePosition.y - m_dimensions.y / 2)
            {
                m_isOnGround = true;
            }
        }
    }
}

void Entity::moveLeft()
{
    if (getVelocity().x > 0)
    {
        setHorizVelocity(getVelocity().x - m_deceleration);
    }
    else
    {
        setHorizVelocity(getVelocity().x - m_acceleration);
    }
}

void Entity::moveRight()
{
    if (getVelocity().x < 0)
    {
        setHorizVelocity(getVelocity().x + m_deceleration);
    }
    else
    {
        setHorizVelocity(getVelocity().x + m_acceleration);
    }
}

void Entity::jump()
{
    setVertVelocity(-m_jumpForce);
}

void Entity::climb(float factor)
{
    // If pressing up or down
    if (m_inputDirection.y != 0.0)
    {
        setVertVelocity(m_defaultClimbSpeed * factor * m_inputDirection.y / 100.0);
    }
    else
    {
        setVertVelocity(0);
    }

    if (m_isOnGround == false)
    {
        m_state = EntityState::Climbing;
    }

    // Animation
    auto it = m_animatedSprites.find(m_state);
    if (it != m_animatedSprites.cend())
    {
        if (getVelocity().y == 0 && m_state == EntityState::Climbing)
        {
            it->second.pause();
        }
        else
        {
            it->second.play();
        }
    }
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    auto it = m_animatedSprites.find(m_state);
    if (it != m_animatedSprites.cend())
    {
        target.draw(it->second, states);
    }
    else
    {
        target.draw(m_defaultSprite, states);
    }

    if (m_isDebugBoxVisible == true)
    {
        target.draw(m_collisionBox, states);
        target.draw(m_tileReactionDot, states);
    }
}

// Apply deceleration
void Entity::applyDeceleration()
{
    if (m_velocity.x >= m_deceleration)
    {
        m_velocity.x -= m_deceleration;
    }
    else if (m_velocity.x <= -m_deceleration)
    {
        m_velocity.x += m_deceleration;
    }
    else
    {
        m_velocity.x = 0;
    }
}

// Apply gravity to the Entity
void Entity::applyGravity()
{
    if (m_isGravityApplied == true)
    {
        m_velocity.y += gravity;
    }
}

// Cap the Entity's velocity
void Entity::maxVelocityCap()
{
    // Set max velocity proportional to input activation percentage
    float maxVelocity = m_maxVelocity.x * m_inputDirection.x / 100.0;
    if (m_velocity.x >= maxVelocity)
    {
        m_velocity.x = maxVelocity;
    }
    else if (m_velocity.x <= maxVelocity)
    {
        m_velocity.x = maxVelocity;
    }

    if (m_velocity.y >= m_maxVelocity.y)
    {
        m_velocity.y = m_maxVelocity.y;
    }
    else if (m_velocity.y <= -m_maxVelocity.y)
    {
        m_velocity.y = -m_maxVelocity.y;
    }
}

// Apply collision with the edges of the Map
void Entity::mapEdgeCollision(bool isHorizCollisionEnabled, bool isVertCollisionEnabled)
{
    if (m_map.isNull())
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
        else if (m_position.x + m_dimensions.x / 2 + m_velocity.x >= m_map.getBounds().x)
        {
            m_position.x = m_map.getBounds().x - m_dimensions.x / 2;
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
        else if (m_position.y + m_dimensions.y / 2 + m_velocity.y >= m_map.getBounds().y)
        {
            m_position.y = m_map.getBounds().y - m_dimensions.y / 2;
            m_velocity.y = 0;
            m_isOnGround = true;
        }
    }
}

// Perform collision checks and resolutions with Tiles, Map edge, and Entities
void Entity::performCollisions()
{
    // Determine the range of positions where the Entity could be in the next tick
    float biggestAxis = std::fmax(m_dimensions.x, m_dimensions.y);
    float highestVelocity = std::fmax(m_velocity.x, m_velocity.y);
    float range = (biggestAxis + highestVelocity) / m_map.getTileSize();

    // Collision with Tiles
    if (m_isTileCollideable == true)
    {
        // Loop through Tiles in the same direction as the Entity's velocity to fix collision problems
        sf::Vector2u positionIndex = m_map.coordsToTileIndex(m_position);
        if (m_velocity.x >= 0)
        {
            for (int i = -range - 1; i <= range + 1; i++)
            {
                if (m_velocity.y >= 0)
                {
                    for (int j = -range - 1; j <= range + 1; j++)
                    {
                        tileCollision(m_map.getKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
                    }
                }
                else
                {
                    for (int j = range + 1; j >= -range - 1; j--)
                    {
                        tileCollision(m_map.getKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
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
                        tileCollision(m_map.getKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
                    }
                }
                else
                {
                    for (int j = range + 1; j >= -range - 1; j--)
                    {
                        tileCollision(m_map.getKTilePtr(sf::Vector2u(positionIndex.x + i, positionIndex.y + j), MapLayer::Solid));
                    }
                }
            }
        }
    }

    mapEdgeCollision();

    // Collision with Entities
    if (m_isEntityCollideable == true)
    {
        for (auto& entity : m_entities)
        {
            if (entity != this)
            {
                entityCollision(entity);
            }
        }
    }
}

void Entity::update()
{
    m_previousPosition = m_position;

    // Movement
    applyGravity();
    if (m_inputDirection.x < 0)
    {
        moveLeft();
        m_isFacingRight = false;
    }
    else if (m_inputDirection.x > 0)
    {
        moveRight();
        m_isFacingRight = true;
    }
    else
    {
        applyDeceleration();
    }

    if (m_isPressingUp && m_isOnGround == true)
    {
        jump();
    }

    // Max velocity
    maxVelocityCap();

    // Reactions with Tiles
    // clang-format off
    std::array<sf::Vector2f, 5> tileReactionPoints = {
        // Center
        sf::Vector2f(getPosition().x + getVelocity().x, getPosition().y + getVelocity().y),
        // Near bottom
        sf::Vector2f(getPosition().x + getVelocity().x, getPosition().y + getDimensions().y / 2.75 + getVelocity().y),
        // Near top
        sf::Vector2f(getPosition().x + getVelocity().x, getPosition().y - getDimensions().y / 2.75 - getVelocity().y),
        // Near left
        sf::Vector2f(getPosition().x - getDimensions().x / 2.75 + getVelocity().x, getPosition().y + getVelocity().y),
        // Near right
        sf::Vector2f(getPosition().x + getDimensions().x / 2.75 + getVelocity().x, getPosition().y + getVelocity().y)
    };
    // clang-format on

    // Cycle through the possible points to do a TileReaction on a Tile on one of those points, if found
    for (std::size_t i = 0; i < tileReactionPoints.size(); i++)
    {
        Tile* tile = m_map.getTilePtr(m_map.coordsToTileIndex(tileReactionPoints[i]), MapLayer::Solid);
        if (tile != nullptr)
        {
            tileReaction(tile);
            m_tileReactionDot.setPosition(tileReactionPoints[i]);
            break;
        }
        // If no valid Tile is found, perform TileReaction on first point (equivalent to nullptr) anyway
        // (to reset Entity state)
        if (i == tileReactionPoints.size() - 1)
        {
            m_tileReactionDot.setPosition(tileReactionPoints.front());
            tileReaction(nullptr);
        }
    }

    // Reactions with Entities
    // ---

    // Collisions
    m_isOnGround = false;
    performCollisions();

    setPosition(getPosition() + getVelocity());

    // States
    if (m_state != EntityState::Climbing || m_isOnGround == true)
    {
        if (m_inputDirection.x != 0.0 && m_isOnGround == true)
        {
            m_state = EntityState::Running;
        }
        else if (m_isOnGround == false)
        {
            if (getVelocity().y >= 0)
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
        // Set animation speed proportionally to the input magnitude
        if (m_state == EntityState::Running)
        {
            it->second.setFrameDuration(100.0 / std::abs(m_inputDirection.x));
        }
        if (m_state == EntityState::Climbing && m_inputDirection.y != 0)
        {
            it->second.setFrameDuration(100.0 / std::abs(m_inputDirection.y) * 2.0);
        }

        if (m_state != EntityState::Climbing)
        {
            it->second.setFlipped(!m_isFacingRight);
        }
        it->second.update();
        it->second.play();
    }

    m_collisionBox.setPosition(getPosition());
}

void Entity::interpolate(float lag)
{
    sf::Vector2f position = m_previousPosition + (m_position - m_previousPosition) * lag;
    auto it = m_animatedSprites.find(m_state);
    if (it != m_animatedSprites.cend())
    {
        it->second.setPosition(position);
    }
    else
    {
        m_defaultSprite.setPosition(position);
    }
}

void Entity::setStateAnimation(EntityState targetState, const AnimatedSprite& animatedSprite, float frameDuration, bool isLoopingEnabled)
{
    auto it = m_animatedSprites.find(targetState);
    // If animation is already set to a state, overwrite it
    if (it != m_animatedSprites.end())
    {
        it->second = animatedSprite;
    }
    else // Otherwise, add it to the map of animations
    {
        it = m_animatedSprites.emplace(targetState, animatedSprite).first;
    }

    // Set animation settings
    it->second.setFrameDuration(frameDuration);
    it->second.setLoopingEnabled(isLoopingEnabled);
    it->second.setPosition(m_position);
}

void Entity::setStateAnimation(EntityState targetState, AnimatedSprite&& animatedSprite, float frameDuration, bool isLoopingEnabled)
{
    auto it = m_animatedSprites.find(targetState);
    // If animation is already set to a state, overwrite it
    if (it != m_animatedSprites.end())
    {
        it->second = animatedSprite;
    }
    else // Otherwise, add it to the map of animations
    {
        it = m_animatedSprites.emplace(targetState, std::move(animatedSprite)).first;
    }

    // Set animation settings
    it->second.setFrameDuration(frameDuration);
    it->second.setLoopingEnabled(isLoopingEnabled);
    it->second.setPosition(m_position);
}

void Entity::setDefaultSpriteTexture(const sf::Texture& texture)
{
    m_defaultSprite.setTexture(texture);
    m_defaultSprite.setOrigin(static_cast<sf::Vector2f>(texture.getSize()) / 2.0f);
    m_defaultSprite.setPosition(m_position);
}

std::string Entity::getEntityTypeString(EntityType entityType)
{
    static const std::unordered_map<EntityType, std::string> entityTypeStrings = {{EntityType::Player, "Player"}};

    auto it = entityTypeStrings.find(entityType);
    if (it != entityTypeStrings.cend())
    {
        return it->second;
    }

    return "Unknown EntityType";
}

std::vector<std::string> Entity::getTextureNames(EntityType entityType)
{
    static const std::unordered_map<EntityType, std::vector<std::string>> entityTextures = {
        {EntityType::Player, {"characterStill", "characterRunning", "characterClimbing", "characterJumping", "characterFalling"}}};

    auto it = entityTextures.find(entityType);
    if (it != entityTextures.cend())
    {
        return it->second;
    }

    return {};
}

// Functions used for getting Entity's pixel position bounds

float Entity::getLeftPixelPosition() const
{
    return m_position.x - m_dimensions.x / 2;
}

float Entity::getRightPixelPosition() const
{
    return m_position.x + m_dimensions.x / 2 - 1; // Solve off-by-one error caused by pixel width
}

float Entity::getTopPixelPosition() const
{
    return m_position.y - m_dimensions.y / 2;
}

float Entity::getBottomPixelPosition() const
{
    return m_position.y + m_dimensions.y / 2 - 1; // Solve off-by-one error caused by pixel width
}
