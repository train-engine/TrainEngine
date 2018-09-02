#include "Level.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <unordered_map>
#include "FileManager.h"
#include "Utility.h"

Level::Level(const ResourceManager& resourceManager, const InputManager& inputManager)
    : m_resourceManager(resourceManager),
      m_inputManager(inputManager),
      m_map(m_resourceManager),
      m_camera(m_map, static_cast<sf::Vector2f>(m_inputManager.GetWindowDimensions())),
      m_hasFocus(true),
      m_creatorMode(false),
      m_displayCollisionBoxes(false)
{
    m_map.SetLayerColor(sf::Color(255, 255, 255, 192), MapLayer::Overlay);
    m_camera.SetFollowLerp(0.2);
}

Level::~Level()
{
    for (const auto& pEntity : m_entities)
    {
        delete pEntity;
    }
}

// Loads the background from a save file
bool Level::LoadBackground(const std::string& filename)
{
    std::ifstream inf;
    inf.open(FileManager::ResourcePath() + filename);
    if (inf.is_open())
    {
        std::cout << "Loading background...\n";

        std::string line;
        while (std::getline(inf, line))
        {
            // Ignore empty lines or those starting with '#'
            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            std::istringstream lineStream(line);
            std::string resourceName;
            float parallaxValue;
            if (!(lineStream >> resourceName >> parallaxValue))
            {
                std::cout << "\nLevel error: Parsing resource name or parallax value failed in file: \"" << filename << "\".\n"
                             "Background loading failed.\n\n";
                return false;
            }

            m_parallaxSprites.emplace_back(ParallaxSprite(m_resourceManager.GetTexture(resourceName), parallaxValue));
            m_parallaxSprites.back().SetResourceName(resourceName);

            std::string pair;
            while (lineStream >> pair)
            {
                size_t delimPos = pair.find(':');
                if (delimPos == std::string::npos)
                {
                    std::cout << "\nLevel error: Parsing key-value pair failed in file: \"" << filename << "\".\n"
                                 "Background loading failed.\n\n";
                    return false;
                }

                std::string key = pair.substr(0, delimPos);
                std::string value = pair.substr(delimPos + 1);

                if (key == "positionMode")
                {
                    m_parallaxSprites.back().SetPositionModeString(value);
                }
                else if (key == "repeatTexture")
                {
                    m_parallaxSprites.back().SetRepeatTextureString(value);
                }
                else if (key == "scale")
                {
                    m_parallaxSprites.back().SetScaleString(value);
                }
                else if (key == "offset")
                {
                    m_parallaxSprites.back().SetOffsetString(value);
                }
            }

            // Position mode
            std::string positionModeString = m_parallaxSprites.back().GetPositionModeString();
            if (positionModeString != "")
            {
                if (positionModeString == "TL")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0, 0});
                    m_parallaxSprites.back().SetPosition({0, 0});
                }
                else if (positionModeString == "TM")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0.5, 0});
                    m_parallaxSprites.back().SetPosition({static_cast<float>(m_map.GetBounds().x / 2), 0});
                }
                else if (positionModeString == "TR")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 0});
                    m_parallaxSprites.back().SetPosition({static_cast<float>(m_map.GetBounds().x), 0});
                }
                else if (positionModeString == "ML")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0, 0.5});
                    m_parallaxSprites.back().SetPosition({0, static_cast<float>(m_map.GetBounds().y) / 2});
                }
                else if (positionModeString == "MM")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0.5, 0.5});
                    m_parallaxSprites.back().SetPosition({static_cast<float>(m_map.GetBounds().x) / 2, static_cast<float>(m_map.GetBounds().y / 2)});
                }
                else if (positionModeString == "MR")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 0.5});
                    m_parallaxSprites.back().SetPosition({static_cast<float>(m_map.GetBounds().x), static_cast<float>(m_map.GetBounds().y) / 2});
                }
                else if (positionModeString == "BL")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 1});
                    m_parallaxSprites.back().SetPosition({0, static_cast<float>(m_map.GetBounds().y)});
                }
                else if (positionModeString == "BM")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0.5, 1});
                    m_parallaxSprites.back().SetPosition({static_cast<float>(m_map.GetBounds().x) / 2, static_cast<float>(m_map.GetBounds().y)});
                }
                else if (positionModeString == "BR")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 1});
                    m_parallaxSprites.back().SetPosition({static_cast<float>(m_map.GetBounds().x), static_cast<float>(m_map.GetBounds().y)});
                }
            }

            // Repeat texture
            std::string repeatTextureString = m_parallaxSprites.back().GetRepeatTextureString();
            if (repeatTextureString != "")
            {
                if (repeatTextureString == "XY")
                {
                    m_parallaxSprites.back().SetTextureRect(static_cast<sf::Vector2f>(m_map.GetBounds()));
                }
                else if (repeatTextureString == "X")
                {
                    m_parallaxSprites.back().SetTextureRect({static_cast<float>(m_map.GetBounds().x),
                        static_cast<float>(m_parallaxSprites.back().GetTexture()->getSize().y)});
                }
                else if (repeatTextureString == "Y")
                {
                    m_parallaxSprites.back().SetTextureRect({static_cast<float>(m_parallaxSprites.back().GetTexture()->getSize().x),
                        static_cast<float>(m_map.GetBounds().y)});
                }
            }

            // Scale
            std::string scaleString = m_parallaxSprites.back().GetScaleString();
            if (scaleString != "")
            {
                if (scaleString == "map")
                {
                    float scale = Utility::GetScaleToFill(static_cast<sf::Vector2f>(m_parallaxSprites.back().GetTexture()->getSize()),
                                                          static_cast<sf::Vector2f>(m_map.GetBounds()));
                    m_parallaxSprites.back().SetScale({scale, scale});
                }
                else
                {
                    size_t commaPos = scaleString.find(',');
                    float x = std::stof(scaleString.substr(0, commaPos));
                    float y = std::stof(scaleString.substr(commaPos + 1));
                    m_parallaxSprites.back().SetScale({x, y});
                }
            }

            // Offset
            std::string offsetString = m_parallaxSprites.back().GetOffsetString();
            if (offsetString != "")
            {
                size_t commaPos = offsetString.find(',');
                float x = std::stof(offsetString.substr(0, commaPos));
                float y = std::stof(offsetString.substr(commaPos + 1));
                m_parallaxSprites.back().SetPosition(m_parallaxSprites.back().GetPosition() + sf::Vector2f(x, y));
            }
        }

        std::sort(m_parallaxSprites.begin(), m_parallaxSprites.end(), [](const ParallaxSprite& a, const ParallaxSprite& b) {return a.GetParallax() > b.GetParallax();});

        std::cout << "Background successfully loaded.\n\n";
        return true;
    }
    else
    {
        std::cout << "Level error: Unable to open \"" << filename << "\".\n"
                     "Background loading failed.\n\n";
        return false;
    }
}

