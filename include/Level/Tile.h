#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

enum class TileType
{
    GrassTopLeftSides = 100,
    GrassTopSide = 101,
    GrassTopRightSides = 102,
    GrassLeftSide = 103,
    GrassNoSides = 104,
    GrassRightSide = 105,
    GrassBotLeftSide = 106,
    GrassBotSide = 107,
    GrassBotRightSides = 108,
    GrassTopLeftRightSides = 109,
    GrassLeftRightSides = 110,
    GrassBotLeftRightSides = 111,
    GrassTopBotLeftSides = 112,
    GrassTopBotSides = 113,
    GrassTopBotRightSides = 114,
    Grass4Sides = 115,
    GrassTopLeftSidesCorner3 = 116,
    GrassTopSideCorner3 = 117,
    GrassTopSideCorner4 = 118,
    GrassTopRightSidesCorner4 = 119,
    GrassLeftSideCorner3 = 120,
    GrassNoSidesCorner3 = 121,
    GrassNoSidesCorner4 = 122,
    GrassRightSideCorner4 = 123,
    GrassLeftSideCorner2 = 124,
    GrassNoSidesCorner2 = 125,
    GrassNoSidesCorner1 = 126,
    GrassRightSideCorner1 = 127,
    GrassBotLeftSidesCorner2 = 128,
    GrassBotSideCorner2 = 129,
    GrassBotSideCorner1 = 130,
    GrassBotRightSidesCorner1 = 131,
    GrassNoSides4Corners = 132,
    GrassNoSidesCorners12 = 133,
    GrassNoSidesCorners34 = 134,
    GrassNoSidesCorners14 = 135,
    GrassNoSidesCorners23 = 136,
    Wood = 150,

    Ladder = 200,
    LadderTop = 201,

    Vine = 500,
    Post = 501
};

class Tile : public sf::Drawable
{
private:
    TileType m_tileType;

    sf::Sprite m_sprite;

    sf::Vector2f m_position;
    sf::Vector2f m_dimensions;

    bool m_isSolid;

protected:
    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;

public:
    // Constructor and destructor
    Tile(const sf::Texture& texture, TileType tileType);
    virtual ~Tile() {}

    // Setters
    void SetTileType(TileType tileType) { m_tileType = tileType; }
    void SetTexture(const sf::Texture& texture);
    void SetPosition(const sf::Vector2f& position);
    void SetDimensions(const sf::Vector2f& dimensions);
    void SetSolid(bool isSolid) { m_isSolid = isSolid; }
    void SetColor(sf::Color color) { m_sprite.setColor(color); }

    // Getters
    TileType GetTileType() const { return m_tileType; }
    static std::string GetTileTypeString(TileType tileType);
    static std::string GetTextureName(TileType tileType);
    const sf::Vector2f& GetPosition() const { return m_position; }
    const sf::Vector2f& GetDimensions() const { return m_dimensions; }
    bool IsSolid() const { return m_isSolid; }
};

#endif // TILE_H
