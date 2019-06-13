#include "States/MainMenuState.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include "Core/FileManager.h"
#include "Misc/Utility.h"
#include "States/CreatorState.h"
#include "States/LoadPlayState.h"
#include "States/MenuOptionsState.h"

MainMenuState::MainMenuState(GameEngine& game)
    : State(game)
    , m_backgroundSprite(m_game.resourceManager.getTexture("menuBackground"))
    , m_gameNameText("TrainEngine", m_game.resourceManager.getFont("mainFont"), 64)
    , m_creditsText("Made by Misha Krieger-Raynauld, Simon Gauvin, Guillaume Jones, and Ba Minh Nguyen.",
                    m_game.resourceManager.getFont("altFont"), 16)
    , m_muteButton(m_game.resourceManager.getTexture("muteNormal"), m_game.resourceManager.getTexture("muteHovered"),
                   m_game.resourceManager.getTexture("muteClicked"), sf::Vector2f(getWindowDimensions().x - 48, 48), sf::Vector2f(64, 64))
    , m_elapsedTicks(0)
{
    // State settings
    m_stateSettings.canSkipUpdates = true;

    // Initialize GUI
    const sf::Font& font = m_game.resourceManager.getFont("mainFont");
    const sf::SoundBuffer& soundBuffer = m_game.resourceManager.getSoundBuffer("click");
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Play Level 1", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Play Level 2", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Play Level 3", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Level Creator", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Options", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(300, 50), -2, 6, "Quit", GuiStyle::White);
    for (auto& button : m_buttons)
    {
        button.setVolume(0.75);
    }

    // Content settings
    m_backgroundSprite.setOrigin(static_cast<sf::Vector2f>(m_backgroundSprite.getTexture()->getSize()) / 2.0f);
    m_gameNameText.setOrigin(m_gameNameText.getLocalBounds().left + m_gameNameText.getLocalBounds().width / 2,
                             m_gameNameText.getLocalBounds().top + m_gameNameText.getLocalBounds().height / 2);
    m_gameNameText.setFillColor(sf::Color(5, 25, 100));

    // Music settings
    m_music.openFromFile(FileManager::resourcePath() + "res/music/stargazer.ogg");
    readMusicSettings();
    m_music.setLoop(true);
    m_music.play();
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::loadPlayStart(const std::string& levelName)
{
    m_music.stop();
    m_game.requestPush(new LoadPlayState(m_game, levelName));
}

void MainMenuState::creatorStart()
{
    m_music.stop();
    m_game.requestPush(new CreatorState(m_game));
}

void MainMenuState::readMusicSettings()
{
    std::ifstream inputFile(FileManager::resourcePath() + "data/settings/sound_settings.txt");
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

void MainMenuState::handleInput()
{
    if (m_game.inputManager.isKeyDescending(sf::Keyboard::Escape))
    {
        m_game.requestPop();
        return;
    }
    if (m_game.inputManager.isKeyDescending(sf::Keyboard::P))
    {
        // Start game
        loadPlayStart("data/levels/level2");
        return;
    }
    if (m_game.inputManager.isKeyDescending(sf::Keyboard::C))
    {
        // Start Level creator
        creatorStart();
        return;
    }

    if (m_game.inputManager.detectedMouseMovedEvent() || m_game.inputManager.detectedTouchMovedEvent())
    {
        for (auto& button : m_buttons)
        {
            button.onMouseHover(getWindowMousePosition());
        }
        m_muteButton.onMouseHover(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonDescending(sf::Mouse::Left) || // Mouse click
        m_game.inputManager.detectedTouchBeganEvent() || m_game.inputManager.detectedTouchMovedEvent()) // Touch
    {
        for (auto& button : m_buttons)
        {
            button.onMouseClick(getWindowMousePosition());
        }
        m_muteButton.onMouseClick(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonAscending(sf::Mouse::Left) || m_game.inputManager.detectedTouchEndedEvent())
    {
        for (std::size_t i = 0; i < m_buttons.size(); i++)
        {
            if (m_buttons[i].onMouseUnclick(getWindowMousePosition()) == true)
            {
                switch (i)
                {
                // Play button
                case 0:
                    loadPlayStart("data/levels/level1");
                    return;
                case 1:
                    loadPlayStart("data/levels/level2");
                    return;
                case 2:
                    loadPlayStart("data/levels/level3");
                    return;
                // Level creator button
                case 3:
                    creatorStart();
                    return;
                // Menu options button
                case 4:
                    m_game.requestPush(new MenuOptionsState(m_game));
                    return;
                // Quit button
                case 5:
                    m_game.requestPop();
                    break;
                default:
                    break;
                }
                break; // Break because if one button has been clicked, the others cannot have also been clicked (optimization)
            }
        }
        if (m_muteButton.onMouseUnclick(getWindowMousePosition()) == true)
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

void MainMenuState::update()
{
    m_gameNameText.setRotation(360 * std::sin(m_elapsedTicks++ / 125.0));
}

void MainMenuState::draw(sf::RenderTarget& target, float lag)
{
    target.draw(m_backgroundSprite);
    target.draw(m_gameNameText);
    target.draw(m_creditsText);

    for (const auto& button : m_buttons)
    {
        target.draw(button);
    }
    target.draw(m_muteButton);
}

void MainMenuState::resume()
{
    readMusicSettings();
    if (m_music.getStatus() != sf::SoundSource::Status::Playing)
    {
        m_music.play();
    }
}

void MainMenuState::onWindowResize()
{
    m_backgroundSprite.setPosition(getWindowDimensions() / 2.0f);
    Utility::setSpriteScaleToFill(m_backgroundSprite, getWindowDimensions());
    m_gameNameText.setPosition(getAbsolutePosition(0.5, 0.2));
    m_creditsText.setPosition(getWindowDimensions().x - (m_creditsText.getGlobalBounds().width) - 4,
                              getWindowDimensions().y - (m_creditsText.getFont()->getLineSpacing(m_creditsText.getCharacterSize())));
    m_muteButton.setPosition(sf::Vector2f(getWindowDimensions().x - 48, 48));
    for (std::size_t i = 0; i < m_buttons.size(); i++)
    {
        m_buttons[i].setPosition(getAbsolutePosition(0.25, 0.45) + sf::Vector2f(0, -90 + 60 * static_cast<int>(i)));
    }
}
