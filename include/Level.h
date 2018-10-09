#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <vector>
#include "Camera.h"
#include "EntityTracker.h"
#include "InputManager.h"
#include "Map.h"
#include "ParallaxSprite.h"
#include "Player.h"
#include "ResourceManager.h"

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
    bool m_isCreatorMode;
    bool m_showDebugBoxes;

    // Functions
    bool LoadBackground(const std::string& filename);
    bool SaveBackground(const std::string& filename);
    bool LoadEntities(const std::string& filename);
    bool SaveEntities(const std::string& filename);
    bool LoadResources(const std::string& filename);
    bool SaveResources(const std::string& filename);

public:
    // Constructor and destructor
    Level(const ResourceManager& resourceManager, const InputManager& inputManager);
    ~Level();

    // Functions
    void HandleInput();
    void Update();
    void Draw(sf::RenderTarget& rTarget, sf::RenderStates states, float lag);

    bool Load(const std::string& levelDirectory);
    bool Save(const std::string& levelDirectory);

    void OnWindowResize();

    sf::Vector2u CoordsToTileIndex(const sf::Vector2f& position) const {return m_map.CoordsToTileIndex(position);}
    sf::Vector2f TileIndexToCoords(const sf::Vector2u& position) const {return m_map.TileIndexToCoords(position);}

    void AddTile(TileType tileType, const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false)
        {m_map.AddTile(tileType, tileIndex, layer, updateTextures);}
    void AddTileRange(TileType tileType, const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures = false)
        {m_map.AddTileRange(tileType, tileIndex, range, layer, updateTextures);}
    void RemoveTile(const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures = false)
        {m_map.RemoveTile(tileIndex, layer, updateTextures);}
    void RemoveTileRange(const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures = false)
        {m_map.RemoveTileRange(tileIndex, range, layer, updateTextures);}
    void Resize(const sf::Vector2u& indexDimensions) {m_map.Resize(indexDimensions);}
    void ClearLayer(MapLayer layer) {m_map.ClearLayer(layer);}

    // Setters
    void SetHasFocus(bool hasFocus) {m_hasFocus = hasFocus;}
    void SetCreatorMode(bool creatorMode);

    // Getters
    const sf::Vector2u& GetMapIndexDimensions() const {return m_map.GetIndexDimensions();}
    unsigned int GetTileSize() const {return m_map.GetTileSize();}
    sf::Vector2f GetLevelMousePosition() const {return m_inputManager.GetMousePosition(m_camera.GetView());}
};

#endif // LEVEL_H_INCLUDED
