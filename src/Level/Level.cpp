#include "Level/Level.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include "Core/FileManager.h"
#include "Misc/Utility.h"

Level::Level(const ResourceManager& resourceManager, const InputManager& inputManager)
    : m_resourceManager(resourceManager)
    , m_inputManager(inputManager)
    , m_map(m_resourceManager)
    , m_hasFocus(true)
    , m_isCreatorModeEnabled(false)
    , m_isEntityDebugBoxVisible(false)
{
    m_map.SetLayerColor(sf::Color(112, 112, 112, 255), MapLayer::Background);
    m_map.SetLayerColor(sf::Color(255, 255, 255, 192), MapLayer::Overlay);

    m_camera.SetFollowLerp(0.2);
    m_camera.SetBoundless(false);
}

Level::~Level()
{
    for (const auto& pEntity : m_entities)
    {
        delete pEntity;
    }
}

// Load the background from a save file
bool Level::LoadBackground(const std::string& filename)
{
    // Remove all ParallaxSprites (necessary when changing level)
    m_parallaxSprites.clear();

    std::ifstream inputFile(FileManager::ResourcePath() + filename);
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

            m_parallaxSprites.emplace_back(m_resourceManager.GetTexture(resourceName), parallaxValue);
            m_parallaxSprites.back().SetResourceName(resourceName);

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
            if (!positionModeString.empty())
            {
                if (positionModeString == "TL")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0, 0});
                    m_parallaxSprites.back().SetPosition({0, 0});
                }
                else if (positionModeString == "TM")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0.5, 0});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(m_map.GetBounds().x / 2.0f, 0));
                }
                else if (positionModeString == "TR")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 0});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(m_map.GetBounds().x, 0));
                }
                else if (positionModeString == "ML")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0, 0.5});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(0, m_map.GetBounds().y / 2.0f));
                }
                else if (positionModeString == "MM")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0.5, 0.5});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(m_map.GetBounds().x / 2.0f, m_map.GetBounds().y / 2.0f));
                }
                else if (positionModeString == "MR")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 0.5});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(m_map.GetBounds().x, m_map.GetBounds().y / 2.0f));
                }
                else if (positionModeString == "BL")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 1});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(0, m_map.GetBounds().y));
                }
                else if (positionModeString == "BM")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({0.5, 1});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(m_map.GetBounds().x / 2.0f, m_map.GetBounds().y));
                }
                else if (positionModeString == "BR")
                {
                    m_parallaxSprites.back().SetRelativeOrigin({1, 1});
                    m_parallaxSprites.back().SetPosition(sf::Vector2f(m_map.GetBounds().x, m_map.GetBounds().y));
                }
            }

            // Repeat texture
            std::string repeatTextureString = m_parallaxSprites.back().GetRepeatTextureString();
            if (!repeatTextureString.empty())
            {
                if (repeatTextureString == "XY")
                {
                    m_parallaxSprites.back().SetTextureRect(static_cast<sf::Vector2f>(m_map.GetBounds()));
                }
                else if (repeatTextureString == "X")
                {
                    m_parallaxSprites.back().SetTextureRect(
                        sf::Vector2f(m_map.GetBounds().x, m_parallaxSprites.back().GetTexture()->getSize().y));
                }
                else if (repeatTextureString == "Y")
                {
                    m_parallaxSprites.back().SetTextureRect(
                        sf::Vector2f(m_parallaxSprites.back().GetTexture()->getSize().x, m_map.GetBounds().y));
                }
            }

            // Scale
            std::string scaleString = m_parallaxSprites.back().GetScaleString();
            if (!scaleString.empty())
            {
                if (scaleString == "map")
                {
                    float scale = Utility::GetScaleToFill(static_cast<sf::Vector2f>(m_parallaxSprites.back().GetTexture()->getSize()),
                                                          static_cast<sf::Vector2f>(m_map.GetBounds()));
                    m_parallaxSprites.back().SetScale({scale, scale});
                }
                else
                {
                    std::size_t commaPos = scaleString.find(',');
                    float x = std::stof(scaleString.substr(0, commaPos));
                    float y = std::stof(scaleString.substr(commaPos + 1));
                    m_parallaxSprites.back().SetScale({x, y});
                }
            }

            // Offset
            std::string offsetString = m_parallaxSprites.back().GetOffsetString();
            if (!offsetString.empty())
            {
                std::size_t commaPos = offsetString.find(',');
                float x = std::stof(offsetString.substr(0, commaPos));
                float y = std::stof(offsetString.substr(commaPos + 1));
                m_parallaxSprites.back().SetPosition(m_parallaxSprites.back().GetPosition() + sf::Vector2f(x, y));
            }
        }

        std::sort(m_parallaxSprites.begin(), m_parallaxSprites.end(), [](const ParallaxSprite& a, const ParallaxSprite& b) {
            return a.GetParallax() > b.GetParallax();
        });

        std::cout << "Background successfully loaded.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to open \"" << filename << "\".\n"
              << "Background loading failed.\n\n";
    return false;
}

