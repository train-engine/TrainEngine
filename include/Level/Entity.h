#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Level/Map.h"
#include "Level/Tile.h"
#include "Misc/AnimatedSprite.h"

enum class EntityType
{
    Player = 000
};

enum class EntityState
{
    Still,
    Running,
    Climbing,
    Jumping,
    Falling
};

class Entity : public sf::Drawable
{
private:
    EntityType m_entityType;
    EntityState m_state;

    // Graphics variables
    sf::Sprite m_defaultSprite;
    std::unordered_map<EntityState, AnimatedSprite> m_animatedSprites;

    // Debug information
    sf::RectangleShape m_collisionBox;
    sf::RectangleShape m_tileReactionDot;
    bool m_isDebugBoxVisible;

    // Physics variables
    sf::Vector2f m_position;
    sf::Vector2f m_previousPosition;
    sf::Vector2f m_dimensions;
    sf::Vector2f m_velocity;

    const sf::Vector2f m_maxVelocity;

    float m_acceleration;
    float m_deceleration;

    bool m_isTileCollideable;
    bool m_isEntityCollideable;

    bool m_isFacingRight;
    bool m_isGravityApplied;
    bool m_isOnGround;

    // Input controls
    sf::Vector2f m_inputDirection;
    bool m_isPressingUp;

    // Movement settings
    float m_jumpForce;
    float m_defaultClimbSpeed;
    float m_defaultDescentSpeed;

    // Collisions and interactions
    void tileCollision(const Tile* tile);
    void entityCollision(const Entity* entity);
    void tileReaction(Tile* tile);
    void entityReaction(Entity* entity);

    // TileCollision functions (overrideable)
    virtual void standardCollision(const Tile* tile);
    virtual void ladderTopCollision(const Tile* tile);

    // EntityCollision functions (overrideable)
    // ---

    // TileReaction functions (overrideable)
    // ---

    // EntityReaction functions (overrideable)
    virtual void playerReaction(Entity* entity) {}

    // Movement
    void moveLeft();
    void moveRight();
    void jump();
    void climb(float factor = 1);

protected:
    Map& m_map;
    std::vector<Entity*>& m_entities;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void applyDeceleration();
    void applyGravity();
    void maxVelocityCap();
    void mapEdgeCollision(bool isHorizCollisionEnabled = true, bool isVertCollisionEnabled = true);
    void performCollisions();

    // Setters
    void setHorizontalInputDirection(float horizInputDirection) { m_inputDirection.x = horizInputDirection; }
    void setVerticalInputDirection(float vertInputDirection) { m_inputDirection.y = vertInputDirection; }
    void setPressingUp(bool isPressingUp) { m_isPressingUp = isPressingUp; }

    // Getters
    bool isOnGround() const { return m_isOnGround; }

    float getHorizontalInputDirection() const { return m_inputDirection.x; }
    float getVerticalInputDirection() const { return m_inputDirection.y; }

    float getJumpForce() const { return m_jumpForce; }
    float getDefaultClimbSpeed() const { return m_defaultClimbSpeed; }
    float getDefaultDescentSpeed() const { return m_defaultDescentSpeed; }

public:
    // Constructor and destructor
    Entity(Map& map, std::vector<Entity*>& entities, EntityType entityType, const sf::Vector2f& position, const sf::Vector2f& dimensions,
           const sf::Vector2f& maxVelocity = {10, 64}, float acceleration = 1, float deceleration = 3, float jumpForce = 18,
           bool isGravityApplied = true, bool isTileCollideable = false, bool isEntityCollideable = false);
    virtual ~Entity() {}

    // Functions
    virtual void handleInput() {}
    virtual void update();
    virtual void interpolate(float lag);

    void setStateAnimation(EntityState targetState, const AnimatedSprite& animatedSprite, float frameDuration, bool isLoopingEnabled = true);
    void setStateAnimation(EntityState targetState, AnimatedSprite&& animatedSprite, float frameDuration, bool isLoopingEnabled = true);

    // Setters
    void setDebugBoxVisible(bool isDebugBoxVisible) { m_isDebugBoxVisible = isDebugBoxVisible; }

    void setPosition(const sf::Vector2f& position) { m_position = position; }
    void setHorizPosition(float horizPosition) { m_position.x = horizPosition; }
    void setVertPosition(float vertPosition) { m_position.y = vertPosition; }

    void setDimensions(const sf::Vector2f& dimensions) { m_dimensions = dimensions; }

    void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
    void setHorizVelocity(float horizVelocity) { m_velocity.x = horizVelocity; }
    void setVertVelocity(float vertVelocity) { m_velocity.y = vertVelocity; }

    void setDefaultSpriteTexture(const sf::Texture& texture);

    // Getters
    EntityType getEntityType() const { return m_entityType; }
    static std::string getEntityTypeString(EntityType entityType);
    static std::vector<std::string> getTextureNames(EntityType entityType);

    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getDimensions() const { return m_dimensions; }
    const sf::Vector2f& getVelocity() const { return m_velocity; }

    float getLeftPixelPosition() const;
    float getRightPixelPosition() const;
    float getTopPixelPosition() const;
    float getBottomPixelPosition() const;
};

#endif // ENTITY_H
