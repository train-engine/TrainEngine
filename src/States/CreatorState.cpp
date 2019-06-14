#include "States/CreatorState.h"
#include <random>
#include "Core/FileManager.h"
#include "States/PauseState.h"

CreatorState::CreatorState(GameEngine& game)
    : State(game)
    , m_loadLevelLabel("Load Level", m_game.resourceManager.getFont("altFont"), 20)
    , m_saveLevelLabel("Save Level as: ", m_game.resourceManager.getFont("altFont"), 20)
    , m_tileNameLabel("Selected tile", m_game.resourceManager.getFont("altFont"), 20)
    , m_loadLevelTextBox(m_game.inputManager, m_game.resourceManager.getFont("altFont"))
    , m_saveLevelTextBox(m_game.inputManager, m_game.resourceManager.getFont("altFont"))
    , m_widthTextBox(m_game.inputManager, m_game.resourceManager.getFont("altFont"))
    , m_heightTextBox(m_game.inputManager, m_game.resourceManager.getFont("altFont"))
    , m_tileNameTextBox(m_game.inputManager, m_game.resourceManager.getFont("altFont"))
    , m_createLevelButton(m_game.resourceManager.getFont("altFont"),
                          m_game.resourceManager.getSoundBuffer("click"), sf::Vector2f(0, 0),
                          sf::Vector2f(230, 30), -2, 6, "Create Level", GuiStyle::Green)
    , m_level(m_game.resourceManager, m_game.inputManager)
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

    m_loadLevelTextBox.setPlaceholderText("Level name");
    m_loadLevelTextBox.setDimensions({230, 30});
    m_loadLevelTextBox.setCharacterSize(20);

    m_saveLevelTextBox.setPlaceholderText("Level name");
    m_saveLevelTextBox.setDimensions({230, 30});
    m_saveLevelTextBox.setCharacterSize(20);

    m_widthTextBox.setPlaceholderText("Width");
    m_widthTextBox.setDimensions({110, 30});
    m_widthTextBox.setCharacterSize(20);
    m_widthTextBox.setDigitsOnly(true);

    m_heightTextBox.setPlaceholderText("Height");
    m_heightTextBox.setDimensions({110, 30});
    m_heightTextBox.setCharacterSize(20);
    m_heightTextBox.setDigitsOnly(true);

    m_tileNameTextBox.setDimensions({230, 30});
    m_tileNameTextBox.setCharacterSize(20);
    m_tileNameTextBox.setReadOnly(true);
    m_tileNameTextBox.setText(Tile::getTileTypeString(m_selectableTileTypes[m_selectedTileTypeIndex]));

    m_createLevelButton.setVolume(0.75);

    // Music settings
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(1, 4);
    m_musicNumber = distribution(generator);

    setMusic();

    m_level.setCreatorModeEnabled(true);
}

CreatorState::CreatorState(GameEngine& game, const std::string& levelDirectory)
    : CreatorState(game)
{
    m_level.load(levelDirectory);
}

CreatorState::~CreatorState()
{
}

void CreatorState::setMusic()
{
    /*
    if (m_musicNumber >= 0 && m_musicNumber <= 3)
    {
        m_music.openFromFile(ResourcePath() + "res/music/build" + std::to_string(m_musicNumber + 1) + ".wav");
        m_music.play();
    }
    */
}

void CreatorState::pauseStart()
{
    m_game.requestPush(new PauseState(m_game));
}

sf::Vector2u CreatorState::getBrushTopLeftIndex(const sf::Vector2f& mousePosition)
{
    sf::Vector2f coords = mousePosition - sf::Vector2f((m_brushSize - 1.0f) / 2.0f * m_level.getTileSize(),
                                                       (m_brushSize - 1.0f) / 2.0f * m_level.getTileSize());
    return m_level.coordsToTileIndex(coords);
}

void CreatorState::clearPreviewMap()
{
    sf::Vector2u brushTopLeftIndex = getBrushTopLeftIndex(m_previousMousePosition);
    for (unsigned int i = 0; i < m_brushSize; i++)
    {
        for (unsigned int j = 0; j < m_brushSize; j++)
        {
            m_level.removeTile(sf::Vector2u(brushTopLeftIndex.x + i, brushTopLeftIndex.y + j), MapLayer::Overlay);
        }
    }
}

