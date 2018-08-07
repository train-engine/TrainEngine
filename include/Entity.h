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
protected:
    static float s_gravity;
    
    Map& m_rMap;
    std::vector<Entity*>& m_rEntities;

    EntityType m_entityType;
    EntityState m_state;

    sf::Sprite m_defaultSprite;
    std::unordered_map<EntityState, AnimatedSprite> m_animatedSprites;
    sf::RectangleShape m_collisionBox;
    bool m_displayCollisionBox;
    
    sf::Vector2f m_position;
    sf::Vector2f m_previousPosition;
    sf::Vector2f m_dimensions;
    sf::Vector2f m_velocity;

    const sf::Vector2f m_maxVelocity;

    float m_acceleration;
    float m_deceleration;

    bool m_isTileCollideable;
    bool m_isEntityCollideable;
    const unsigned int m_collisionPriority;

    bool m_isGoingRight;
    bool m_applyGravity;
    bool m_isOnGround;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void ApplyDeceleration();
    void ApplyGravity();
    void MaxVelocityCap();
    void MapEdgeCollision(bool applyHorizCollision = true, bool applyVertCollision = true);
    void PerformCollisions();

    // Collision with Tiles
    void TileCollision(const Tile* const pTile);

    // TileCollision functions
    virtual void StandardCollision(const Tile* const pTile);
    virtual void LadderTopCollision(const Tile* const pTile);

    // Collision with Entities
    void EntityCollision(const Entity* const pEntity);

    // EntityCollision functions
    // ---

public:
    // Constructor and destructor
    Entity(Map& rMap, std::vector<Entity*>& rEntities, EntityType entityType,
           const sf::Vector2f& position, const sf::Vector2f& dimensions, const sf::Vector2f& maxVelocity = {10, 64},
           float acceleration = 1, float deceleration = 3, bool applyGravity = false,
           bool isTileCollideable = false, bool isEntityCollideable = false, unsigned int collisionPriority = 0);
    virtual ~Entity() {}

    // Functions
    virtual void HandleInput() {}
    virtual void Update();
    virtual void Interpolate(float lag);
    
    void AddAnimation(EntityState targetState, const AnimatedSprite& animatedSprite);
    void AddAnimation(EntityState targetState, AnimatedSprite&& animatedSprite);

    // Setters
    void ToggleCollisionBoxDisplay() {m_displayCollisionBox = !m_displayCollisionBox;}
    void SetPosition(const sf::Vector2f& position);
    void SetHorizPosition(float horizPosition) {m_position.x = horizPosition;}
    void SetVertPosition(float vertPosition) {m_position.y = vertPosition;}
    void SetDimensions(const sf::Vector2f& dimensions);

    void ResetVelocity();
    void ResetHorizVelocity();
    void ResetVertVelocity();

    void SetDefaultSpriteTexture(const sf::Texture& texture);
    void SetOnGround(bool isOnGround) {m_isOnGround = isOnGround;}

    // Getters
    static std::string GetEntityTypeString(EntityType entityType);
    static std::vector<std::string> GetTextureNames(EntityType entityType);
    EntityType GetEntityType() const {return m_entityType;}
    const sf::Vector2f& GetPosition() const {return m_position;}
    const sf::Vector2f& GetDimensions() const {return m_dimensions;}
    const sf::Vector2f& GetVelocity() const {return m_velocity;}
    bool IsTileCollideable() const {return m_isTileCollideable;}
    bool IsEntityCollideable() const {return m_isEntityCollideable;}
    const unsigned int GetCollisionPriority() const {return m_collisionPriority;}
    
    float GetLeftPixelPosition() const;
    float GetRightPixelPosition() const;
    float GetTopPixelPosition() const;
    float GetBottomPixelPosition() const;
};

#endif // ENTITY_H_INCLUDED
