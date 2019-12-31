#include "States/LoadPlayState.h"
#include <fstream>
#include <iostream>
#include "Core/FileManager.h"
#include "Misc/Utility.h"
#include "States/PlayState.h"

LoadPlayState::LoadPlayState(GameEngine& game, const std::string& levelDirectory)
    : State(game)
    , m_thread(&LoadPlayState::loadResources, this)
    , m_backgroundSprite(m_game.resourceManager.loadTexture("loadScreen", "res/images/backgrounds/load_screen.png"))
    , m_loadingText("Loading...", m_game.resourceManager.getFont("mainFont"), 128)
    , m_startSound(m_game.resourceManager.loadSoundBuffer("loadSound", "res/sounds/load_sound.wav"))
    , m_loadingBar(sf::Vector2f(0, 0), sf::Vector2f(750, 50), sf::Color::White, sf::Color::Black, sf::Color::Black, -2, 0)
    , m_progress(0)
    , m_total(1)
    , m_levelDirectory(levelDirectory)
    , m_isLoaded(false)
{
    // Load resources on other thread
    m_thread.launch();

    // State settings
    m_stateSettings.isCloseable = false;

    // Content settings
    m_backgroundSprite.setOrigin(static_cast<sf::Vector2f>(m_backgroundSprite.getTexture()->getSize()) / 2.0f);
    m_backgroundSprite.setColor(sf::Color(230, 230, 230));

    m_loadingText.setOrigin(m_loadingText.getLocalBounds().left + m_loadingText.getLocalBounds().width / 2,
                            m_loadingText.getLocalBounds().top + m_loadingText.getLocalBounds().height / 2);
    m_loadingText.setFillColor(sf::Color::White);

    // Music settings
    m_startSound.setVolume(25);
    m_startSound.play();
}
#include "States/MenuOptionsState.h"
LoadPlayState::~LoadPlayState()
{
    // Unload resources
    m_game.resourceManager.unloadTexture("loadScreen");
    m_game.resourceManager.unloadSoundBuffer("loadSound");
}

void LoadPlayState::playStart()
{
    m_thread.wait();
    m_game.requestSwap(new PlayState(m_game, m_levelDirectory));
}

