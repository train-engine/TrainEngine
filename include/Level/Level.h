#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Core/InputManager.h"
#include "Core/ResourceManager.h"
#include "Level/Camera.h"
#include "Level/EntityTracker.h"
#include "Level/Map.h"
#include "Level/ParallaxSprite.h"

class Level final
{
private:
    const ResourceManager& m_resourceManager;
    const InputManager& m_inputManager;

    Map m_map;
    std::vector<ParallaxSprite> m_parallaxSprites;
    std::vector<Entity*> m_entities;

    Camera m_camera;

    bool m_hasFocus;
    bool m_isCreatorModeEnabled;
    bool m_isEntityDebugBoxVisible;

    // Functions
    bool loadBackground(const std::string& filename);
    bool saveBackground(const std::string& filename) const;
    bool loadEntities(const std::string& filename);
    bool saveEntities(const std::string& filename) const;
    bool loadResources(const std::string& filename);
    bool saveResources(const std::string& filename) const;

public:
    // Constructor and destructor
    Level(const ResourceManager& resourceManager, const InputManager& inputManager);
    ~Level();

    // Functions
    void handleInput();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states, float lag);

    bool load(const std::string& levelDirectory);
    bool save(const std::string& levelDirectory) const;

    void onWindowResize();

    sf::Vector2u coordsToTileIndex(const sf::Vector2f& position) const { return m_map.coordsToTileIndex(position); }
    sf::Vector2f tileIndexToCoords(const sf::Vector2u& position) const { return m_map.tileIndexToCoords(position); }

    void addTile(TileType tileType, const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false);
    void addTileRange(TileType tileType, const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer,
                      bool updateTextures = false);
    void removeTile(const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false);
    void removeTileRange(const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures = false);

    void resize(const sf::Vector2u& indexDimensions) { m_map.resize(indexDimensions); }
    void clearLayer(MapLayer layer) { m_map.clearLayer(layer); }

    // Setters
    void setFocus(bool hasFocus) { m_hasFocus = hasFocus; }
    void setCreatorModeEnabled(bool isCreatorModeEnabled);

    // Getters
    const sf::Vector2u& getMapIndexDimensions() const { return m_map.getIndexDimensions(); }
    unsigned int getTileSize() const { return m_map.getTileSize(); }
    sf::Vector2f getLevelMousePosition() const { return m_inputManager.getMousePosition(m_camera.getView()); }
};

#endif // LEVEL_H
