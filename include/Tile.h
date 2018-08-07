#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <SFML/Graphics.hpp>

// How and when to implement derived Tile classes:
//
// The only time when you should create a derived Tile class is when this class needs to perform functions that the default Tile class does not,
// such as animations.
// The default Tile class will suffice for any static block with no animations or interactions with other Tiles, regardless of its Texture.
//
// When defining a derived Tile class, add all of the derived class's specific functions in the Update() function.
// You may also rewrite the draw() function if you wish.
// Of course, the constructor and destructor of the derived class can be changed, and most likely will be.
// Do not forget to add an entry to the TileType enum for your new Tile, and update the Map::AddTile() function switch statement accordingly.

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
protected:
    TileType m_tileType;

    sf::Sprite m_sprite;

    sf::Vector2f m_position;
    sf::Vector2f m_dimensions;

    bool m_isSolid;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;

public:
    // Constructor and destructor
    Tile(const sf::Texture& texture, TileType tileType, bool isSolid = false);
    virtual ~Tile() {}

    // Getters
    static std::string GetTileTypeString(TileType tileType);
    static std::string GetTextureName(TileType tileType);
    TileType GetTileType() const {return m_tileType;}
    const sf::Vector2f& GetPosition() const {return m_position;}
    const sf::Vector2f& GetDimensions() const {return m_dimensions;}
    bool IsSolid() const {return m_isSolid;}

    // Setters
    void SetPosition(const sf::Vector2f& position);
    void SetDimensions(const sf::Vector2f& dimensions);
    void SetTexture(const sf::Texture& texture);
    void SetTileType(TileType tileType) {m_tileType = tileType;}
    void SetColor(sf::Color color) {m_sprite.setColor(color);}
};

#endif // TILE_H_INCLUDED
