#include "Level/Map.h"
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include "Core/FileManager.h"
#include "Level/Tile.h"

namespace
{
    const sf::Vector2f maxDimensions(4096, 4096);
} // namespace

Map::Map(const ResourceManager& resourceManager)
    : m_resourceManager(resourceManager)
    , m_indexDimensions(0, 0)
    , m_layerCount(static_cast<unsigned int>(MapLayer::Count))
    , m_tileSize(64)
    , m_isGridVisible(false)
{
    m_tiles.resize(m_layerCount);

    m_horizGridLine.setFillColor(sf::Color(255, 255, 255, 128));
    m_vertGridLine.setFillColor(sf::Color(255, 255, 255, 128));

    m_layerColors.fill(sf::Color::White);
}

Map::~Map()
{
    clear();
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2f viewPosition = target.getView().getCenter();
    sf::Vector2f viewDimensions = target.getView().getSize();

    float viewLeft = (viewPosition.x - viewDimensions.x / 2) / m_tileSize;
    if (viewLeft < 0)
    {
        viewLeft = 0;
    }

    float viewRight = (viewPosition.x + viewDimensions.x / 2) / m_tileSize;
    if (viewRight > m_indexDimensions.x)
    {
        viewRight = m_indexDimensions.x;
    }

    float viewTop = (viewPosition.y - viewDimensions.y / 2) / m_tileSize;
    if (viewTop < 0)
    {
        viewTop = 0;
    }

    float viewBottom = (viewPosition.y + viewDimensions.y / 2) / m_tileSize;
    if (viewBottom > m_indexDimensions.y)
    {
        viewBottom = m_indexDimensions.y;
    }

    for (unsigned int z = 0; z < m_layerCount; z++)
    {
        for (unsigned int y = viewTop; y < viewBottom; y++)
        {
            for (unsigned int x = viewLeft; x < viewRight; x++)
            {
                if (m_tiles[z][y][x] != nullptr)
                {
                    target.draw(*m_tiles[z][y][x], states);
                }
            }
        }
    }

    if (m_isGridVisible == true)
    {
        drawGrid(target, states);
    }
}

// Draw grid lines around Tiles
void Map::drawGrid(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2f viewPosition = target.getView().getCenter();
    sf::Vector2f viewDimensions = target.getView().getSize();

    m_horizGridLine.setSize({viewDimensions.x, 2});
    m_vertGridLine.setSize({2, viewDimensions.y});

    // Set initial horizontal grid line position
    m_horizGridLine.setPosition(viewPosition.x - viewDimensions.x / 2,
                                viewPosition.y - viewDimensions.y / 2 -
                                    static_cast<int>(viewPosition.y - viewDimensions.y / 2) % static_cast<int>(m_tileSize) - 1);
    if (m_horizGridLine.getPosition().y < -1)
    {
        m_horizGridLine.setPosition(m_horizGridLine.getPosition().x, -1);
    }
    if (m_horizGridLine.getPosition().x < 0)
    {
        m_horizGridLine.setPosition(0, m_horizGridLine.getPosition().y);
    }
    if (m_horizGridLine.getPosition().x + m_horizGridLine.getSize().x > getBounds().x)
    {
        m_horizGridLine.setSize(sf::Vector2f(getBounds().x - m_horizGridLine.getPosition().x, 2));
    }

    // Set initial vertical grid line position
    m_vertGridLine.setPosition(viewPosition.x - viewDimensions.x / 2 -
                                   static_cast<int>(viewPosition.x - viewDimensions.x / 2) % static_cast<int>(m_tileSize) - 1,
                               viewPosition.y - viewDimensions.y / 2);
    if (m_vertGridLine.getPosition().x < -1)
    {
        m_vertGridLine.setPosition(-1, m_vertGridLine.getPosition().y);
    }
    if (m_vertGridLine.getPosition().y < 0)
    {
        m_vertGridLine.setPosition(m_vertGridLine.getPosition().x, 0);
    }
    if (m_vertGridLine.getPosition().y + m_vertGridLine.getSize().y > getBounds().y)
    {
        m_vertGridLine.setSize(sf::Vector2f(2, getBounds().y - m_vertGridLine.getPosition().y));
    }

    // Draw and move grid lines incrementally from their starting position
    while (m_horizGridLine.getPosition().y <= viewPosition.y + viewDimensions.y / 2 && m_horizGridLine.getPosition().y <= getBounds().y)
    {
        target.draw(m_horizGridLine, states);
        m_horizGridLine.setPosition(m_horizGridLine.getPosition() + sf::Vector2f(0, m_tileSize));
    }
    while (m_vertGridLine.getPosition().x <= viewPosition.x + viewDimensions.x / 2 && m_vertGridLine.getPosition().x <= getBounds().x)
    {
        target.draw(m_vertGridLine, states);
        m_vertGridLine.setPosition(m_vertGridLine.getPosition() + sf::Vector2f(m_tileSize, 0));
    }
}

