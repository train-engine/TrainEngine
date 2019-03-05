#include "MainMenuState.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include "CreatorState.h"
#include "FileManager.h"
#include "LoadPlayState.h"
#include "MenuOptionsState.h"
#include "Utility.h"

MainMenuState::MainMenuState(GameEngine& rGame)
    : State(rGame)
    , m_backgroundSprite(m_rGame.resourceManager.GetTexture("menuBackground"))
    , m_gameNameText("TrainEngine", m_rGame.resourceManager.GetFont("mainFont"), 64)
    , m_creditsText("Made by Misha Krieger-Raynauld, Simon Gauvin, Guillaume Jones, and Ba Minh Nguyen.",
                    m_rGame.resourceManager.GetFont("altFont"), 16)
    , m_muteButton(m_rGame.resourceManager.GetTexture("muteNormal"), m_rGame.resourceManager.GetTexture("muteHovered"),
                   m_rGame.resourceManager.GetTexture("muteClicked"), sf::Vector2f(GetWindowDimensions().x - 48, 48), sf::Vector2f(64, 64))
    , m_elapsedTicks(0)
{
    // State settings
    m_stateSettings.canSkipUpdates = true;

    // Initialize GUI
    const sf::Font& font = m_rGame.resourceManager.GetFont("mainFont");
    const sf::SoundBuffer& soundBuffer = m_rGame.resourceManager.GetSoundBuffer("click");
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Play Level 1", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Play Level 2", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Play Level 3", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Level Creator", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Options", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Quit", GuiStyle::White);
    for (auto& button : m_buttons)
    {
        button.SetVolume(0.75);
    }

    // Content settings
    m_backgroundSprite.setOrigin(static_cast<sf::Vector2f>(m_backgroundSprite.getTexture()->getSize()) / 2.0f);
    m_gameNameText.setOrigin(m_gameNameText.getLocalBounds().left + m_gameNameText.getLocalBounds().width / 2,
                             m_gameNameText.getLocalBounds().top + m_gameNameText.getLocalBounds().height / 2);
    m_gameNameText.setFillColor(sf::Color(5, 25, 100));

    // Music settings
    m_music.openFromFile(FileManager::ResourcePath() + "res/music/stargazer.ogg");
    ReadMusicSettings();
    m_music.setLoop(true);
    m_music.play();
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::LoadPlayStart(const std::string& levelName)
{
    m_music.stop();
    m_rGame.RequestPush(new LoadPlayState(m_rGame, levelName));
}

void MainMenuState::CreatorStart()
{
    m_music.stop();
    m_rGame.RequestPush(new CreatorState(m_rGame));
}

void MainMenuState::ReadMusicSettings()
{
    std::ifstream inputFile(FileManager::ResourcePath() + "data/settings/sound_settings.txt");
    if (inputFile)
    {
        bool isMuted = false;
        inputFile >> isMuted;
        unsigned int volume = 50;
        inputFile >> volume;
        m_music.setVolume(volume);
        std::cout << "Successfully read sound settings.\n";
        return;
    }
    std::cout << "Failed to read sound settings.\n";
}

void MainMenuState::HandleInput()
{
    if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::Escape))
    {
        m_rGame.RequestPop();
        return;
    }
    if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::P))
    {
        // Start game
        LoadPlayStart("data/levels/level2");
        return;
    }
    if (m_rGame.inputManager.IsKeyDescending(sf::Keyboard::C))
    {
        // Start Level creator
        CreatorStart();
        return;
    }

    if (m_rGame.inputManager.DetectedMouseMovedEvent() || m_rGame.inputManager.DetectedTouchMovedEvent())
    {
        for (auto& rButton : m_buttons)
        {
            rButton.OnMouseHover(GetWindowMousePosition());
        }
        m_muteButton.OnMouseHover(GetWindowMousePosition());
    }
    if (m_rGame.inputManager.IsMouseButtonDescending(sf::Mouse::Left) || // Mouse click
        m_rGame.inputManager.DetectedTouchBeganEvent() || m_rGame.inputManager.DetectedTouchMovedEvent()) // Touch
    {
        for (auto& rButton : m_buttons)
        {
            rButton.OnMouseClick(GetWindowMousePosition());
        }
        m_muteButton.OnMouseClick(GetWindowMousePosition());
    }
    if (m_rGame.inputManager.IsMouseButtonAscending(sf::Mouse::Left) || m_rGame.inputManager.DetectedTouchEndedEvent())
    {
        for (unsigned int i = 0; i < m_buttons.size(); i++)
        {
            if (m_buttons[i].OnMouseUnclick(GetWindowMousePosition()) == true)
            {
                switch (i)
                {
                // Play button
                case 0:
                    LoadPlayStart("data/levels/level1");
                    return;
                case 1:
                    LoadPlayStart("data/levels/level2");
                    return;
                case 2:
                    LoadPlayStart("data/levels/level3");
                    return;
                // Level creator button
                case 3:
                    CreatorStart();
                    return;
                // Menu options button
                case 4:
                    m_rGame.RequestPush(new MenuOptionsState(m_rGame));
                    return;
                // Quit button
                case 5:
                    m_rGame.RequestPop();
                    break;
                default:
                    break;
                }
                break; // Break because if one button has been clicked, the others cannot have also been clicked (optimization)
            }
        }
        if (m_muteButton.OnMouseUnclick(GetWindowMousePosition()) == true)
        {
            if (m_music.getStatus() == sf::SoundSource::Status::Playing)
            {
                m_music.pause();
            }
            else
            {
                m_music.play();
            }
        }
    }
}

void MainMenuState::Update()
{
    m_gameNameText.setRotation(360 * std::sin(m_elapsedTicks++ / 125.0));
}

void MainMenuState::Draw(sf::RenderTarget& rTarget, float lag)
{
    rTarget.draw(m_backgroundSprite);
    rTarget.draw(m_gameNameText);
    rTarget.draw(m_creditsText);

    for (const auto& button : m_buttons)
    {
        rTarget.draw(button);
    }
    rTarget.draw(m_muteButton);
}

void MainMenuState::Resume()
{
    ReadMusicSettings();
    if (m_music.getStatus() != sf::SoundSource::Status::Playing)
    {
        m_music.play();
    }
}

void MainMenuState::OnWindowResize()
{
    m_backgroundSprite.setPosition(GetWindowDimensions() / 2.0f);
    Utility::SetSpriteScaleToFill(m_backgroundSprite, GetWindowDimensions());
    m_gameNameText.setPosition(GetAbsolutePosition(0.5, 0.2));
    m_creditsText.setPosition(GetWindowDimensions().x - (m_creditsText.getGlobalBounds().width) - 4,
                              GetWindowDimensions().y - (m_creditsText.getFont()->getLineSpacing(m_creditsText.getCharacterSize())));
    m_muteButton.SetPosition(sf::Vector2f(GetWindowDimensions().x - 48, 48));
    for (unsigned int i = 0; i < m_buttons.size(); i++)
    {
        m_buttons[i].SetPosition(GetAbsolutePosition(0.25, 0.45) + sf::Vector2f(0, -90 + 60 * static_cast<int>(i)));
    }
}
