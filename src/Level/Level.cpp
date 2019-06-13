#include "Level/Level.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include "Core/FileManager.h"
#include "Level/Player.h"
#include "Misc/Utility.h"

Level::Level(const ResourceManager& resourceManager, const InputManager& inputManager)
    : m_resourceManager(resourceManager)
    , m_inputManager(inputManager)
    , m_map(m_resourceManager)
    , m_hasFocus(true)
    , m_isCreatorModeEnabled(false)
    , m_isEntityDebugBoxVisible(false)
{
    m_map.setLayerColor(sf::Color(112, 112, 112, 255), MapLayer::Background);
    m_map.setLayerColor(sf::Color(255, 255, 255, 192), MapLayer::Overlay);

    m_camera.setFollowLerp(0.2);
    m_camera.setBoundless(false);
}

Level::~Level()
{
    for (const auto& entity : m_entities)
    {
        delete entity;
    }
}

// Load the background from a save file
bool Level::loadBackground(const std::string& filename)
{
    // Remove all ParallaxSprites (necessary when changing level)
    m_parallaxSprites.clear();

    std::ifstream inputFile(FileManager::resourcePath() + filename);
    if (inputFile)
    {
        std::cout << "Loading background...\n";

        std::string line;
        while (std::getline(inputFile, line))
        {
            // Ignore empty lines or those starting with '#'
            if (line.empty() || line.front() == '#')
            {
                continue;
            }

            std::istringstream lineStream(line);
            std::string resourceName;
            float parallaxValue;
            if (!(lineStream >> resourceName >> parallaxValue))
            {
                std::cerr << "\nLevel error: Parsing resource name or parallax value failed in file: \"" << filename << "\".\n"
                          << "Background loading failed.\n\n";
                return false;
            }

            m_parallaxSprites.emplace_back(m_resourceManager.getTexture(resourceName), parallaxValue);
            m_parallaxSprites.back().setResourceName(resourceName);

            std::string pair;
            while (lineStream >> pair)
            {
                std::size_t delimPos = pair.find(':');
                if (delimPos == std::string::npos)
                {
                    std::cerr << "\nLevel error: Parsing key-value pair failed in file: \"" << filename << "\".\n"
                              << "Background loading failed.\n\n";
                    return false;
                }

                std::string key = pair.substr(0, delimPos);
                std::string value = pair.substr(delimPos + 1);

                if (key == "positionMode")
                {
                    m_parallaxSprites.back().setPositionModeString(value);
                }
                else if (key == "repeatTexture")
                {
                    m_parallaxSprites.back().setRepeatTextureString(value);
                }
                else if (key == "scale")
                {
                    m_parallaxSprites.back().setScaleString(value);
                }
                else if (key == "offset")
                {
                    m_parallaxSprites.back().setOffsetString(value);
                }
            }

            // Position mode
            std::string positionModeString = m_parallaxSprites.back().getPositionModeString();
            if (!positionModeString.empty())
            {
                if (positionModeString == "TL")
                {
                    m_parallaxSprites.back().setRelativeOrigin({0, 0});
                    m_parallaxSprites.back().setPosition({0, 0});
                }
                else if (positionModeString == "TM")
                {
                    m_parallaxSprites.back().setRelativeOrigin({0.5, 0});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(m_map.getBounds().x / 2.0f, 0));
                }
                else if (positionModeString == "TR")
                {
                    m_parallaxSprites.back().setRelativeOrigin({1, 0});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(m_map.getBounds().x, 0));
                }
                else if (positionModeString == "ML")
                {
                    m_parallaxSprites.back().setRelativeOrigin({0, 0.5});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(0, m_map.getBounds().y / 2.0f));
                }
                else if (positionModeString == "MM")
                {
                    m_parallaxSprites.back().setRelativeOrigin({0.5, 0.5});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(m_map.getBounds().x / 2.0f, m_map.getBounds().y / 2.0f));
                }
                else if (positionModeString == "MR")
                {
                    m_parallaxSprites.back().setRelativeOrigin({1, 0.5});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(m_map.getBounds().x, m_map.getBounds().y / 2.0f));
                }
                else if (positionModeString == "BL")
                {
                    m_parallaxSprites.back().setRelativeOrigin({1, 1});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(0, m_map.getBounds().y));
                }
                else if (positionModeString == "BM")
                {
                    m_parallaxSprites.back().setRelativeOrigin({0.5, 1});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(m_map.getBounds().x / 2.0f, m_map.getBounds().y));
                }
                else if (positionModeString == "BR")
                {
                    m_parallaxSprites.back().setRelativeOrigin({1, 1});
                    m_parallaxSprites.back().setPosition(sf::Vector2f(m_map.getBounds().x, m_map.getBounds().y));
                }
            }

            // Repeat texture
            std::string repeatTextureString = m_parallaxSprites.back().getRepeatTextureString();
            if (!repeatTextureString.empty())
            {
                if (repeatTextureString == "XY")
                {
                    m_parallaxSprites.back().setTextureRect(static_cast<sf::Vector2f>(m_map.getBounds()));
                }
                else if (repeatTextureString == "X")
                {
                    m_parallaxSprites.back().setTextureRect(
                        sf::Vector2f(m_map.getBounds().x, m_parallaxSprites.back().getTexture()->getSize().y));
                }
                else if (repeatTextureString == "Y")
                {
                    m_parallaxSprites.back().setTextureRect(
                        sf::Vector2f(m_parallaxSprites.back().getTexture()->getSize().x, m_map.getBounds().y));
                }
            }

            // Scale
            std::string scaleString = m_parallaxSprites.back().getScaleString();
            if (!scaleString.empty())
            {
                if (scaleString == "map")
                {
                    float scale = Utility::getScaleToFill(static_cast<sf::Vector2f>(m_parallaxSprites.back().getTexture()->getSize()),
                                                          static_cast<sf::Vector2f>(m_map.getBounds()));
                    m_parallaxSprites.back().setScale({scale, scale});
                }
                else
                {
                    std::size_t commaPos = scaleString.find(',');
                    float x = std::stof(scaleString.substr(0, commaPos));
                    float y = std::stof(scaleString.substr(commaPos + 1));
                    m_parallaxSprites.back().setScale({x, y});
                }
            }

            // Offset
            std::string offsetString = m_parallaxSprites.back().getOffsetString();
            if (!offsetString.empty())
            {
                std::size_t commaPos = offsetString.find(',');
                float x = std::stof(offsetString.substr(0, commaPos));
                float y = std::stof(offsetString.substr(commaPos + 1));
                m_parallaxSprites.back().setPosition(m_parallaxSprites.back().getPosition() + sf::Vector2f(x, y));
            }
        }

        std::sort(m_parallaxSprites.begin(), m_parallaxSprites.end(), [](const ParallaxSprite& a, const ParallaxSprite& b) {
            return a.getParallax() > b.getParallax();
        });

        std::cout << "Background successfully loaded.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to open \"" << filename << "\".\n"
              << "Background loading failed.\n\n";
    return false;
}

