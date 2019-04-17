#include "States/CreatorState.h"
#include <algorithm>
#include <iostream>
#include <random>
#include "Core/FileManager.h"
#include "States/PauseState.h"

CreatorState::CreatorState(GameEngine& rGame)
    : State(rGame)
    , m_loadLevelLabel("Load Level", m_rGame.resourceManager.GetFont("altFont"), 20)
    , m_saveLevelLabel("Save Level as: ", m_rGame.resourceManager.GetFont("altFont"), 20)
    , m_tileNameLabel("Selected tile", m_rGame.resourceManager.GetFont("altFont"), 20)
    , m_loadLevelTextBox(m_rGame.inputManager, m_rGame.resourceManager.GetFont("altFont"))
    , m_saveLevelTextBox(m_rGame.inputManager, m_rGame.resourceManager.GetFont("altFont"))
    , m_widthTextBox(m_rGame.inputManager, m_rGame.resourceManager.GetFont("altFont"))
    , m_heightTextBox(m_rGame.inputManager, m_rGame.resourceManager.GetFont("altFont"))
    , m_tileNameTextBox(m_rGame.inputManager, m_rGame.resourceManager.GetFont("altFont"))
    , m_createLevelButton(m_rGame.resourceManager.GetFont("altFont"),
                          m_rGame.resourceManager.GetSoundBuffer("click"), sf::Vector2f(0, 0),
                          sf::Vector2f(230, 30), -2, 6, "Create Level", GuiStyle::Green)
    , m_level(m_rGame.resourceManager, m_rGame.inputManager)
    , m_selectableTileTypes{TileType::Grass4Sides, TileType::Wood, TileType::Ladder, TileType::LadderTop, TileType::Vine}
    , m_selectedTileTypeIndex(0)
    , m_musicNumber(0)
    , m_brushSize(1)
{
    // State settings
    m_stateSettings.backgroundColor = sf::Color(172, 172, 172);

    // Initialize GUI
    m_panel.setFillColor(sf::Color(235, 235, 235, 235));
    m_panel.setOutlineColor(sf::Color(0, 0, 0, 235));
    m_panel.setOutlineThickness(-1);

    m_loadLevelLabel.setFillColor(sf::Color::Black);
    m_saveLevelLabel.setFillColor(sf::Color::Black);
    m_tileNameLabel.setFillColor(sf::Color::Black);

    m_loadLevelTextBox.SetBackgroundText("Level name");
    m_loadLevelTextBox.SetDimensions({230, 30});
    m_loadLevelTextBox.SetCharacterSize(20);

    m_saveLevelTextBox.SetBackgroundText("Level name");
    m_saveLevelTextBox.SetDimensions({230, 30});
    m_saveLevelTextBox.SetCharacterSize(20);

    m_widthTextBox.SetBackgroundText("Width");
    m_widthTextBox.SetDimensions({110, 30});
    m_widthTextBox.SetCharacterSize(20);
    m_widthTextBox.SetDigitsOnly(true);

    m_heightTextBox.SetBackgroundText("Height");
    m_heightTextBox.SetDimensions({110, 30});
    m_heightTextBox.SetCharacterSize(20);
    m_heightTextBox.SetDigitsOnly(true);

    m_tileNameTextBox.SetDimensions({230, 30});
    m_tileNameTextBox.SetCharacterSize(20);
    m_tileNameTextBox.SetReadOnly(true);
    m_tileNameTextBox.SetText(Tile::GetTileTypeString(m_selectableTileTypes[m_selectedTileTypeIndex]));

    m_createLevelButton.SetVolume(0.75);

    // Music settings
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(1, 4);
    m_musicNumber = distribution(generator);

    SetMusic();

    m_level.SetCreatorModeEnabled(true);
}

CreatorState::CreatorState(GameEngine& rGame, const std::string& levelDirectory)
    : CreatorState(rGame)
{
    m_level.Load(levelDirectory);
}

CreatorState::~CreatorState()
{
}

void CreatorState::SetMusic()
{
    /*
    if (m_musicNumber >= 0 && m_musicNumber <= 3)
    {
        m_music.openFromFile(ResourcePath() + "res/music/build" + std::to_string(m_musicNumber + 1) + ".wav");
        m_music.play();
    }
    */
}

void CreatorState::PauseStart()
{
    m_rGame.RequestPush(new PauseState(m_rGame));
}

sf::Vector2u CreatorState::GetBrushTopLeftIndex(const sf::Vector2f& mousePosition)
{
    sf::Vector2f coords = mousePosition - sf::Vector2f((m_brushSize - 1.0f) / 2.0f * m_level.GetTileSize(),
                                                       (m_brushSize - 1.0f) / 2.0f * m_level.GetTileSize());
    return m_level.CoordsToTileIndex(coords);
}