// Saves the background to a save file
bool Level::SaveBackground(const std::string& filename)
{
    std::ofstream outf;
    outf.open(FileManager::ResourcePath() + filename);
    if (outf.is_open())
    {
        std::cout << "Saving background...\n";
        outf << "# Syntax:\n"
        "# resourceName parallaxValue pos:[TL|TM|TR|ML|MM|MR|BL|BM|BR] repeatTexture[-o]:[XY|X|Y] scale[-o]:[x,y|map] offset[-o]:x,y\n\n";
        std::sort(m_parallaxSprites.begin(), m_parallaxSprites.end(), [](const ParallaxSprite& a, const ParallaxSprite& b) {return a.GetParallax() > b.GetParallax();});
        std::cout << "Number of parallax sprites: " << m_parallaxSprites.size() << '\n';

        std::cout << "Parallax sprites:\n";
        for (const auto& parallaxSprite : m_parallaxSprites)
        {
            outf << parallaxSprite.GetResourceName() << ' ' << parallaxSprite.GetParallax() << " positionMode:" << parallaxSprite.GetPositionModeString();
            if (parallaxSprite.GetRepeatTextureString() != "")
            {
                outf << " repeatTexture:" << parallaxSprite.GetRepeatTextureString();
            }
            if (parallaxSprite.GetScaleString() != "")
            {
                outf << " scale:" << parallaxSprite.GetScaleString();
            }
            if (parallaxSprite.GetOffsetString() != "")
            {
                outf << " offset:" << parallaxSprite.GetOffsetString();
            }
            outf << '\n';

            std::cout << parallaxSprite.GetResourceName() << ' ' << parallaxSprite.GetParallax() << " positionMode:" << parallaxSprite.GetPositionModeString();
        }

        outf.close();
        std::cout << "Background successfully saved.\n\n";
        return true;
    }
    else
    {
        std::cout << "Level error: Unable to save \"" << filename << "\".\n"
        "Background saving failed.\n\n";
        return false;
    }
}

