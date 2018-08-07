#ifndef GRASSBLOCK_H_INCLUDED
#define GRASSBLOCK_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "Tile.h"

class GrassBlock : public Tile
{
public:
    // Constructor
    GrassBlock(const sf::Texture& texture, TileType type = TileType::Grass4Sides);
};

#endif // GRASSBLOCK_H_INCLUDED
