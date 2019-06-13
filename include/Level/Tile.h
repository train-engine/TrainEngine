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
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    // Constructor and destructor
    Tile(const sf::Texture& texture, TileType tileType);
    virtual ~Tile() {}

    // Setters
    void setTileType(TileType tileType) { m_tileType = tileType; }
    void setTexture(const sf::Texture& texture);
    void setPosition(const sf::Vector2f& position);
    void setDimensions(const sf::Vector2f& dimensions);
    void setSolid(bool isSolid) { m_isSolid = isSolid; }
    void setColor(sf::Color color) { m_sprite.setColor(color); }

    // Getters
    TileType getTileType() const { return m_tileType; }
    static std::string getTileTypeString(TileType tileType);
    static std::string getTextureName(TileType tileType);
    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getDimensions() const { return m_dimensions; }
    bool isSolid() const { return m_isSolid; }
};

#endif // TILE_H
