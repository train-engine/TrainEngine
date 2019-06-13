#ifndef MAP_H
#define MAP_H

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Core/ResourceManager.h"
#include "Level/Tile.h"

enum class MapLayer
{
    Background,
    Solid,
    Overlay,
    Count
};

class Map final : public sf::Drawable
{
private:
    const ResourceManager& m_resourceManager;

    std::vector<std::vector<std::vector<Tile*>>> m_tiles;

    mutable sf::RectangleShape m_horizGridLine;
    mutable sf::RectangleShape m_vertGridLine;

    sf::Vector2u m_indexDimensions;
    const unsigned int m_layerCount; // Set to MapLayer::Count in constructor, to avoid repetitive casts
    unsigned int m_tileSize;

    std::array<sf::Color, static_cast<std::size_t>(MapLayer::Count)> m_layerColors;

    bool m_isGridVisible;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void drawGrid(sf::RenderTarget& rTarget, sf::RenderStates states) const;

public:
    // Constructor and destructor
    explicit Map(const ResourceManager& resourceManager);
    ~Map();

    // Functions
    void update();
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;

    sf::Vector2u coordsToTileIndex(const sf::Vector2f& position) const;
    sf::Vector2f tileIndexToCoords(const sf::Vector2u& position) const;

    // Tile functions
    void updateTileTexture(const sf::Vector2u& tileIndex, MapLayer layer);

    void addTile(TileType tileType, const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false);
    void addTileRange(TileType tileType, const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer,
                      bool updateTextures = false);
    void removeTile(const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false);
    void removeTileRange(const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures = false);

    void resize(const sf::Vector2u& indexDimensions);
    void clear();
    void clearLayer(MapLayer layer);

    // Setters
    void setLayerColor(sf::Color color, MapLayer layer);
    void setGridVisible(bool isGridVisible) { m_isGridVisible = isGridVisible; }

    // Getters
    const sf::Vector2u& getIndexDimensions() const { return m_indexDimensions; }
    unsigned int getLayerCount() const { return m_layerCount; }

    unsigned int getTileSize() const { return m_tileSize; }
    sf::Vector2u getBounds() const;
    bool isNull() const;

    const Tile* getKTilePtr(const sf::Vector2u& index, MapLayer layer) const;
    Tile* getTilePtr(const sf::Vector2u& index, MapLayer layer) const;
};

#endif // MAP_H