// Save the background to a save file
bool Level::SaveBackground(const std::string& filename) const
{
    std::ofstream outputFile(FileManager::ResourcePath() + filename);
    if (outputFile)
    {
        std::cout << "Saving background...\n";
        outputFile << "# Syntax:\n"
                      "# resourceName parallaxValue pos:[TL|TM|TR|ML|MM|MR|BL|BM|BR] repeatTexture[-o]:[XY|X|Y] "
                      "scale[-o]:[x,y|map] offset[-o]:x,y\n\n";

        std::vector<ParallaxSprite> sortedParallaxSprites(m_parallaxSprites);
        std::sort(sortedParallaxSprites.begin(), sortedParallaxSprites.end(), [](const ParallaxSprite& a, const ParallaxSprite& b) {
            return a.GetParallax() > b.GetParallax();
        });

        std::cout << "Number of parallax sprites: " << m_parallaxSprites.size() << '\n';

        std::cout << "Parallax sprites:\n";
        for (const auto& parallaxSprite : m_parallaxSprites)
        {
            outputFile << parallaxSprite.GetResourceName() << ' ' << parallaxSprite.GetParallax()
                       << " positionMode:" << parallaxSprite.GetPositionModeString();
            if (!parallaxSprite.GetRepeatTextureString().empty())
            {
                outputFile << " repeatTexture:" << parallaxSprite.GetRepeatTextureString();
            }
            if (!parallaxSprite.GetScaleString().empty())
            {
                outputFile << " scale:" << parallaxSprite.GetScaleString();
            }
            if (!parallaxSprite.GetOffsetString().empty())
            {
                outputFile << " offset:" << parallaxSprite.GetOffsetString();
            }
            outputFile << '\n';

            std::cout << parallaxSprite.GetResourceName() << ' ' << parallaxSprite.GetParallax()
                      << " positionMode:" << parallaxSprite.GetPositionModeString() << '\n';
        }

        std::cout << "Background successfully saved.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to save \"" << filename << "\".\n"
              << "Background saving failed.\n\n";
    return false;
}

// Load the Entities from a save file
bool Level::LoadEntities(const std::string& filename)
{
    // Remove all Entities (necessary when changing level)
    for (const auto& pEntity : m_entities)
    {
        delete pEntity;
    }

    std::ifstream inputFile(FileManager::ResourcePath() + filename);
    if (inputFile)
    {
        std::cout << "Loading Entities...\n";
        unsigned int entityCount = 0;
        inputFile >> entityCount;
        std::cout << "Number of Entities: " << entityCount << '\n';

        m_entities.resize(entityCount);
        std::cout << "Entities:\n";
        for (auto& rpEntity : m_entities)
        {
            unsigned int type = 0;
            float xPosition = 0;
            float yPosition = 0;
            inputFile >> type >> xPosition >> yPosition;
            switch (static_cast<EntityType>(type))
            {
            case EntityType::Player:
                rpEntity = new Player(m_map, m_entities, m_inputManager, sf::Vector2f(xPosition, yPosition));
                rpEntity->AddAnimation(EntityState::Still,
                                       AnimatedSprite(m_resourceManager.GetTexture("characterStill"), sf::Vector2u(54, 82), 22, 3));
                rpEntity->AddAnimation(EntityState::Running,
                                       AnimatedSprite(m_resourceManager.GetTexture("characterRunning"), sf::Vector2u(82, 82), 27, 1));
                rpEntity->AddAnimation(EntityState::Climbing,
                                       AnimatedSprite(m_resourceManager.GetTexture("characterClimbing"),
                                                      sf::Vector2u(70, 82),
                                                      8,
                                                      3,
                                                      true,
                                                      false));
                rpEntity->AddAnimation(EntityState::Jumping,
                                       AnimatedSprite(m_resourceManager.GetTexture("characterJumping"), sf::Vector2u(66, 82), 3, 2));
                rpEntity->AddAnimation(EntityState::Falling,
                                       AnimatedSprite(m_resourceManager.GetTexture("characterFalling"), sf::Vector2u(72, 82), 3, 2));
                rpEntity->SetPosition({xPosition, yPosition});
                break;
            default:
                break;
            }
            std::cout << Entity::GetEntityTypeString(rpEntity->GetEntityType()) << " at (" << rpEntity->GetPosition().x << ", "
                      << rpEntity->GetPosition().y << ")\n";
        }

        std::cout << "Entities successfully loaded.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to open \"" << filename << "\".\n"
              << "Entities loading failed.\n\n";
    return false;
}

// Save the Entities to a save file
bool Level::SaveEntities(const std::string& filename) const
{
    std::ofstream outputFile(FileManager::ResourcePath() + filename);
    if (outputFile)
    {
        std::cout << "Saving Entities...\n";
        outputFile << m_entities.size() << '\n';
        std::cout << "Number of Entities: " << m_entities.size() << '\n';

        std::cout << "Entities:\n";
        for (const auto& pEntity : m_entities)
        {
            outputFile << static_cast<int>(pEntity->GetEntityType()) << ' ' << pEntity->GetPosition().x << ' ' << pEntity->GetPosition().y
                       << "\n";
            std::cout << Entity::GetEntityTypeString(pEntity->GetEntityType()) << " at (" << pEntity->GetPosition().x << ", "
                      << pEntity->GetPosition().y << ")\n";
        }

        std::cout << "Entities successfully saved.\n\n";
        return true;
    }

    std::cerr << "Level error: Unable to save \"" << filename << "\".\n"
              << "Entities saving failed.\n\n";
    return false;
}

// Load the list of necessary resources for the Level from a save file
bool Level::LoadResources(const std::string& filename)
{
    // TODO
    return true;
}

// Save the list of necessary resources for the Level to a save file
bool Level::SaveResources(const std::string& filename) const
{
    std::ofstream outputFile(FileManager::ResourcePath() + filename);
    if (outputFile)
    {
        std::cout << "Saving resources...\n";

        std::set<std::string> resources;

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
                        resources.insert(Tile::GetTextureName(pTile->GetTileType()));
                    }
                }
            }
        }

        // Background
        for (const auto& parallaxSprite : m_parallaxSprites)
        {
            resources.insert(parallaxSprite.GetResourceName());
        }

        // Entities
        for (const auto& pEntity : m_entities)
        {
            if (pEntity != nullptr)
            {
                std::vector<std::string> entityTextureNames = Entity::GetTextureNames(pEntity->GetEntityType());
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

void Level::HandleInput()
{
    // Do not handle input if the Level does not have the GUI focus
    if (m_hasFocus == false)
    {
        return;
    }

    // Show debug boxes
    if (m_inputManager.IsKeyDescending(sf::Keyboard::BackSlash))
    {
        m_isEntityDebugBoxVisible = !m_isEntityDebugBoxVisible;
        for (const auto& pEntity : m_entities)
        {
            pEntity->SetDebugBoxVisible(m_isEntityDebugBoxVisible);
        }
    }

    // Entities
    if (m_isCreatorModeEnabled == false)
    {
        for (const auto& pEntity : m_entities)
        {
            pEntity->HandleInput();
        }
    }

    // Camera manual control
    float cameraSpeed = 5 * m_camera.GetZoom();
    if (m_inputManager.IsKeyHeld(sf::Keyboard::I))
    {
        m_camera.Move({0, -cameraSpeed});
    }
    if (m_inputManager.IsKeyHeld(sf::Keyboard::J))
    {
        m_camera.Move({-cameraSpeed, 0});
    }
    if (m_inputManager.IsKeyHeld(sf::Keyboard::K))
    {
        m_camera.Move({0, cameraSpeed});
    }
    if (m_inputManager.IsKeyHeld(sf::Keyboard::L))
    {
        m_camera.Move({cameraSpeed, 0});
    }

    if (m_inputManager.IsMouseButtonDescending(sf::Mouse::Middle))
    {
        m_camera.SetTranslate(m_camera.GetPosition(), GetLevelMousePosition(), 60, true);
    }
    if (m_camera.GetMode() == CameraMode::Static && m_inputManager.IsKeyDescending(sf::Keyboard::Q))
    {
        if (!m_entities.empty() && m_entities.front() != nullptr)
        {
            m_camera.SetFollow(*m_entities.front(), true);
        }
    }

    // Camera zoom
    if (m_inputManager.DetectedMouseWheelScrolledEvent())
    {
        m_camera.Zoom(1 - m_inputManager.GetVerticalMouseWheelDelta() * 0.05);
    }

    // Print the mouse cursor's position when space is held (for testing)
    if (m_inputManager.IsKeyHeld(sf::Keyboard::Space))
    {
        std::cout << "Mouse position: " << GetLevelMousePosition().x << ", " << GetLevelMousePosition().y << '\n';
    }
}

void Level::Update()
{
    m_map.Update();

    if (m_isCreatorModeEnabled == false)
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

    // Change view to Camera view
    sf::View oldView = rTarget.getView();
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

    // Reset the target's view back to its initial view
    rTarget.setView(oldView);
}

// Load all Level components, such as the Map, the Entities and the Parallax background
bool Level::Load(const std::string& levelDirectory)
{
    std::cout << "\nLoading Level: " << levelDirectory << "\n\n";
    if (m_map.Load(levelDirectory + "/tiles.txt") && LoadBackground(levelDirectory + "/background.txt") &&
        LoadEntities(levelDirectory + "/entities.txt") && LoadResources(levelDirectory + "/resources.txt"))
    {
        m_camera.SetBounds(static_cast<sf::Vector2f>(m_map.GetBounds()));

        if (m_isCreatorModeEnabled == false)
        {
            if (!m_entities.empty() && m_entities.front() != nullptr)
            {
                m_camera.SetFollow(*m_entities.front(), true);
            }
        }

        std::cout << "Level successfully loaded.\n\n";
        return true;
    }

    std::cout << "Failed to load Level.\n\n";
    return false;
}

bool Level::Save(const std::string& levelDirectory) const
{
    std::cout << "\nSaving Level: " << levelDirectory << "\n\n";
    // TODO
    // check if dir exists
    // if not, create it
    // make temp copy and replace if everything works

    if (m_map.Save(levelDirectory + "/tiles.txt") && SaveBackground(levelDirectory + "/background.txt") &&
        SaveEntities(levelDirectory + "/entities.txt") && SaveResources(levelDirectory + "/resources.txt"))
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
    float oldZoom = m_camera.GetZoom();
    m_camera.SetDimensions(static_cast<sf::Vector2f>(m_inputManager.GetWindowDimensions()));
    m_camera.SetZoom(oldZoom);
}

void Level::SetCreatorModeEnabled(bool isCreatorModeEnabled)
{
    m_isCreatorModeEnabled = isCreatorModeEnabled;
    m_map.SetGridVisible(m_isCreatorModeEnabled);
    m_camera.SetBoundless(m_isCreatorModeEnabled);
    if (m_isCreatorModeEnabled == true)
    {
        m_camera.SetMaxDimensions({7680, 4320});
    }
    else
    {
        m_camera.SetMaxDimensions({2560, 1440});
    }
}