bool CreatorState::checkMouseChangedTile()
{
    return getBrushTopLeftIndex(m_previousMousePosition) != getBrushTopLeftIndex(m_currentMousePosition);
}

void CreatorState::handleInput()
{
    if (m_game.inputManager.detectedLostFocusEvent() || m_game.inputManager.isKeyDescending(sf::Keyboard::Escape))
    {
        pauseStart();
        return;
    }

    m_previousMousePosition = m_currentMousePosition;
    m_currentMousePosition = m_level.getLevelMousePosition();

    // Tile selection
    if ((m_game.inputManager.isKeyDescending(sf::Keyboard::Q) || m_game.inputManager.isKeyDescending(sf::Keyboard::E) ||
         m_game.inputManager.isKeyDescending(sf::Keyboard::Up) || m_game.inputManager.isKeyDescending(sf::Keyboard::Down)) &&
        !m_loadLevelTextBox.hasFocus() && !m_saveLevelTextBox.hasFocus() && !m_widthTextBox.hasFocus() && !m_heightTextBox.hasFocus())
    {
        if (m_game.inputManager.isKeyDescending(sf::Keyboard::E) || m_game.inputManager.isKeyDescending(sf::Keyboard::Up))
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
        else if (m_game.inputManager.isKeyDescending(sf::Keyboard::Q) || m_game.inputManager.isKeyDescending(sf::Keyboard::Down))
        {
            if (m_selectedTileTypeIndex == 0)
            {
                m_selectedTileTypeIndex = m_selectableTileTypes.size() - 1;
            }
            else
            {
                m_selectedTileTypeIndex--;
            }
        }
        m_tileNameTextBox.setText(Tile::getTileTypeString(m_selectableTileTypes[m_selectedTileTypeIndex]));
    }

    // Brush size
    if (m_game.inputManager.isKeyDescending(sf::Keyboard::Add))
    {
        clearPreviewMap();
        m_brushSize++;
    }
    if (m_game.inputManager.isKeyDescending(sf::Keyboard::Subtract) && m_brushSize > 1)
    {
        clearPreviewMap();
        m_brushSize--;
    }

    // Preview Map
    if (checkMouseChangedTile() || // Mouse changed Tile
        m_game.inputManager.isKeyDescending(sf::Keyboard::Add) || m_game.inputManager.isKeyDescending(sf::Keyboard::Subtract) || // Brush
        m_game.inputManager.isKeyDescending(sf::Keyboard::Q) || m_game.inputManager.isKeyDescending(sf::Keyboard::E)) // Change Tile
    {
        clearPreviewMap();
        m_level.addTileRange(m_selectableTileTypes[m_selectedTileTypeIndex],
                             getBrushTopLeftIndex(m_currentMousePosition),
                             sf::Vector2u(m_brushSize, m_brushSize),
                             MapLayer::Overlay,
                             true);
    }

    // Tile placement or removal
    if (getWindowMousePosition().x <= m_panel.getPosition().x &&
        (checkMouseChangedTile() || m_game.inputManager.detectedMouseButtonPressedEvent()) &&
        (m_game.inputManager.isMouseButtonHeld(sf::Mouse::Left) || m_game.inputManager.isMouseButtonHeld(sf::Mouse::Right)))
    {
        // Add Tile
        if (m_game.inputManager.isMouseButtonHeld(sf::Mouse::Left))
        {
            m_level.addTileRange(m_selectableTileTypes[m_selectedTileTypeIndex],
                                 getBrushTopLeftIndex(m_currentMousePosition),
                                 sf::Vector2u(m_brushSize, m_brushSize),
                                 MapLayer::Solid,
                                 true);
        }
        // Remove Tile
        else
        {
            m_level.removeTileRange(getBrushTopLeftIndex(m_currentMousePosition),
                                    sf::Vector2u(m_brushSize, m_brushSize),
                                    MapLayer::Solid,
                                    true);
        }
    }

    // Gui
    m_loadLevelTextBox.handleInput();
    m_saveLevelTextBox.handleInput();
    m_widthTextBox.handleInput();
    m_heightTextBox.handleInput();

    if (m_game.inputManager.isKeyDescending(sf::Keyboard::Return))
    {
        // Loading
        if (m_loadLevelTextBox.hasFocus())
        {
            if (m_level.load("data/levels/" + m_loadLevelTextBox.getText()))
            {
                m_loadLevelTextBox.setText("");
                m_loadLevelTextBox.setFocus(false);
                m_widthTextBox.setText(std::to_string(m_level.getMapIndexDimensions().x));
                m_heightTextBox.setText(std::to_string(m_level.getMapIndexDimensions().y));
            }
        }
        // Saving
        else if (m_saveLevelTextBox.hasFocus() && !m_saveLevelTextBox.getText().isEmpty())
        {
            m_level.save("data/levels/" + m_saveLevelTextBox.getText());
        }
        // Resizing
        else if (m_heightTextBox.hasFocus() || m_widthTextBox.hasFocus())
        {
            sf::Vector2u newMapIndexDimensions = sf::Vector2u(0, 0);
            if (!m_widthTextBox.getText().isEmpty())
            {
                newMapIndexDimensions.x = std::stoi(m_widthTextBox.getText().toAnsiString());
            }
            if (!m_heightTextBox.getText().isEmpty())
            {
                newMapIndexDimensions.y = std::stoi(m_heightTextBox.getText().toAnsiString());
            }
            m_level.resize(newMapIndexDimensions);
        }
    }

    if (m_game.inputManager.detectedMouseMovedEvent())
    {
        m_createLevelButton.onMouseHover(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonDescending(sf::Mouse::Left))
    {
        m_createLevelButton.onMouseClick(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonAscending(sf::Mouse::Left))
    {
        if (m_createLevelButton.onMouseUnclick(getWindowMousePosition()) == true)
        {
            m_level.resize({10, 10});
        }
    }

    // Level focus
    if (m_loadLevelTextBox.hasFocus() || m_saveLevelTextBox.hasFocus() || m_widthTextBox.hasFocus() || m_heightTextBox.hasFocus() ||
        getWindowMousePosition().x >= m_panel.getPosition().x)
    {
        m_level.setFocus(false);
    }
    else
    {
        m_level.setFocus(true);
    }

    m_level.handleInput();
}

void CreatorState::update()
{
    m_level.update();

    m_loadLevelTextBox.update();
    m_saveLevelTextBox.update();
    m_widthTextBox.update();
    m_heightTextBox.update();

    // Music rotation
    if (m_music.getStatus() == sf::SoundSource::Stopped)
    {
        m_musicNumber++;
        if (m_musicNumber > 4)
        {
            m_musicNumber = 0;
        }
        setMusic();
    }
}

void CreatorState::draw(sf::RenderTarget& target, float lag)
{
    drawBackgroundColor(target);

    m_level.draw(target, sf::RenderStates::Default, lag);

    target.draw(m_panel);

    target.draw(m_loadLevelLabel);
    target.draw(m_loadLevelTextBox);

    target.draw(m_saveLevelLabel);
    target.draw(m_saveLevelTextBox);

    target.draw(m_widthTextBox);
    target.draw(m_heightTextBox);

    target.draw(m_createLevelButton);

    target.draw(m_tileNameLabel);
    target.draw(m_tileNameTextBox);
}

void CreatorState::onWindowResize()
{
    m_panel.setSize(sf::Vector2f(250, getWindowDimensions().y));
    m_panel.setPosition(getWindowDimensions().x - m_panel.getSize().x, 0);

    m_loadLevelLabel.setPosition(m_panel.getPosition() + sf::Vector2f(10, 10));
    m_loadLevelTextBox.setPosition(m_loadLevelLabel.getPosition() + sf::Vector2f(0, 30));

    m_saveLevelLabel.setPosition(m_loadLevelTextBox.getPosition() + sf::Vector2f(0, 40));
    m_saveLevelTextBox.setPosition(m_saveLevelLabel.getPosition() + sf::Vector2f(0, 30));

    m_widthTextBox.setPosition(m_saveLevelTextBox.getPosition() + sf::Vector2f(0, 40));
    m_heightTextBox.setPosition(m_widthTextBox.getPosition() + sf::Vector2f(m_widthTextBox.getDimensions().x + 10, 0));

    m_createLevelButton.setPosition(m_widthTextBox.getPosition() + sf::Vector2f(0, 40) + m_saveLevelTextBox.getDimensions() / 2.0f);

    m_tileNameLabel.setPosition(m_widthTextBox.getPosition() + sf::Vector2f(0, 80));
    m_tileNameTextBox.setPosition(m_tileNameLabel.getPosition() + sf::Vector2f(0, 30));

    m_level.onWindowResize();
}
