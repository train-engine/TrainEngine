#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "Map.h"
#include "Tile.h"

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

    sf::Sprite m_defaultSprite;
    std::unordered_map<EntityState, AnimatedSprite> m_animatedSprites;

    sf::RectangleShape m_collisionBox;
    sf::RectangleShape m_tileReactionDot;
    bool m_showDebugBox;

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
    bool m_canGravitate;
    bool m_isOnGround;

    bool m_isPressingUp;
    bool m_isPressingDown;
    bool m_isPressingLeft;
    bool m_isPressingRight;
    bool m_isPressingShift;

    float m_jumpForce;
    float m_defaultClimbSpeed;
    float m_defaultDescentSpeed;

    // Collisions and interactions
    void TileCollision(const Tile* pTile);
    void EntityCollision(const Entity* pEntity);
    void TileReaction(Tile* pTile);
    void EntityReaction(Entity* pEntity);

    // TileCollision functions (overrideable)
    virtual void StandardCollision(const Tile* pTile);
    virtual void LadderTopCollision(const Tile* pTile);

    // EntityCollision functions (overrideable)
    // ---

    // TileReaction functions (overrideable)
    // ---

    // EntityReaction functions (overrideable)
    virtual void PlayerReaction(Entity* pEntity) {}

    // Movement
    void MoveLeft();
    void MoveRight();
    void Jump();
    void Climb(float factor = 1);

protected:
    Map& m_rMap;
    std::vector<Entity*>& m_rEntities;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void ApplyDeceleration();
    void ApplyGravity();
    void MaxVelocityCap();
    void MapEdgeCollision(bool applyHorizCollision = true, bool applyVertCollision = true);
    void PerformCollisions();

    // Setters
    void SetPressingUp(bool isPressingUp) {m_isPressingUp = isPressingUp;}
    void SetPressingDown(bool isPressingDown) {m_isPressingDown = isPressingDown;}
    void SetPressingLeft(bool isPressingLeft) {m_isPressingLeft = isPressingLeft;}
    void SetPressingRight(bool isPressingRight) {m_isPressingRight = isPressingRight;}
    void SetPressingShift(bool isPressingShift) {m_isPressingShift = isPressingShift;}

    // Getters
    bool IsOnGround() const {return m_isOnGround;}

    bool IsPressingUp() const {return m_isPressingUp;}
    bool IsPressingDown() const {return m_isPressingDown;}
    bool IsPressingLeft() const {return m_isPressingLeft;}
    bool IsPressingRight() const {return m_isPressingRight;}
    bool IsPressingShift() const {return m_isPressingShift;}

    float GetJumpForce() const {return m_jumpForce;}
    float GetDefaultClimbSpeed() const {return m_defaultClimbSpeed;}
    float GetDefaultDescentSpeed() const {return m_defaultDescentSpeed;}

public:
    // Constructor and destructor
    Entity(Map& rMap, std::vector<Entity*>& rEntities, EntityType entityType,
           const sf::Vector2f& position, const sf::Vector2f& dimensions, const sf::Vector2f& maxVelocity = {10, 64},
           float acceleration = 1, float deceleration = 3, float jumpForce = 18, bool applyGravity = true,
           bool isTileCollideable = false, bool isEntityCollideable = false);
    virtual ~Entity() {}

    // Functions
    virtual void HandleInput() {}
    virtual void Update();
    virtual void Interpolate(float lag);
    
    void AddAnimation(EntityState targetState, const AnimatedSprite& animatedSprite);
    void AddAnimation(EntityState targetState, AnimatedSprite&& animatedSprite);

    // Setters
    void SetShowDebugBox(bool showDebugBox) {m_showDebugBox = showDebugBox;}

    void SetPosition(const sf::Vector2f& position) {m_position = position;}
    void SetHorizPosition(float horizPosition) {m_position.x = horizPosition;}
    void SetVertPosition(float vertPosition) {m_position.y = vertPosition;}

    void SetDimensions(const sf::Vector2f& dimensions) {m_dimensions = dimensions;}

    void SetVelocity(const sf::Vector2f& velocity) {m_velocity = velocity;}
    void SetHorizVelocity(float horizVelocity) {m_velocity.x = horizVelocity;}
    void SetVertVelocity(float vertVelocity) {m_velocity.y = vertVelocity;}

    void SetDefaultSpriteTexture(const sf::Texture& texture);

    // Getters
    EntityType GetEntityType() const {return m_entityType;}
    static std::string GetEntityTypeString(EntityType entityType);
    static std::vector<std::string> GetTextureNames(EntityType entityType);

    const sf::Vector2f& GetPosition() const {return m_position;}
    const sf::Vector2f& GetDimensions() const {return m_dimensions;}
    const sf::Vector2f& GetVelocity() const {return m_velocity;}
    
    float GetLeftPixelPosition() const;
    float GetRightPixelPosition() const;
    float GetTopPixelPosition() const;
    float GetBottomPixelPosition() const;
};

#endif // ENTITY_H_INCLUDED