// Save the background to a save file
bool Level::saveBackground(const std::string& filename) const
{
    std::ofstream outputFile(FileManager::resourcePath() + filename);
    if (outputFile)
    {
        std::cout << "Saving background...\n";
        outputFile << "# Syntax:\n"
                      "# resourceName parallaxValue pos:[TL|TM|TR|ML|MM|MR|BL|BM|BR] repeatTexture[-o]:[XY|X|Y] "
                      "scale[-o]:[x,y|map] offset[-o]:x,y\n\n";

        std::vector<ParallaxSprite> sortedParallaxSprites(m_parallaxSprites);
        std::sort(sortedParallaxSprites.begin(), sortedParallaxSprites.end(), [](const ParallaxSprite& a, const ParallaxSprite& b) {
            return a.getParallax() > b.getParallax();
        });

        std::cout << "Number of parallax sprites: " << m_parallaxSprites.size() << '\n';

        std::cout << "Parallax sprites:\n";
        for (const auto& parallaxSprite : m_parallaxSprites)
        {
            outputFile << parallaxSprite.getResourceName() << ' ' << parallaxSprite.getParallax()
                       << " positionMode:" << parallaxSprite.getPositionModeString();
            if (!parallaxSprite.getRepeatTextureString().empty())
            {
                outputFile << " repeatTexture:" << parallaxSprite.getRepeatTextureString();
            }
            if (!parallaxSprite.getScaleString().empty())
            {
                outputFile << " scale:" << parallaxSprite.getScaleString();
            }
            if (!parallaxSprite.getOffsetString().empty())
            {
                outputFile << " offset:" << parallaxSprite.getOffsetString();
            }
            outputFile << '\n';

            std::cout << parallaxSprite.getResourceName() << ' ' << parallaxSprite.getParallax()
                      << " positionMode:" << parallaxSprite.getPositionModeString() << '\n';
        }

        std::cout << "Background successfully saved.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to save \"" << filename << "\".\n"
              << "Background saving failed.\n\n";
    return false;
}

// Load the Entities from a save file
bool Level::loadEntities(const std::string& filename)
{
    // Remove all Entities (necessary when changing level)
    for (const auto& entity : m_entities)
    {
        delete entity;
    }

    std::ifstream inputFile(FileManager::resourcePath() + filename);
    if (inputFile)
    {
        std::cout << "Loading Entities...\n";
        unsigned int entityCount = 0;
        inputFile >> entityCount;
        std::cout << "Number of Entities: " << entityCount << '\n';

        m_entities.resize(entityCount);
        std::cout << "Entities:\n";
        for (auto& entity : m_entities)
        {
            unsigned int type = 0;
            float xPosition = 0;
            float yPosition = 0;
            inputFile >> type >> xPosition >> yPosition;
            switch (static_cast<EntityType>(type))
            {
            case EntityType::Player:
                entity = new Player(m_map, m_entities, m_inputManager, sf::Vector2f(xPosition, yPosition));
                entity->setStateAnimation(EntityState::Still,
                                            AnimatedSprite(m_resourceManager.getTexture("characterStill"), sf::Vector2u(54, 82), 22), 3);
                entity->setStateAnimation(EntityState::Running,
                                            AnimatedSprite(m_resourceManager.getTexture("characterRunning"), sf::Vector2u(82, 82), 27), 1);
                entity->setStateAnimation(EntityState::Climbing,
                                            AnimatedSprite(m_resourceManager.getTexture("characterClimbing"), sf::Vector2u(70, 82), 8), 2);
                entity->setStateAnimation(EntityState::Jumping,
                                            AnimatedSprite(m_resourceManager.getTexture("characterJumping"), sf::Vector2u(66, 82), 3), 2);
                entity->setStateAnimation(EntityState::Falling,
                                            AnimatedSprite(m_resourceManager.getTexture("characterFalling"), sf::Vector2u(72, 82), 3), 2);
                entity->setPosition({xPosition, yPosition});
                break;
            default:
                break;
            }
            std::cout << Entity::getEntityTypeString(entity->getEntityType()) << " at (" << entity->getPosition().x << ", "
                      << entity->getPosition().y << ")\n";
        }

        std::cout << "Entities successfully loaded.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to open \"" << filename << "\".\n"
              << "Entities loading failed.\n\n";
    return false;
}

// Save the Entities to a save file
bool Level::saveEntities(const std::string& filename) const
{
    std::ofstream outputFile(FileManager::resourcePath() + filename);
    if (outputFile)
    {
        std::cout << "Saving Entities...\n";
        outputFile << m_entities.size() << '\n';
        std::cout << "Number of Entities: " << m_entities.size() << '\n';

        std::cout << "Entities:\n";
        for (const auto& entity : m_entities)
        {
            outputFile << static_cast<int>(entity->getEntityType()) << ' ' << entity->getPosition().x << ' ' << entity->getPosition().y
                       << "\n";
            std::cout << Entity::getEntityTypeString(entity->getEntityType()) << " at (" << entity->getPosition().x << ", "
                      << entity->getPosition().y << ")\n";
        }

        std::cout << "Entities successfully saved.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to save \"" << filename << "\".\n"
              << "Entities saving failed.\n\n";
    return false;
}

// Load the list of necessary resources for the Level from a save file
bool Level::loadResources(const std::string& filename)
{
    // TODO
    return true;
}

// Save the list of necessary resources for the Level to a save file
bool Level::saveResources(const std::string& filename) const
{
    std::ofstream outputFile(FileManager::resourcePath() + filename);
    if (outputFile)
    {
        std::cout << "Saving resources...\n";

        std::set<std::string> resources;

        // Tiles
        for (unsigned int z = 0; z < m_map.getLayerCount(); z++)
        {
            for (unsigned int y = 0; y < m_map.getIndexDimensions().y; y++)
            {
                for (unsigned int x = 0; x < m_map.getIndexDimensions().x; x++)
                {
                    const Tile* tile = m_map.getKTilePtr(sf::Vector2u(x, y), static_cast<MapLayer>(z));
                    if (tile != nullptr)
                    {
                        resources.insert(Tile::getTextureName(tile->getTileType()));
                    }
                }
            }
        }

        // Background
        for (const auto& parallaxSprite : m_parallaxSprites)
        {
            resources.insert(parallaxSprite.getResourceName());
        }

        // Entities
        for (const auto& entity : m_entities)
        {
            if (entity != nullptr)
            {
                std::vector<std::string> entityTextureNames = Entity::getTextureNames(entity->getEntityType());
                for (const auto& textureName : entityTextureNames)
                {
                    resources.insert(textureName);
                }
            }
        }

        std::cout << "Number of resources: " << resources.size() << '\n';

        for (const auto& resource : resources)
        {
            outputFile << resource << '\n';
        }

        std::cout << "Resources successfully saved.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to save \"" << filename << "\".\n"
              << "Resources saving failed.\n\n";
    return false;
}

void Level::handleInput()
{
    // Do not handle input if the Level does not have the GUI focus
    if (m_hasFocus == false)
    {
        return;
    }

    // Show debug boxes
    if (m_inputManager.isKeyDescending(sf::Keyboard::BackSlash))
    {
        m_isEntityDebugBoxVisible = !m_isEntityDebugBoxVisible;
        for (const auto& entity : m_entities)
        {
            entity->setDebugBoxVisible(m_isEntityDebugBoxVisible);
        }
    }

    // Entities
    if (m_isCreatorModeEnabled == false)
    {
        for (const auto& entity : m_entities)
        {
            entity->handleInput();
        }
    }

    // Camera manual control
    float cameraSpeed = 5 * m_camera.getZoom();
    if (m_inputManager.isKeyHeld(sf::Keyboard::I))
    {
        m_camera.move({0, -cameraSpeed});
    }
    if (m_inputManager.isKeyHeld(sf::Keyboard::J))
    {
        m_camera.move({-cameraSpeed, 0});
    }
    if (m_inputManager.isKeyHeld(sf::Keyboard::K))
    {
        m_camera.move({0, cameraSpeed});
    }
    if (m_inputManager.isKeyHeld(sf::Keyboard::L))
    {
        m_camera.move({cameraSpeed, 0});
    }

    if (m_inputManager.isMouseButtonDescending(sf::Mouse::Middle))
    {
        m_camera.setTranslate(m_camera.getPosition(), getLevelMousePosition(), 60, true);
    }
    if (m_camera.getMode() == CameraMode::Static && m_inputManager.isKeyDescending(sf::Keyboard::Q))
    {
        if (!m_entities.empty() && m_entities.front() != nullptr)
        {
            m_camera.setFollow(*m_entities.front(), true);
        }
    }

    // Camera zoom
    if (m_inputManager.detectedMouseWheelScrolledEvent())
    {
        m_camera.zoom(1 - m_inputManager.getMouseWheelDelta().y * 0.05);
    }
    else if (m_inputManager.getJoystickAxisPosition(0, sf::Joystick::V) != 0.0)
    {
        m_camera.zoom(1 - m_inputManager.getJoystickAxisPosition(0, sf::Joystick::V) / -100 / 20);
    }

    // Print the mouse cursor's position when space is held (for testing)
    if (m_inputManager.isKeyHeld(sf::Keyboard::Space))
    {
        std::cout << "Mouse position: " << getLevelMousePosition().x << ", " << getLevelMousePosition().y << '\n';
    }
}

void Level::update()
{
    m_map.update();

    if (m_isCreatorModeEnabled == false)
    {
        for (const auto& entity : m_entities)
        {
            entity->update();
        }
    }

    m_camera.update();
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states, float lag)
{
    // Camera
    m_camera.interpolate(lag);

    // Change view to Camera view
    sf::View oldView = target.getView();
    target.setView(m_camera.getView());

    // Parallax background
    for (auto& parallaxSprite : m_parallaxSprites)
    {
        parallaxSprite.update(m_camera);
        target.draw(parallaxSprite, states);
    }

    // Map and Entities
    target.draw(m_map, states);
    for (const auto& entity : m_entities)
    {
        entity->interpolate(lag);
        target.draw(*entity, states);
    }

    // Reset the target's view back to its initial view
    target.setView(oldView);
}

// Load all Level components, such as the Map, the Entities and the Parallax background
bool Level::load(const std::string& levelDirectory)
{
    std::cout << "\nLoading Level: " << levelDirectory << "\n\n";
    if (m_map.load(levelDirectory + "/tiles.txt") && loadBackground(levelDirectory + "/background.txt") &&
        loadEntities(levelDirectory + "/entities.txt") && loadResources(levelDirectory + "/resources.txt"))
    {
        m_camera.setBounds(static_cast<sf::Vector2f>(m_map.getBounds()));

        if (m_isCreatorModeEnabled == false)
        {
            if (!m_entities.empty() && m_entities.front() != nullptr)
            {
                m_camera.setFollow(*m_entities.front(), true);
            }
        }

        std::cout << "Level successfully loaded.\n\n";
        return true;
    }

    std::cout << "Failed to load Level.\n\n";
    return false;
}

bool Level::save(const std::string& levelDirectory) const
{
    std::cout << "\nSaving Level: " << levelDirectory << "\n\n";
    // TODO
    // check if dir exists
    // if not, create it
    // make temp copy and replace if everything works

    if (m_map.save(levelDirectory + "/tiles.txt") && saveBackground(levelDirectory + "/background.txt") &&
        saveEntities(levelDirectory + "/entities.txt") && saveResources(levelDirectory + "/resources.txt"))
    {
        std::cout << "Level successfully saved.\n\n";
        return true;
    }

    std::cout << "Failed to save Level.\n\n";
    return false;
}

void Level::onWindowResize()
{
    // Resize Camera to window dimensions
    float oldZoom = m_camera.getZoom();
    m_camera.setDimensions(static_cast<sf::Vector2f>(m_inputManager.getWindowDimensions()));
    m_camera.setZoom(oldZoom);
}

void Level::addTile(TileType tileType, const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures)
{
    m_map.addTile(tileType, tileIndex, layer, updateTextures);
}

void Level::addTileRange(TileType tileType, const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures)
{
    m_map.addTileRange(tileType, tileIndex, range, layer, updateTextures);
}

void Level::removeTile(const sf::Vector2u& tileIndex, MapLayer layer, bool updateTextures)
{
    m_map.removeTile(tileIndex, layer, updateTextures);
}

void Level::removeTileRange(const sf::Vector2u& tileIndex, const sf::Vector2u& range, MapLayer layer, bool updateTextures)
{
    m_map.removeTileRange(tileIndex, range, layer, updateTextures);
}

void Level::setCreatorModeEnabled(bool isCreatorModeEnabled)
{
    m_isCreatorModeEnabled = isCreatorModeEnabled;
    m_map.setGridVisible(m_isCreatorModeEnabled);
    m_camera.setBoundless(m_isCreatorModeEnabled);
    if (m_isCreatorModeEnabled == true)
    {
        m_camera.setMaxDimensions({7680, 4320});
    }
    else
    {
        m_camera.setMaxDimensions({2560, 1440});
    }
}