void Map::update()
{
}

// Load the Map from a save file
bool Map::load(const std::string& filename)
{
    // First delete all elements of the vector (necessary when changing level)
    clear();

    std::ifstream inputFile(FileManager::resourcePath() + filename);
    if (inputFile)
    {
        std::cout << "Loading Map...\n";
        inputFile >> m_indexDimensions.x >> m_indexDimensions.y >> m_tileSize;
        std::cout << "Dimensions:\t" << m_indexDimensions.x << 'x' << m_indexDimensions.y << '\n';
        std::cout << "TileSize:\t" << m_tileSize << '\n';

        // Vector resizing
        for (unsigned int z = 0; z < m_layerCount; z++)
        {
            m_tiles[z].resize(m_indexDimensions.y);
            for (unsigned int y = 0; y < m_indexDimensions.y; y++)
            {
                m_tiles[z][y].resize(m_indexDimensions.x);
            }
        }

        // Vector assigning
        std::cout << "Tile map:\n";
        for (unsigned int z = 0; z < m_layerCount; z++)
        {
            std::cout << '#' << z << ": \n";
            // Ignore all chars until '\n' to peek next character properly
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (inputFile.peek() == '-') // Empty layer
            {
                for (unsigned int y = 0; y < m_indexDimensions.y; y++)
                {
                    for (unsigned int x = 0; x < m_indexDimensions.x; x++)
                    {
                        m_tiles[z][y][x] = nullptr;
                    }
                }
                inputFile.ignore(); // Remove the '-' from the buffer to continue reading

                std::cout << "(empty)\n";
                continue;
            }

            for (unsigned int y = 0; y < m_indexDimensions.y; y++)
            {
                bool hasReachedSemiColon = false;
                for (unsigned int x = 0; x < m_indexDimensions.x; x++)
                {
                    // If a semicolon has not been reached, read the next group of characters
                    if (hasReachedSemiColon == false)
                    {
                        std::string input;
                        inputFile >> input;
                        if (input == ";")
                        {
                            hasReachedSemiColon = true;
                        }
                        else
                        {
                            if (input.empty() || input.size() > 3 ||
                                std::find_if(input.cbegin(), input.cend(), [](char c) { return std::isdigit(c) == false; }) != input.cend())
                            {
                                std::cerr << "\nMap error: Parsing tile type failed in file: \"" << filename << "\".\n"
                                          << "Map loading failed.\n\n";
                                return false;
                            }

                            int type = std::stoi(input);
                            addTile(static_cast<TileType>(type), sf::Vector2u(x, y), static_cast<MapLayer>(z), false);
                        }
                    }
                    // If a semicolon has been reached (possibly in this exact loop), set this index to a null pointer
                    if (hasReachedSemiColon == true)
                    {
                        m_tiles[z][y][x] = nullptr;
                    }

                    if (m_tiles[z][y][x] != nullptr)
                    {
                        std::cout << static_cast<int>(m_tiles[z][y][x]->getTileType()) << ' ';
                    }
                    else
                    {
                        std::cout << "--- ";
                    }
                }
                std::cout << '\n';
            }
        }

        std::cout << "Map successfully loaded.\n\n";
        return true;
    }

    std::cerr << "Map error: Unable to open \"" << filename << "\".\n"
              << "Map loading failed.\n\n";
    return false;
}

