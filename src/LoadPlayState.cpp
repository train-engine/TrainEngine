#include "LoadPlayState.h"
#include <fstream>
#include <iostream>
#include "FileManager.h"
#include "PlayState.h"
#include "Utility.h"

LoadPlayState::LoadPlayState(GameEngine& rGame, const std::string& levelDirectory)
    : State(rGame),
      m_thread(&LoadPlayState::LoadResources, this),
      m_backgroundSprite(m_rGame.m_resourceManager.LoadTexture("loadScreen", "res/images/backgrounds/load_screen.png")),
      m_loadingText("Loading...", m_rGame.m_resourceManager.GetFont("mainFont"), 128),
      m_startSound(m_rGame.m_resourceManager.LoadSoundBuffer("loadSound", "res/sounds/load_sound.wav")),
      m_loadingBar(sf::Vector2f(0, 0), sf::Vector2f(750, 50),
                   sf::Color::White, sf::Color::Black, sf::Color::Black, -2, 0),
      m_progress(0),
      m_total(1),
      m_levelDirectory(levelDirectory),
      m_isLoaded(false)
{
    // Load resources on other thread
    m_thread.launch();

    // State settings
    m_stateSettings.isCloseable = false;
    
    // Content settings
    m_backgroundSprite.setOrigin(static_cast<sf::Vector2f>(m_backgroundSprite.getTexture()->getSize()) / 2.0f);
    m_backgroundSprite.setPosition(GetWindowDimensions() / 2.0f);
    Utility::SetSpriteScaleToFill(m_backgroundSprite, GetWindowDimensions());
    m_backgroundSprite.setColor(sf::Color(230, 230, 230));
    
    m_loadingText.setOrigin(m_loadingText.getLocalBounds().left + m_loadingText.getLocalBounds().width / 2, m_loadingText.getLocalBounds().top + m_loadingText.getLocalBounds().height / 2);
    m_loadingText.setPosition(GetAbsolutePosition(0.5, 1) + sf::Vector2f(0, - m_loadingText.getGlobalBounds().height / 2 - 15));
    m_loadingText.setFillColor(sf::Color::White);

    m_loadingBar.SetPosition(GetAbsolutePosition(0.5, 0) + sf::Vector2f(0, m_loadingText.getGlobalBounds().top - m_loadingBar.GetDimensions().y / 2 - 15));
    
    m_startSound.setVolume(25);
    m_startSound.play();
}

LoadPlayState::~LoadPlayState()
{
    // Unload resources
    m_rGame.m_resourceManager.UnloadTexture("loadScreen");
    m_rGame.m_resourceManager.UnloadSoundBuffer("loadSound");
}

void LoadPlayState::PlayStart()
{
    m_thread.wait();
    m_rGame.RequestSwap(new PlayState(m_rGame, m_levelDirectory));
}

