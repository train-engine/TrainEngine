#include "Level/Tile.h"
#include <unordered_map>

Tile::Tile(const sf::Texture& texture, TileType tileType)
    : m_tileType(tileType)
    , m_sprite(texture)
    , m_isSolid(true)
{
}

void Tile::draw(sf::RenderTarget& rTarget, sf::RenderStates states) const
{
    rTarget.draw(m_sprite, states);
}

std::string Tile::GetTileTypeString(TileType tileType)
{
    static const std::unordered_map<TileType, std::string> tileTypeStrings = {{TileType::Grass4Sides, "Grass"},
                                                                              {TileType::Wood, "Wood"},
                                                                              {TileType::Ladder, "Ladder"},
                                                                              {TileType::LadderTop, "Ladder top"},
                                                                              {TileType::Vine, "Vine"},
                                                                              {TileType::Post, "Post"}};

    auto it = tileTypeStrings.find(tileType);
    if (it != tileTypeStrings.cend())
    {
        return it->second;
    }

    return "Unknown TileType";
}

std::string Tile::GetTextureName(TileType tileType)
{
    static const std::unordered_map<TileType, std::string> tileTextures = {{TileType::GrassTopLeftSides, "grassTopLeftSides"},
                                                                           {TileType::GrassTopSide, "grassTopSide"},
                                                                           {TileType::GrassTopRightSides, "grassTopRightSides"},
                                                                           {TileType::GrassLeftSide, "grassLeftSide"},
                                                                           {TileType::GrassNoSides, "grassNoSides"},
                                                                           {TileType::GrassRightSide, "grassRightSide"},
                                                                           {TileType::GrassBotLeftSide, "grassBotLeftSides"},
                                                                           {TileType::GrassBotSide, "grassBotSide"},
                                                                           {TileType::GrassBotRightSides, "grassBotRightSides"},
                                                                           {TileType::GrassTopLeftRightSides, "grassTopLeftRightSides"},
                                                                           {TileType::GrassLeftRightSides, "grassLeftRightSides"},
                                                                           {TileType::GrassBotLeftRightSides, "grassBotLeftRightSides"},
                                                                           {TileType::GrassTopBotLeftSides, "grassTopBotLeftSides"},
                                                                           {TileType::GrassTopBotSides, "grassTopBotSides"},
                                                                           {TileType::GrassTopBotRightSides, "grassTopBotRightSides"},
                                                                           {TileType::Grass4Sides, "grass4Sides"},
                                                                           {TileType::GrassTopLeftSidesCorner3, "grassTopLeftSidesCorner3"},
                                                                           {TileType::GrassTopSideCorner3, "grassTopSideCorner3"},
                                                                           {TileType::GrassTopSideCorner4, "grassTopSideCorner4"},
                                                                           {TileType::GrassTopRightSidesCorner4,
                                                                            "grassTopRightSidesCorner4"},
                                                                           {TileType::GrassLeftSideCorner3, "grassLeftSideCorner3"},
                                                                           {TileType::GrassNoSidesCorner3, "grassNoSidesCorner3"},
                                                                           {TileType::GrassNoSidesCorner4, "grassNoSidesCorner4"},
                                                                           {TileType::GrassRightSideCorner4, "grassRightSideCorner4"},
                                                                           {TileType::GrassLeftSideCorner2, "grassLeftSideCorner2"},
                                                                           {TileType::GrassNoSidesCorner2, "grassNoSidesCorner2"},
                                                                           {TileType::GrassNoSidesCorner1, "grassNoSidesCorner1"},
                                                                           {TileType::GrassRightSideCorner1, "grassRightSideCorner1"},
                                                                           {TileType::GrassBotLeftSidesCorner2, "grassBotLeftSidesCorner2"},
                                                                           {TileType::GrassBotSideCorner2, "grassBotSideCorner2"},
                                                                           {TileType::GrassBotSideCorner1, "grassBotSideCorner1"},
                                                                           {TileType::GrassBotRightSidesCorner1,
                                                                            "grassBotRightSidesCorner1"},
                                                                           {TileType::GrassNoSides4Corners, "grassNoSides4Corners"},
                                                                           {TileType::GrassNoSidesCorners12, "grassNoSidesCorners12"},
                                                                           {TileType::GrassNoSidesCorners34, "grassNoSidesCorners34"},
                                                                           {TileType::GrassNoSidesCorners14, "grassNoSidesCorners14"},
                                                                           {TileType::GrassNoSidesCorners23, "grassNoSidesCorners23"},
                                                                           {TileType::Wood, "wood"},
                                                                           {TileType::Ladder, "ladder"},
                                                                           {TileType::LadderTop, "ladder"},
                                                                           {TileType::Vine, "vine"},
                                                                           {TileType::Post, "post"}};

    auto it = tileTextures.find(tileType);
    if (it != tileTextures.cend())
    {
        return it->second;
    }

    return "";
}

void Tile::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_sprite.setPosition(m_position);
}

void Tile::SetDimensions(const sf::Vector2f& dimensions)
{
    m_dimensions = dimensions;
}

void Tile::SetTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}