// Save the Map to a save file
bool Map::save(const std::string& filename) const
{
    std::ofstream outputFile(FileManager::resourcePath() + filename);
    if (outputFile)
    {
        std::cout << "Saving Map...\n";
        outputFile << m_indexDimensions.x << '\n' << m_indexDimensions.y << '\n' << m_tileSize << '\n';
        std::cout << "Dimensions:\t" << m_indexDimensions.x << 'x' << m_indexDimensions.y << '\n';
        std::cout << "TileSize:\t" << m_tileSize << '\n';

        for (unsigned int z = 0; z < m_layerCount; z++)
        {
            std::string layerOutput;
            bool isEmptyLayer = true;

            if (z != static_cast<unsigned int>(MapLayer::Overlay)) // Do not save overlay (count as empty layer)
            {
                for (unsigned int y = 0; y < m_indexDimensions.y; y++)
                {
                    for (unsigned int x = 0; x < m_indexDimensions.x; x++)
                    {
                        if (m_tiles[z][y][x] == nullptr)
                        {
                            layerOutput += "000";
                        }
                        else
                        {
                            layerOutput += std::to_string(static_cast<int>(m_tiles[z][y][x]->getTileType()));
                            isEmptyLayer = false;
                        }
                        if (x + 1 < m_indexDimensions.x)
                        {
                            layerOutput += ' ';
                        }
                    }
                    layerOutput += '\n';
                }
            }

            if (isEmptyLayer == false)
            {
                outputFile << layerOutput;
            }
            else
            {
                outputFile << "-\n";
            }
        }

        std::cout << "Map successfully saved.\n\n";
        return true;
    }

    std::cerr << "Map error: Unable to save \"" << filename << "\".\n"
              << "Map saving failed.\n\n";
    return false;
}

// Convert world coordinates to a Tile index
sf::Vector2u Map::coordsToTileIndex(const sf::Vector2f& position) const
{
    return sf::Vector2u(position.x / m_tileSize, position.y / m_tileSize);
}

// Convert a Tile index to world coordinates
sf::Vector2f Map::tileIndexToCoords(const sf::Vector2u& position) const
{
    return sf::Vector2f(position.x * m_tileSize, position.y * m_tileSize);
}