// Load resources
void LoadPlayState::LoadResources()
{
    std::ifstream inf;
    inf.open(FileManager::ResourcePath() + m_levelDirectory + "/resources.txt");
    if (inf.is_open())
    {
        std::cout << "\nLoading resources...\n";
        
        std::string line;
        while (std::getline(inf, line))
        {
            m_total++;
        }
        inf.clear();
        inf.seekg(0, std::ios::beg);
        
        while (std::getline(inf, line))
        {
            if (line == "parallaxMountains")
            {
                m_rGame.m_resourceManager.LoadTexture("parallaxMountains1", "res/images/backgrounds/parallax_mountains/parallax_mountains1.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxMountains1", true);
                m_rGame.m_resourceManager.LoadTexture("parallaxMountains2", "res/images/backgrounds/parallax_mountains/parallax_mountains2.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxMountains2", true);
                m_rGame.m_resourceManager.LoadTexture("parallaxMountains3", "res/images/backgrounds/parallax_mountains/parallax_mountains3.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxMountains3", true);
                m_rGame.m_resourceManager.LoadTexture("parallaxMountains4", "res/images/backgrounds/parallax_mountains/parallax_mountains4.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxMountains4", true);
                m_rGame.m_resourceManager.LoadTexture("parallaxMountains5", "res/images/backgrounds/parallax_mountains/parallax_mountains5.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxMountains5", true);
            }
            else if (line == "parallaxUnderwater")
            {
                m_rGame.m_resourceManager.LoadTexture("parallaxUnderwater1", "res/images/backgrounds/parallax_underwater/parallax_underwater1.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxUnderwater1", true);
                m_rGame.m_resourceManager.LoadTexture("parallaxUnderwater2", "res/images/backgrounds/parallax_underwater/parallax_underwater2.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxUnderwater2", true);
                m_rGame.m_resourceManager.LoadTexture("parallaxUnderwater3", "res/images/backgrounds/parallax_underwater/parallax_underwater3.png");
                m_rGame.m_resourceManager.SetTextureRepeated("parallaxUnderwater3", true);
            }
            
            else if (line == "grassTopLeftSides")           m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 0, 64, 64));
            else if (line == "grassTopSide")                m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 0, 64, 64));
            else if (line == "grassTopRightSides")          m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 0, 64, 64));
            else if (line == "grassLeftSide")               m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 64, 64, 64));
            else if (line == "grassNoSides")                m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 64, 64, 64));
            else if (line == "grassRightSide")              m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 64, 64, 64));
            else if (line == "grassBotLeftSides")           m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 128, 64, 64));
            else if (line == "grassBotSide")                m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 128, 64, 64));
            else if (line == "grassBotRightSides")          m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 128, 64, 64));
            else if (line == "grassTopLeftRightSides")      m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 0, 64, 64));
            else if (line == "grassLeftRightSides")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 64, 64, 64));
            else if (line == "grassBotLeftRightSides")      m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 128, 64, 64));
            else if (line == "grassTopBotLeftSides")        m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(0, 192, 64, 64));
            else if (line == "grassTopBotSides")            m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(64, 192, 64, 64));
            else if (line == "grassTopBotRightSides")       m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(128, 192, 64, 64));
            else if (line == "grass4Sides")                 m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 192, 64, 64));
            else if (line == "grassTopLeftSidesCorner3")    m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 0, 64, 64));
            else if (line == "grassTopSideCorner3")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 0, 64, 64));
            else if (line == "grassTopSideCorner4")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 0, 64, 64));
            else if (line == "grassTopRightSidesCorner4")   m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 0, 64, 64));
            else if (line == "grassLeftSideCorner3")        m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 64, 64, 64));
            else if (line == "grassNoSidesCorner3")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 64, 64, 64));
            else if (line == "grassNoSidesCorner4")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 64, 64, 64));
            else if (line == "grassRightSideCorner4")       m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 64, 64, 64));
            else if (line == "grassLeftSideCorner2")        m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 128, 64, 64));
            else if (line == "grassNoSidesCorner2")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 128, 64, 64));
            else if (line == "grassNoSidesCorner1")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 128, 64, 64));
            else if (line == "grassRightSideCorner1")       m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 128, 64, 64));
            else if (line == "grassBotLeftSidesCorner2")    m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 192, 64, 64));
            else if (line == "grassBotSideCorner2")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 192, 64, 64));
            else if (line == "grassBotSideCorner1")         m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 192, 64, 64));
            else if (line == "grassBotRightSidesCorner1")   m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 192, 64, 64));
            else if (line == "grassNoSides4Corners")        m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(192, 256, 64, 64));
            else if (line == "grassNoSidesCorners12")       m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(256, 256, 64, 64));
            else if (line == "grassNoSidesCorners34")       m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(320, 256, 64, 64));
            else if (line == "grassNoSidesCorners14")       m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(384, 256, 64, 64));
            else if (line == "grassNoSidesCorners23")       m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/grass.png", sf::IntRect(448, 256, 64, 64));
            
            else if (line == "wood")                        m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/wood.png");
            
            else if (line == "ladder")                      m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/metal_ladder.png");
            else if (line == "vine")                        m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/vine.png");
            else if (line == "post")                        m_rGame.m_resourceManager.LoadTexture(line, "res/images/tiles/post.png");
            
            else if (line == "characterStill")              m_rGame.m_resourceManager.LoadTexture(line, "res/images/entities/player/player_standing.png");
            else if (line == "characterRunning")            m_rGame.m_resourceManager.LoadTexture(line, "res/images/entities/player/player_running.png");
            else if (line == "characterClimbing")           m_rGame.m_resourceManager.LoadTexture(line, "res/images/entities/player/player_climbing.png");
            else if (line == "characterJumping")            m_rGame.m_resourceManager.LoadTexture(line, "res/images/entities/player/player_jumping.png");
            else if (line == "characterFalling")            m_rGame.m_resourceManager.LoadTexture(line, "res/images/entities/player/player_falling.png");

            else
            {
                std::cerr << "Loading error: Unknown resource \"" << line << "\".\n";
            }

            m_progress++;
        }
        inf.close();
        std::cout << "Resources successfully loaded.\n\n";
    }
    else
    {
        std::cout << "Loading error: Unable to open \"" << m_levelDirectory << "/resources.txt" << "\".\n"
                     "Resources loading failed.\n\n";
    }
    
    m_isLoaded = true;
}

void LoadPlayState::HandleInput()
{

}

void LoadPlayState::Update()
{
    m_loadingBar.SetFraction(static_cast<double>(m_progress) / static_cast<double>(m_total));

    if (m_isLoaded == true)
    {
        PlayStart();
    }
}

void LoadPlayState::Draw(sf::RenderTarget& rTarget, float lag)
{
    rTarget.setView(GetDefaultView());
    
    rTarget.draw(m_backgroundSprite);
    rTarget.draw(m_loadingText);
    rTarget.draw(m_loadingBar);
}
