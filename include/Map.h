#ifndef MAP_H
#define MAP_H

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Tile.h"

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

    std::array<sf::Color, static_cast<size_t>(MapLayer::Count)> m_layerColors;

    bool m_isGridVisible;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void DrawGrid(sf::RenderTarget& rTarget, sf::RenderStates states) const;

public:
    // Constructor and destructor
    explicit Map(const ResourceManager& resourceManager);
    ~Map();

    // Functions
    void Update();
    bool Load(const std::string& filename);
    bool Save(const std::string& filename) const;

    sf::Vector2u CoordsToTileIndex(const sf::Vector2f& position) const;
    sf::Vector2f TileIndexToCoords(const sf::Vector2u& position) const;

    // Tile functions
    void UpdateTileTexture(const sf::Vector2u& tileIndex, MapLayer layer);

    void AddTile(TileType tileType, const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false);
    void AddTileRange(TileType tileType, const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer,
                      bool updateTextures = false);
    void RemoveTile(const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false);
    void RemoveTileRange(const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures = false);

    void Resize(const sf::Vector2u& indexDimensions);
    void Clear();
    void ClearLayer(MapLayer layer);

    // Setters
    void SetLayerColor(sf::Color color, MapLayer layer);
    void SetGridVisible(bool isGridVisible) { m_isGridVisible = isGridVisible; }

    // Getters
    const sf::Vector2u& GetIndexDimensions() const { return m_indexDimensions; }
    unsigned int GetLayerCount() const { return m_layerCount; }

    unsigned int GetTileSize() const { return m_tileSize; }
    sf::Vector2u GetBounds() const;
    bool IsNull() const;

    const Tile* GetKTilePtr(const sf::Vector2u& index, MapLayer layer) const;
    Tile* GetTilePtr(const sf::Vector2u& index, MapLayer layer) const;
};

#endif // MAP_H