// Update a Tile's texture according to surrounding Tiles
void Map::updateTileTexture(const sf::Vector2u& tileIndex, MapLayer layer)
{
    if (layer == MapLayer::Count)
    {
        return;
    }

    unsigned int x = tileIndex.x;
    unsigned int y = tileIndex.y;
    unsigned int z = static_cast<unsigned int>(layer);

    // If index is outside of Map bounds
    if (x >= m_indexDimensions.x || y >= m_indexDimensions.y)
    {
        return;
    }

    // If Grass-like Tile
    if (m_tiles[z][y][x] != nullptr && m_tiles[z][y][x]->getTileType() >= TileType::GrassTopLeftSides &&
        m_tiles[z][y][x]->getTileType() <= TileType::GrassNoSidesCorners23)
    {
        bool isTopLeftEmpty = true;
        bool isTopEmpty = true;
        bool isTopRightEmpty = true;
        bool isRightEmpty = true;
        bool isBottomRightEmpty = true;
        bool isBottomEmpty = true;
        bool isBottomLeftEmpty = true;
        bool isLeftEmpty = true;

        if (x > 0)
        {
            if (y > 0)
            {
                isTopLeftEmpty = (m_tiles[z][y - 1][x - 1] == nullptr || m_tiles[z][y - 1][x - 1]->isSolid() == false);
            }
            else
            {
                isTopLeftEmpty = false;
            }

            if (y < m_indexDimensions.y - 1)
            {
                isBottomLeftEmpty = (m_tiles[z][y + 1][x - 1] == nullptr || m_tiles[z][y + 1][x - 1]->isSolid() == false);
            }
            else
            {
                isBottomLeftEmpty = false;
            }

            isLeftEmpty = (m_tiles[z][y][x - 1] == nullptr || m_tiles[z][y][x - 1]->isSolid() == false);
        }
        else
        {
            isTopLeftEmpty = false;
            isBottomLeftEmpty = false;
            isLeftEmpty = false;
        }

        if (x < m_indexDimensions.x - 1)
        {
            if (y > 0)
            {
                isTopRightEmpty = (m_tiles[z][y - 1][x + 1] == nullptr || m_tiles[z][y - 1][x + 1]->isSolid() == false);
            }
            else
            {
                isTopRightEmpty = false;
            }

            if (y < m_indexDimensions.y - 1)
            {
                isBottomRightEmpty = (m_tiles[z][y + 1][x + 1] == nullptr || m_tiles[z][y + 1][x + 1]->isSolid() == false);
            }
            else
            {
                isBottomRightEmpty = false;
            }

            isRightEmpty = (m_tiles[z][y][x + 1] == nullptr || m_tiles[z][y][x + 1]->isSolid() == false);
        }
        else
        {
            isTopRightEmpty = false;
            isBottomRightEmpty = false;
            isRightEmpty = false;
        }

        if (y > 0)
        {
            isTopEmpty = (m_tiles[z][y - 1][x] == nullptr || m_tiles[z][y - 1][x]->isSolid() == false);
        }
        else
        {
            isTopEmpty = false;
        }

        if (y < m_indexDimensions.y - 1)
        {
            isBottomEmpty = (m_tiles[z][y + 1][x] == nullptr || m_tiles[z][y + 1][x]->isSolid() == false);
        }
        else
        {
            isBottomEmpty = false;
        }

        if (isTopEmpty == true && isLeftEmpty == true && isRightEmpty == false && isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassTopLeftSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == false && isRightEmpty == false && isBottomEmpty == false &&
                 ((isBottomRightEmpty == false && isBottomLeftEmpty == false) || (isBottomLeftEmpty == true && isBottomRightEmpty == true)))
        {
            addTile(TileType::GrassTopSide, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == false && isRightEmpty == true && isBottomLeftEmpty == false && isBottomEmpty == false)
        {
            addTile(TileType::GrassTopRightSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false &&
                 ((isTopRightEmpty == false && isBottomRightEmpty == false) || (isTopRightEmpty == true && isBottomRightEmpty == true)) &&
                 isLeftEmpty == true && isRightEmpty == false && isBottomEmpty == false)
        {
            addTile(TileType::GrassLeftSide, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == true &&
                 isRightEmpty == false && isBottomLeftEmpty == false && isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassNoSides, sf::Vector2u(x, y), layer);
        }
        else if (((isTopLeftEmpty == false && isBottomLeftEmpty == false) || (isTopLeftEmpty == true && isBottomLeftEmpty == true)) &&
                 isTopEmpty == false && isLeftEmpty == false && isRightEmpty == true && isBottomEmpty == false)
        {
            addTile(TileType::GrassRightSide, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == true && isRightEmpty == false && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotLeftSide, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false &&
                 ((isTopRightEmpty == false && isTopLeftEmpty == false) || (isTopRightEmpty == true && isTopLeftEmpty == true)) &&
                 isLeftEmpty == false && isRightEmpty == false && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotSide, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isLeftEmpty == false && isRightEmpty == true && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotRightSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == true && isRightEmpty == true && isBottomEmpty == false)
        {
            addTile(TileType::GrassTopLeftRightSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false && isLeftEmpty == true && isRightEmpty == true && isBottomEmpty == false)
        {
            addTile(TileType::GrassLeftRightSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false && isLeftEmpty == true && isRightEmpty == true && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotLeftRightSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == true && isRightEmpty == false && isBottomEmpty == true)
        {
            addTile(TileType::GrassTopBotLeftSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == false && isRightEmpty == false && isBottomEmpty == true)
        {
            addTile(TileType::GrassTopBotSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == false && isRightEmpty == true && isBottomEmpty == true)
        {
            addTile(TileType::GrassTopBotRightSides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == true && isRightEmpty == true && isBottomEmpty == true)
        {
            addTile(TileType::Grass4Sides, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == true && isRightEmpty == false && isBottomEmpty == false && isBottomRightEmpty == true)
        {
            addTile(TileType::GrassTopLeftSidesCorner3, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == false && isRightEmpty == false && isBottomLeftEmpty == false &&
                 isBottomEmpty == false && isBottomRightEmpty == true)
        {
            addTile(TileType::GrassTopSideCorner3, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == false && isRightEmpty == false && isBottomLeftEmpty == true &&
                 isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassTopSideCorner4, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == true && isLeftEmpty == false && isRightEmpty == true && isBottomLeftEmpty == true && isBottomEmpty == false)
        {
            addTile(TileType::GrassTopRightSidesCorner4, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == true && isRightEmpty == false &&
                 isBottomEmpty == false && isBottomRightEmpty == true)
        {
            addTile(TileType::GrassLeftSideCorner3, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == false && isBottomEmpty == false && isBottomRightEmpty == true)
        {
            addTile(TileType::GrassNoSidesCorner3, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == true && isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassNoSidesCorner4, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isLeftEmpty == false && isRightEmpty == true &&
                 isBottomLeftEmpty == true && isBottomEmpty == false)
        {
            addTile(TileType::GrassRightSideCorner4, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false && isTopRightEmpty == true && isLeftEmpty == true && isRightEmpty == false && isBottomEmpty == false &&
                 isBottomRightEmpty == false)
        {
            addTile(TileType::GrassLeftSideCorner2, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isTopRightEmpty == true && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomRightEmpty == false && isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassNoSidesCorner2, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == true && isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == false && isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassNoSidesCorner1, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == true && isTopEmpty == false && isLeftEmpty == false && isRightEmpty == true &&
                 isBottomLeftEmpty == false && isBottomEmpty == false)
        {
            addTile(TileType::GrassRightSideCorner1, sf::Vector2u(x, y), layer);
        }
        else if (isTopEmpty == false && isTopRightEmpty == true && isLeftEmpty == true && isRightEmpty == false && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotLeftSidesCorner2, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isTopRightEmpty == true && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotSideCorner2, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == true && isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotSideCorner1, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == true && isTopEmpty == false && isLeftEmpty == false && isRightEmpty == true && isBottomEmpty == true)
        {
            addTile(TileType::GrassBotRightSidesCorner1, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == true && isTopEmpty == false && isTopRightEmpty == true && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == true && isBottomEmpty == false && isBottomRightEmpty == true)
        {
            addTile(TileType::GrassNoSides4Corners, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == true && isTopEmpty == false && isTopRightEmpty == true && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == false && isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassNoSidesCorners12, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == true && isBottomEmpty == false && isBottomRightEmpty == true)
        {
            addTile(TileType::GrassNoSidesCorners34, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == true && isTopEmpty == false && isTopRightEmpty == false && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == true && isBottomEmpty == false && isBottomRightEmpty == false)
        {
            addTile(TileType::GrassNoSidesCorners14, sf::Vector2u(x, y), layer);
        }
        else if (isTopLeftEmpty == false && isTopEmpty == false && isTopRightEmpty == true && isLeftEmpty == false &&
                 isRightEmpty == false && isBottomLeftEmpty == false && isBottomEmpty == false && isBottomRightEmpty == true)
        {
            addTile(TileType::GrassNoSidesCorners23, sf::Vector2u(x, y), layer);
        }
        else
        {
            addTile(TileType::GrassNoSides, sf::Vector2u(x, y), layer);
        }
    }
}

// Create a new Tile at the specified index
void Map::addTile(TileType tileType, const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures)
{
    if (layer == MapLayer::Count)
    {
        return;
    }

    unsigned int x = tileIndex.x;
    unsigned int y = tileIndex.y;
    unsigned int z = static_cast<unsigned int>(layer);

    // If index is outside of Map bounds
    if (x >= m_indexDimensions.x || y >= m_indexDimensions.y)
    {
        return;
    }

    // Delete existing Tile if found
    if (m_tiles[z][y][x] != nullptr)
    {
        delete m_tiles[z][y][x];
    }

    switch (tileType)
    {
    case TileType::GrassTopLeftSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopLeftSides"), tileType);
        break;
    case TileType::GrassTopSide:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopSide"), tileType);
        break;
    case TileType::GrassTopRightSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopRightSides"), tileType);
        break;
    case TileType::GrassLeftSide:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassLeftSide"), tileType);
        break;
    case TileType::GrassNoSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSides"), tileType);
        break;
    case TileType::GrassRightSide:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassRightSide"), tileType);
        break;
    case TileType::GrassBotLeftSide:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotLeftSides"), tileType);
        break;
    case TileType::GrassBotSide:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotSide"), tileType);
        break;
    case TileType::GrassBotRightSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotRightSides"), tileType);
        break;
    case TileType::GrassTopLeftRightSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopLeftRightSides"), tileType);
        break;
    case TileType::GrassLeftRightSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassLeftRightSides"), tileType);
        break;
    case TileType::GrassBotLeftRightSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotLeftRightSides"), tileType);
        break;
    case TileType::GrassTopBotLeftSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopBotLeftSides"), tileType);
        break;
    case TileType::GrassTopBotSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopBotSides"), tileType);
        break;
    case TileType::GrassTopBotRightSides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopBotRightSides"), tileType);
        break;
    case TileType::Grass4Sides:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grass4Sides"), tileType);
        break;
    case TileType::GrassTopLeftSidesCorner3:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopLeftSidesCorner3"), tileType);
        break;
    case TileType::GrassTopSideCorner3:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopSideCorner3"), tileType);
        break;
    case TileType::GrassTopSideCorner4:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopSideCorner4"), tileType);
        break;
    case TileType::GrassTopRightSidesCorner4:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassTopRightSidesCorner4"), tileType);
        break;
    case TileType::GrassLeftSideCorner3:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassLeftSideCorner3"), tileType);
        break;
    case TileType::GrassNoSidesCorner3:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorner3"), tileType);
        break;
    case TileType::GrassNoSidesCorner4:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorner4"), tileType);
        break;
    case TileType::GrassRightSideCorner4:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassRightSideCorner4"), tileType);
        break;
    case TileType::GrassLeftSideCorner2:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassLeftSideCorner2"), tileType);
        break;
    case TileType::GrassNoSidesCorner2:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorner2"), tileType);
        break;
    case TileType::GrassNoSidesCorner1:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorner1"), tileType);
        break;
    case TileType::GrassRightSideCorner1:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassRightSideCorner1"), tileType);
        break;
    case TileType::GrassBotLeftSidesCorner2:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotLeftSidesCorner2"), tileType);
        break;
    case TileType::GrassBotSideCorner2:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotSideCorner2"), tileType);
        break;
    case TileType::GrassBotSideCorner1:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotSideCorner1"), tileType);
        break;
    case TileType::GrassBotRightSidesCorner1:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassBotRightSidesCorner1"), tileType);
        break;
    case TileType::GrassNoSides4Corners:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSides4Corners"), tileType);
        break;
    case TileType::GrassNoSidesCorners12:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorners12"), tileType);
        break;
    case TileType::GrassNoSidesCorners34:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorners34"), tileType);
        break;
    case TileType::GrassNoSidesCorners14:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorners14"), tileType);
        break;
    case TileType::GrassNoSidesCorners23:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("grassNoSidesCorners23"), tileType);
        break;
    case TileType::Wood:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("wood"), tileType);
        break;
    case TileType::Ladder:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("ladder"), tileType);
        m_tiles[z][y][x]->setSolid(false);
        break;
    case TileType::LadderTop:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("ladder"), tileType);
        break;
    case TileType::Vine:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("vine"), tileType);
        m_tiles[z][y][x]->setSolid(false);
        break;
    case TileType::Post:
        m_tiles[z][y][x] = new Tile(m_resourceManager.getTexture("post"), tileType);
        m_tiles[z][y][x]->setSolid(false);
        break;
    default:
        m_tiles[z][y][x] = nullptr;
        break;
    }

    if (m_tiles[z][y][x] != nullptr)
    {
        m_tiles[z][y][x]->setPosition(sf::Vector2f(tileIndex.x * m_tileSize, tileIndex.y * m_tileSize));
        m_tiles[z][y][x]->setDimensions(sf::Vector2f(m_tileSize, m_tileSize));
        m_tiles[z][y][x]->setColor(m_layerColors[z]);
    }

    if (updateTextures == true)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                updateTileTexture(sf::Vector2u(x + i, y + j), layer);
            }
        }
    }
}

// Create a new range of Tiles at the specified index
void Map::addTileRange(TileType tileType, const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures)
{
    if (layer == MapLayer::Count)
    {
        return;
    }

    for (unsigned int y = 0; y < range.y; y++)
    {
        for (unsigned int x = 0; x < range.x; x++)
        {
            addTile(tileType, tileIndex + sf::Vector2u(x, y), layer);
        }
    }

    if (updateTextures == true)
    {
        for (int y = -1; y <= static_cast<int>(range.y); y++)
        {
            for (int x = -1; x <= static_cast<int>(range.x); x++)
            {
                updateTileTexture(sf::Vector2u(tileIndex.x + x, tileIndex.y + y), layer);
            }
        }
    }
}

// Delete the Tile at the specified index
void Map::removeTile(const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures)
{
    if (layer == MapLayer::Count)
    {
        return;
    }

    unsigned int x = tileIndex.x;
    unsigned int y = tileIndex.y;
    unsigned int z = static_cast<unsigned int>(layer);

    // If index is outside of Map bounds
    if (x >= m_indexDimensions.x || y >= m_indexDimensions.y)
    {
        return;
    }

    // Delete existing Tile if found
    if (m_tiles[z][y][x] != nullptr)
    {
        delete m_tiles[z][y][x];
        m_tiles[z][y][x] = nullptr;
    }

    if (updateTextures == true)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                updateTileTexture(sf::Vector2u(x + i, y + j), layer);
            }
        }
    }
}

// Delete the range of Tiles at the specified index
void Map::removeTileRange(const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures)
{
    if (layer == MapLayer::Count)
    {
        return;
    }

    for (unsigned int y = 0; y < range.y; y++)
    {
        for (unsigned int x = 0; x < range.x; x++)
        {
            removeTile(tileIndex + sf::Vector2u(x, y), layer);
        }
    }

    if (updateTextures == true)
    {
        for (int y = -1; y <= static_cast<int>(range.y); y++)
        {
            for (int x = -1; x <= static_cast<int>(range.x); x++)
            {
                updateTileTexture(sf::Vector2u(tileIndex.x + x, tileIndex.y + y), layer);
            }
        }
    }
}

// Resize the Map to the specified index dimensions
void Map::resize(const sf::Vector2u& indexDimensions)
{
    if (indexDimensions.x == 0 || indexDimensions.y == 0)
    {
        return;
    }

    // Max dimensions
    sf::Vector2u newIndexDimensions =
        sf::Vector2u(std::fmin(indexDimensions.x, maxDimensions.x), std::fmin(indexDimensions.y, maxDimensions.y));

    for (unsigned int z = 0; z < m_layerCount; z++)
    {
        // Delete excess Tiles on the Y axis
        for (unsigned int y = newIndexDimensions.y; y < m_indexDimensions.y; y++)
        {
            for (unsigned int x = 0; x < m_indexDimensions.x; x++)
            {
                delete m_tiles[z][y][x];
            }
        }
        m_tiles[z].resize(newIndexDimensions.y);

        // Delete excess Tiles on the X axis
        for (unsigned int y = 0; y < newIndexDimensions.y; y++)
        {
            for (unsigned int x = newIndexDimensions.x; x < m_indexDimensions.x; x++)
            {
                delete m_tiles[z][y][x];
            }
            m_tiles[z][y].resize(newIndexDimensions.x);
        }

        // Set new Tiles on the Y axis to nullptr
        for (unsigned int y = m_indexDimensions.y; y < newIndexDimensions.y; y++)
        {
            for (unsigned int x = 0; x < newIndexDimensions.x; x++)
            {
                m_tiles[z][y][x] = nullptr;
            }
        }
        // Set new Tiles on the X axis to nullptr
        for (unsigned int y = 0; y < newIndexDimensions.y; y++)
        {
            for (unsigned int x = m_indexDimensions.x; x < newIndexDimensions.x; x++)
            {
                m_tiles[z][y][x] = nullptr;
            }
        }
    }

    m_indexDimensions = newIndexDimensions;
}

// Remove all Tiles by deleting them and setting them to nullptr, but conserve the Map's index dimensions
void Map::clear()
{
    for (unsigned int z = 0; z < m_layerCount; z++)
    {
        for (unsigned int y = 0; y < m_indexDimensions.y; y++)
        {
            for (unsigned int x = 0; x < m_indexDimensions.x; x++)
            {
                delete m_tiles[z][y][x];
                m_tiles[z][y][x] = nullptr;
            }
        }
    }
}

// Remove all Tiles on a Layer by deleting them and setting them to nullptr
void Map::clearLayer(MapLayer layer)
{
    if (layer == MapLayer::Count)
    {
        return;
    }

    for (unsigned int y = 0; y < m_indexDimensions.y; y++)
    {
        for (unsigned int x = 0; x < m_indexDimensions.x; x++)
        {
            delete m_tiles[static_cast<unsigned int>(layer)][y][x];
            m_tiles[static_cast<unsigned int>(layer)][y][x] = nullptr;
        }
    }
}

// Set a layer's Tiles' color
void Map::setLayerColor(sf::Color color, MapLayer layer)
{
    if (layer == MapLayer::Count)
    {
        return;
    }

    unsigned int z = static_cast<unsigned int>(layer);

    m_layerColors[z] = color;

    for (unsigned int y = 0; y < m_indexDimensions.y; y++)
    {
        for (unsigned int x = 0; x < m_indexDimensions.x; x++)
        {
            if (m_tiles[z][y][x] != nullptr)
            {
                m_tiles[z][y][x]->setColor(m_layerColors[z]);
            }
        }
    }
}

// Return Map dimensions, in world coords
sf::Vector2u Map::getBounds() const
{
    return sf::Vector2u(m_indexDimensions.x * m_tileSize, m_indexDimensions.y * m_tileSize);
}

// Return true if the Map is 0x0
bool Map::isNull() const
{
    return m_indexDimensions == sf::Vector2u(0, 0);
}

// Return a pointer to a const Tile at given coords
const Tile* Map::getKTilePtr(const sf::Vector2u& index, MapLayer layer) const
{
    if (layer == MapLayer::Count || index.x >= m_indexDimensions.x || index.y >= m_indexDimensions.y)
    {
        return nullptr;
    }

    return m_tiles[static_cast<int>(layer)][index.y][index.x];
}

// Return a pointer to a Tile at given coords
Tile* Map::getTilePtr(const sf::Vector2u& index, MapLayer layer) const
{
    if (layer == MapLayer::Count || index.x >= m_indexDimensions.x || index.y >= m_indexDimensions.y)
    {
        return nullptr;
    }

    return m_tiles[static_cast<int>(layer)][index.y][index.x];
}