// Loads the Entities from a save file
bool Level::LoadEntities(const std::string& filename)
{
    // Delete all Entities (necessary when changing level)
    for (const auto& pEntity : m_entities)
    {
        delete pEntity;
    }

    std::ifstream inf;
    inf.open(FileManager::ResourcePath() + filename);
    if (inf.is_open())
    {
        std::cout << "Loading Entities...\n";
        unsigned int entityCount = 0;
        inf >> entityCount;
        std::cout << "Number of Entities: " << entityCount << '\n';

        m_entities.resize(entityCount);
        std::cout << "Entities:\n";
        for (auto& rpEntity : m_entities)
        {
            unsigned int type = 0;
            float xPosition = 0;
            float yPosition = 0;
            inf >> type >> xPosition >> yPosition;
            switch (static_cast<EntityType>(type))
            {
                case EntityType::Player:
                    rpEntity = new Player(m_map, m_entities, m_inputManager, sf::Vector2f (xPosition, yPosition));
                    rpEntity->AddAnimation(EntityState::Still, AnimatedSprite(m_resourceManager.GetTexture("characterStill"), sf::Vector2u(54, 82), 22, 3));
                    rpEntity->AddAnimation(EntityState::Running, AnimatedSprite(m_resourceManager.GetTexture("characterRunning"), sf::Vector2u(82, 82), 27, 1));
                    rpEntity->AddAnimation(EntityState::Climbing, AnimatedSprite(m_resourceManager.GetTexture("characterClimbing"), sf::Vector2u(70, 82), 8, 3, true, false));
                    rpEntity->AddAnimation(EntityState::Jumping, AnimatedSprite(m_resourceManager.GetTexture("characterJumping"), sf::Vector2u(66, 82), 3, 2));
                    rpEntity->AddAnimation(EntityState::Falling, AnimatedSprite(m_resourceManager.GetTexture("characterFalling"), sf::Vector2u(72, 82), 3, 2));
                    rpEntity->SetPosition(sf::Vector2f(xPosition, yPosition));
                    break;
                default:
                    break;
            }
            std::cout << Entity::GetEntityTypeString(rpEntity->GetEntityType()) << " at (" << rpEntity->GetPosition().x << ", " << rpEntity->GetPosition().y << ")\n";
        }
        
        inf.close();
        std::cout << "Entities successfully loaded.\n\n";
        return true;
    }
    else
    {
        std::cout << "Level error: Unable to open \"" << filename << "\".\n"
                     "Entities loading failed.\n\n";
        return false;
    }
}