void CreatorState::ClearPreviewMap()
{
    sf::Vector2u brushTopLeftIndex = GetBrushTopLeftIndex(m_previousMousePosition);
    for (unsigned int i = 0; i < m_brushSize; i++)
    {
        for (unsigned int j = 0; j < m_brushSize; j++)
        {
            m_level.RemoveTile(sf::Vector2u(brushTopLeftIndex.x + i, brushTopLeftIndex.y + j), MapLayer::Overlay);
        }
    }
}

bool CreatorState::CheckMouseChangedTile()
{
    return (GetBrushTopLeftIndex(m_previousMousePosition) != GetBrushTopLeftIndex(m_currentMousePosition));
}

void CreatorState::HandleInput()
{
    if (m_rGame.inputManager.DetectedLostFocusEvent() || m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Escape))
    {
        PauseStart();
        return;
    }

    m_previousMousePosition = m_currentMousePosition;
    m_currentMousePosition = m_level.GetLevelMousePosition();

    // Tile selection
    if ((m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Q) || m_rGame.inputManager.IsKeyDescending(sf::Keyboard::E) ||
         m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Up) || m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Down)) &&
        !m_loadLevelTextBox.HasFocus() && !m_saveLevelTextBox.HasFocus() && !m_widthTextBox.HasFocus() && !m_heightTextBox.HasFocus())
    {
        if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::E) || m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Up))
        {
            if (m_selectedTileTypeIndex >= m_selectableTileTypes.size() - 1)
            {
                m_selectedTileTypeIndex = 0;
            }
            else
            {
                m_selectedTileTypeIndex++;
            }
        }
        else if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Q) || m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Down))
        {
            if (m_selectedTileTypeIndex == 0 && !m_selectableTileTypes.empty())
            {
                m_selectedTileTypeIndex = m_selectableTileTypes.size() - 1;
            }
            else
            {
                m_selectedTileTypeIndex--;
            }
        }
        m_tileNameTextBox.SetText(Tile::GetTileTypeString(m_selectableTileTypes[m_selectedTileTypeIndex]));
    }

    // Brush size
    if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Add))
    {
        ClearPreviewMap();
        m_brushSize++;
    }
    if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Subtract) && m_brushSize > 1)
    {
        ClearPreviewMap();
        m_brushSize--;
    }

    // Preview Map
    if (CheckMouseChangedTile() || // Mouse changed Tile
        m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Add) || m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Subtract) || // Brush
        m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Q) || m_rGame.inputManager.IsKeyDescending(sf::Keyboard::E)) // Change Tile
    {
        ClearPreviewMap();
        m_level.AddTileRange(m_selectableTileTypes[m_selectedTileTypeIndex],
                             GetBrushTopLeftIndex(m_currentMousePosition),
                             sf::Vector2u(m_brushSize, m_brushSize),
                             MapLayer::Overlay,
                             true);
    }

    // Tile placement or removal
    if (GetWindowMousePosition().x <= m_panel.getPosition().x &&
        (CheckMouseChangedTile() || m_rGame.inputManager.DetectedMouseButtonPressedEvent()) &&
        (m_rGame.inputManager.IsMouseButtonHeld(sf::Mouse::Left) || m_rGame.inputManager.IsMouseButtonHeld(sf::Mouse::Right)))
    {
        // Add Tile
        if (m_rGame.inputManager.IsMouseButtonHeld(sf::Mouse::Left))
        {
            m_level.AddTileRange(m_selectableTileTypes[m_selectedTileTypeIndex],
                                 GetBrushTopLeftIndex(m_currentMousePosition),
                                 sf::Vector2u(m_brushSize, m_brushSize),
                                 MapLayer::Solid,
                                 true);
        }
        // Remove Tile
        else
        {
            m_level.RemoveTileRange(GetBrushTopLeftIndex(m_currentMousePosition),
                                    sf::Vector2u(m_brushSize, m_brushSize),
                                    MapLayer::Solid,
                                    true);
        }
    }

    // Gui
    m_loadLevelTextBox.HandleInput();
    m_saveLevelTextBox.HandleInput();
    m_widthTextBox.HandleInput();
    m_heightTextBox.HandleInput();

    if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Return))
    {
        // Loading
        if (m_loadLevelTextBox.HasFocus())
        {
            if (m_level.Load("data/levels/" + m_loadLevelTextBox.GetText()))
            {
                m_loadLevelTextBox.SetText("");
                m_loadLevelTextBox.SetFocus(false);
                m_widthTextBox.SetText(std::to_string(m_level.GetMapIndexDimensions().x));
                m_heightTextBox.SetText(std::to_string(m_level.GetMapIndexDimensions().y));
            }
        }
        // Saving
        else if (m_saveLevelTextBox.HasFocus() && !m_saveLevelTextBox.GetText().isEmpty())
        {
            m_level.Save("data/levels/" + m_saveLevelTextBox.GetText());
        }
        // Resizing
        else if (m_heightTextBox.HasFocus() || m_widthTextBox.HasFocus())
        {
            sf::Vector2u newMapIndexDimensions = sf::Vector2u(0, 0);
            if (!m_widthTextBox.GetText().isEmpty())
            {
                newMapIndexDimensions.x = std::stoi(m_widthTextBox.GetText().toAnsiString());
            }
            if (!m_heightTextBox.GetText().isEmpty())
            {
                newMapIndexDimensions.y = std::stoi(m_heightTextBox.GetText().toAnsiString());
            }
            m_level.Resize(newMapIndexDimensions);
        }
    }

    if (m_rGame.inputManager.DetectedMouseMovedEvent())
    {
        m_createLevelButton.OnMouseHover(GetWindowMousePosition());
    }
    if (m_rGame.inputManager.IsMouseButtonDescending(sf::Mouse::Left))
    {
        m_createLevelButton.OnMouseClick(GetWindowMousePosition());
    }
    if (m_rGame.inputManager.IsMouseButtonAscending(sf::Mouse::Left))
    {
        if (m_createLevelButton.OnMouseUnclick(GetWindowMousePosition()) == true)
        {
            m_level.Resize({10, 10});
        }
    }

    // Level focus
    if (m_loadLevelTextBox.HasFocus() || m_saveLevelTextBox.HasFocus() || m_widthTextBox.HasFocus() || m_heightTextBox.HasFocus() ||
        GetWindowMousePosition().x >= m_panel.getPosition().x)
    {
        m_level.SetFocus(false);
    }
    else
    {
        m_level.SetFocus(true);
    }

    m_level.HandleInput();
}