// Load resources
void LoadPlayState::loadResources()
{
    std::ifstream inputFile(FileManager::resourcePath() + m_levelDirectory + "/resources.txt");
    if (inputFile)
    {
        // Unused, but its existence is necessary to make OpenGL calls without an active window in the current thread
        sf::Context context;

        std::cout << "\nLoading resources...\n";

        std::string line;
        while (std::getline(inputFile, line))
        {
            m_total++;
        }
        inputFile.clear();
        inputFile.seekg(0, std::ios::beg);

        while (std::getline(inputFile, line))
        {
            if (line == "grassTopLeftSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 0, 64, 64));
            else if (line == "grassTopSide")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 0, 64, 64));
            else if (line == "grassTopRightSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 0, 64, 64));
            else if (line == "grassLeftSide")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 64, 64, 64));
            else if (line == "grassNoSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 64, 64, 64));
            else if (line == "grassRightSide")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 64, 64, 64));
            else if (line == "grassBotLeftSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 128, 64, 64));
            else if (line == "grassBotSide")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 128, 64, 64));
            else if (line == "grassBotRightSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 128, 64, 64));
            else if (line == "grassTopLeftRightSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 0, 64, 64));
            else if (line == "grassLeftRightSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 64, 64, 64));
            else if (line == "grassBotLeftRightSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 128, 64, 64));
            else if (line == "grassTopBotLeftSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 192, 64, 64));
            else if (line == "grassTopBotSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 192, 64, 64));
            else if (line == "grassTopBotRightSides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 192, 64, 64));
            else if (line == "grass4Sides")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 192, 64, 64));
            else if (line == "grassTopLeftSidesCorner3")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 0, 64, 64));
            else if (line == "grassTopSideCorner3")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 0, 64, 64));
            else if (line == "grassTopSideCorner4")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 0, 64, 64));
            else if (line == "grassTopRightSidesCorner4")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 0, 64, 64));
            else if (line == "grassLeftSideCorner3")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 64, 64, 64));
            else if (line == "grassNoSidesCorner3")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 64, 64, 64));
            else if (line == "grassNoSidesCorner4")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 64, 64, 64));
            else if (line == "grassRightSideCorner4")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 64, 64, 64));
            else if (line == "grassLeftSideCorner2")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 128, 64, 64));
            else if (line == "grassNoSidesCorner2")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 128, 64, 64));
            else if (line == "grassNoSidesCorner1")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 128, 64, 64));
            else if (line == "grassRightSideCorner1")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 128, 64, 64));
            else if (line == "grassBotLeftSidesCorner2")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 192, 64, 64));
            else if (line == "grassBotSideCorner2")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 192, 64, 64));
            else if (line == "grassBotSideCorner1")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 192, 64, 64));
            else if (line == "grassBotRightSidesCorner1")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 192, 64, 64));
            else if (line == "grassNoSides4Corners")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 256, 64, 64));
            else if (line == "grassNoSidesCorners12")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 256, 64, 64));
            else if (line == "grassNoSidesCorners34")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 256, 64, 64));
            else if (line == "grassNoSidesCorners14")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 256, 64, 64));
            else if (line == "grassNoSidesCorners23")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 256, 64, 64));

            else if (line == "wood")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/wood.png");

            else if (line == "ladder")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/metal_ladder.png");
            else if (line == "vine")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/vine.png");
            else if (line == "post")
                m_game.resourceManager.loadTexture(line, "res/images/tiles/post.png");

            else if (line == "parallaxMountains1")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_mountains/parallax_mountains1.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }
            else if (line == "parallaxMountains2")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_mountains/parallax_mountains2.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }
            else if (line == "parallaxMountains3")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_mountains/parallax_mountains3.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }
            else if (line == "parallaxMountains4")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_mountains/parallax_mountains4.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }
            else if (line == "parallaxMountains5")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_mountains/parallax_mountains5.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }
            else if (line == "parallaxUnderwater1")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_underwater/parallax_underwater1.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }
            else if (line == "parallaxUnderwater2")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_underwater/parallax_underwater2.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }
            else if (line == "parallaxUnderwater3")
            {
                m_game.resourceManager.loadTexture(line, "res/images/backgrounds/parallax_underwater/parallax_underwater3.png");
                m_game.resourceManager.setTextureRepeated(line, true);
            }

            else if (line == "characterStill")
                m_game.resourceManager.loadTexture(line, "res/images/entities/player/player_standing.png");
            else if (line == "characterRunning")
                m_game.resourceManager.loadTexture(line, "res/images/entities/player/player_running.png");
            else if (line == "characterClimbing")
                m_game.resourceManager.loadTexture(line, "res/images/entities/player/player_climbing.png");
            else if (line == "characterJumping")
                m_game.resourceManager.loadTexture(line, "res/images/entities/player/player_jumping.png");
            else if (line == "characterFalling")
                m_game.resourceManager.loadTexture(line, "res/images/entities/player/player_falling.png");

            else
            {
                std::cerr << "Loading error: Unknown resource \"" << line << "\".\n";
            }

            m_progress++;
        }

        std::cout << "Resources successfully loaded.\n\n";
    }
    else
    {
        std::cerr << "Loading error: Unable to open \"" << m_levelDirectory << "/resources.txt\".\n"
                  << "Resources loading failed.\n\n";
    }

    m_isLoaded = true;
}

void LoadPlayState::handleInput()
{
}

void LoadPlayState::update()
{
    m_loadingBar.setFraction(static_cast<double>(m_progress) / static_cast<double>(m_total));

    if (m_isLoaded == true)
    {
        playStart();
    }
}

void LoadPlayState::draw(sf::RenderTarget& target, float lag)
{
    target.draw(m_backgroundSprite);
    target.draw(m_loadingText);
    target.draw(m_loadingBar);
}

void LoadPlayState::onWindowResize()
{
    m_backgroundSprite.setPosition(getWindowDimensions() / 2.0f);
    Utility::setSpriteScaleToFill(m_backgroundSprite, getWindowDimensions());

    m_loadingText.setPosition(getAbsolutePosition(0.5, 1) + sf::Vector2f(0, -m_loadingText.getGlobalBounds().height / 2 - 15));

    m_loadingBar.setPosition(getAbsolutePosition(0.5, 0) +
                             sf::Vector2f(0, m_loadingText.getGlobalBounds().top - m_loadingBar.getDimensions().y / 2 - 15));
}