// Saves the Entities to a save file
bool Level::SaveEntities(const std::string& filename)
{
    std::ofstream outf;
    outf.open(FileManager::ResourcePath() + filename);
    if (outf.is_open())
    {
        std::cout << "Saving Entities...\n";
        outf << m_entities.size() << '\n';
        std::cout << "Number of Entities: " << m_entities.size() << '\n';

        std::cout << "Entities:\n";
        for (const auto& pEntity : m_entities)
        {
            outf << static_cast<int>(pEntity->GetEntityType()) << ' ' << pEntity->GetPosition().x << ' ' << pEntity->GetPosition().y << "\n";
            std::cout << Entity::GetEntityTypeString(pEntity->GetEntityType()) << " at (" << pEntity->GetPosition().x << ", " << pEntity->GetPosition().y << ")\n";
        }

        outf.close();
        std::cout << "Entities successfully saved.\n\n";
        return true;
    }
    else
    {
        std::cout << "Level error: Unable to save \"" << filename << "\".\n"
                     "Entities saving failed.\n\n";
        return false;
    }
}

// Loads the list of necessary resources for the Level from a save file
bool Level::LoadResources(const std::string& filename)
{
    // TO DO
}

// Saves the list of necessary resources for the Level to a save file
bool Level::SaveResources(const std::string& filename)
{
    std::ofstream outf;
    outf.open(FileManager::ResourcePath() + filename);
    if (outf.is_open())
    {
        std::cout << "Saving resources...\n";
        std::vector<std::string> resources;

        // Tiles
        for (unsigned int z = 0; z < m_map.GetLayerCount(); z++)
        {
            for (unsigned int y = 0; y < m_map.GetIndexDimensions().y; y++)
            {
                for (unsigned int x = 0; x < m_map.GetIndexDimensions().x; x++)
                {
                    const Tile* pTile = m_map.GetKTilePtr(sf::Vector2u(x, y), static_cast<MapLayer>(z));
                    if (pTile != nullptr)
                    {
                        Utility::AddIfUnique(resources, Tile::GetTextureName(pTile->GetTileType()));
                    }
                }
            }
        }

        // Entities
        for (const auto& pEntity : m_entities)
        {
            if (pEntity != nullptr)
            {
                std::vector<std::string> entityTextureNames = Entity::GetTextureNames(pEntity->GetEntityType());
                for (const auto& textureName : entityTextureNames)
                {
                    Utility::AddIfUnique(resources, textureName);
                }
            }
        }

        // Background
        resources.emplace_back("parallaxMountains");
        resources.emplace_back("parallaxUnderwater");

        std::cout << "Number of resources: " << resources.size() << '\n';

        for (const auto& resource : resources)
        {
            outf << resource << '\n';
        }

        outf.close();
        std::cout << "Resources successfully saved.\n\n";
        return true;
    }
    else
    {
        std::cout << "Level error: Unable to save \"" << filename << "\".\n"
                     "Resources saving failed.\n\n";
        return false;
    }
}

void Level::HandleInput()
{
    // Return if the Level does not have the GUI focus
    if (m_hasFocus == false)
    {
        return;
    }

    // Display collision boxes
    if (m_inputManager.KeyDown(sf::Keyboard::BackSlash))
    {
        m_displayCollisionBoxes = !m_displayCollisionBoxes;
        for (const auto pEntity : m_entities)
        {
            pEntity->DisplayCollisionBox(m_displayCollisionBoxes);
        }
    }

    // Entities
    if (m_creatorMode == false)
    {
        for (const auto& pEntity : m_entities)
        {
            pEntity->HandleInput();
        }
    }

    // Camera manual control
    float cameraSpeed = 5 * m_camera.GetZoom();
    if (m_inputManager.KeyHeld(sf::Keyboard::I))
    {
        m_camera.Move({0, -cameraSpeed});
    }
    if (m_inputManager.KeyHeld(sf::Keyboard::J))
    {
        m_camera.Move({-cameraSpeed, 0});
    }
    if (m_inputManager.KeyHeld(sf::Keyboard::K))
    {
        m_camera.Move({0, cameraSpeed});
    }
    if (m_inputManager.KeyHeld(sf::Keyboard::L))
    {
        m_camera.Move({cameraSpeed, 0});
    }

    if (m_inputManager.MouseButtonDown(sf::Mouse::Middle))
    {
        m_camera.SetTranslate(m_camera.GetPosition(), GetLevelMousePosition(), 60, true);
    }
    if (m_camera.GetMode() == CameraMode::Static && m_inputManager.KeyDown(sf::Keyboard::Q))
    {
        if (!m_entities.empty() && m_entities[0] != nullptr)
        {
            m_camera.SetFollow(*m_entities[0], true);
        }
    }

    // Camera zoom
    if (m_inputManager.DetectMouseWheelScrolledEvent())
    {
        m_camera.Zoom(1 - m_inputManager.GetMouseWheelDelta() * 0.05);
    }

    // Print the mouse cursor's position when space is held (for testing)
    if (m_inputManager.KeyHeld(sf::Keyboard::Space))
    {
        std::cout << "Mouse position: " << GetLevelMousePosition().x << ", " << GetLevelMousePosition().y << '\n';
    }
}