void CreatorState::Update()
{
    m_level.Update();

    m_loadLevelTextBox.Update();
    m_saveLevelTextBox.Update();
    m_widthTextBox.Update();
    m_heightTextBox.Update();

    // Music rotation
    if (m_music.getStatus() == sf::SoundSource::Stopped)
    {
        m_musicNumber++;
        if (m_musicNumber > 4)
        {
            m_musicNumber = 0;
        }
        SetMusic();
    }
}

void CreatorState::Draw(sf::RenderTarget& rTarget, float lag)
{
    DrawBackgroundColor(rTarget);

    m_level.Draw(rTarget, sf::RenderStates::Default, lag);

    rTarget.draw(m_panel);

    rTarget.draw(m_loadLevelLabel);
    rTarget.draw(m_loadLevelTextBox);

    rTarget.draw(m_saveLevelLabel);
    rTarget.draw(m_saveLevelTextBox);

    rTarget.draw(m_widthTextBox);
    rTarget.draw(m_heightTextBox);

    rTarget.draw(m_createLevelButton);

    rTarget.draw(m_tileNameLabel);
    rTarget.draw(m_tileNameTextBox);
}

void CreatorState::OnWindowResize()
{
    m_panel.setSize(sf::Vector2f(250, GetWindowDimensions().y));
    m_panel.setPosition(GetWindowDimensions().x - m_panel.getSize().x, 0);

    m_loadLevelLabel.setPosition(m_panel.getPosition() + sf::Vector2f(10, 10));
    m_loadLevelTextBox.SetPosition(m_loadLevelLabel.getPosition() + sf::Vector2f(0, 30));

    m_saveLevelLabel.setPosition(m_loadLevelTextBox.GetPosition() + sf::Vector2f(0, 40));
    m_saveLevelTextBox.SetPosition(m_saveLevelLabel.getPosition() + sf::Vector2f(0, 30));

    m_widthTextBox.SetPosition(m_saveLevelTextBox.GetPosition() + sf::Vector2f(0, 40));
    m_heightTextBox.SetPosition(m_widthTextBox.GetPosition() + sf::Vector2f(m_widthTextBox.GetDimensions().x + 10, 0));

    m_createLevelButton.SetPosition(m_widthTextBox.GetPosition() + sf::Vector2f(0, 40) + m_saveLevelTextBox.GetDimensions() / 2.0f);

    m_tileNameLabel.setPosition(m_widthTextBox.GetPosition() + sf::Vector2f(0, 80));
    m_tileNameTextBox.SetPosition(m_tileNameLabel.getPosition() + sf::Vector2f(0, 30));

    m_level.OnWindowResize();
}