void Level::Update()
{
    m_map.Update();

    if (m_creatorMode == false)
    {
        for (const auto& pEntity : m_entities)
        {
            pEntity->Update();
        }
    }

    m_camera.Update();
}

void Level::Draw(sf::RenderTarget& rTarget, sf::RenderStates states, float lag)
{
    // Camera
    m_camera.Interpolate(lag);
    rTarget.setView(m_camera.GetView());

    // Parallax background
    for (auto& rParallaxSprite : m_parallaxSprites)
    {
        rParallaxSprite.Update(m_camera);
        rTarget.draw(rParallaxSprite, states);
    }

    // Map and Entities
    rTarget.draw(m_map, states);
    for (const auto& pEntity : m_entities)
    {
        pEntity->Interpolate(lag);
        rTarget.draw(*pEntity, states);
    }
}

// Calls the loading functions for the Entities and the Map (comment à changer et rendre consistent, du genre Load all components ou jsp)
bool Level::Load(const std::string& levelDirectory)
{
    std::cout << "\nLoading Level: " << levelDirectory << "\n\n";
    if (m_map.Load(levelDirectory + "/tiles.txt") &&
        LoadBackground(levelDirectory + "/background.txt") &&
        LoadEntities(levelDirectory + "/entities.txt"))
    {
        m_camera.SetBounds(static_cast<sf::Vector2f>(m_map.GetBounds()));

        if (m_creatorMode == false)
        {
            if (!m_entities.empty())
            {
                m_camera.SetFollow(*m_entities[0], true);
            }
        }

        std::cout << "Level successfully loaded.\n\n";
        return true;
    }
    std::cout << "Failed to load Level.\n\n";
    return false;
}
    
bool Level::Save(const std::string& levelDirectory)
{
    std::cout << "\nSaving Level: " << levelDirectory << "\n\n";
    // TO DO:
    // check if dir exists
        // if not, create it
    // make temp copy and replace if everything works

    if (m_map.Save(levelDirectory + "/tiles.txt") &&
        SaveBackground(levelDirectory + "/background.txt") &&
        SaveEntities(levelDirectory + "/entities.txt") &&
        SaveResources(levelDirectory + "/resources.txt"))
    {
        std::cout << "Level successfully saved.\n\n";
        return true;
    }
    std::cout << "Failed to save Level.\n\n";
    return false;
}

void Level::OnWindowResize()
{
    // Resize Camera to window dimensions
    m_camera.SetDimensions(static_cast<sf::Vector2f>(m_inputManager.GetWindowDimensions()));
}

void Level::SetCreatorMode(bool creatorMode)
{
    m_map.SetDisplayGrid(creatorMode);
    m_camera.SetBoundless(true);
    m_creatorMode = creatorMode;
    if (m_creatorMode == true)
    {
        m_camera.SetMaxDimensions({7680, 4320});
    }
    else
    {
        m_camera.SetMaxDimensions({1920, 1080});
    }
}
