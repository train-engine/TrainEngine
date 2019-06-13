#include "States/PlayState.h"
#include <fstream>
#include <iostream>
#include "Core/FileManager.h"
#include "States/PauseState.h"

PlayState::PlayState(GameEngine& game, const std::string& levelDirectory)
    : State(game)
    , m_darkness(getWindowDimensions())
    , m_muteButton(m_game.resourceManager.getTexture("muteNormal"), m_game.resourceManager.getTexture("muteHovered"),
                   m_game.resourceManager.getTexture("muteClicked"), sf::Vector2f(getWindowDimensions().x - 48, 48), sf::Vector2f(64, 64))
    , m_level(m_game.resourceManager, m_game.inputManager)
{
    // Content settings
    m_stateSettings.backgroundColor = sf::Color(238, 241, 244);
    m_darkness.setFillColor(sf::Color(0, 0, 0, 20));

    // Music
    m_music.openFromFile(FileManager::resourcePath() + "res/music/theme_song_8_bit.wav");
    readMusicSettings();
    m_music.setLoop(true);
    m_music.play();

    m_level.load(levelDirectory);
}

PlayState::~PlayState()
{
}

void PlayState::readMusicSettings()
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

void PlayState::pauseStart()
{
    m_game.requestPush(new PauseState(m_game));
}

void PlayState::handleInput()
{
    m_level.setFocus(true); // Reset focus back to true to give back control to the level after actions with GUI

    if (m_game.inputManager.detectedLostFocusEvent() || m_game.inputManager.isKeyDescending(sf::Keyboard::Escape))
    {
        pauseStart();
        return;
    }

    if (m_game.inputManager.detectedMouseMovedEvent())
    {
        m_muteButton.onMouseHover(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonDescending(sf::Mouse::Left))
    {
        m_muteButton.onMouseClick(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonAscending(sf::Mouse::Left))
    {
        if (m_muteButton.onMouseUnclick(getWindowMousePosition()) == true)
        {
            m_level.setFocus(false);
            if (m_music.getStatus() == sf::SoundSource::Status::Playing)
            {
                m_music.pause();
            }
            else if (m_music.getStatus() == sf::SoundSource::Status::Paused || m_music.getStatus() == sf::SoundSource::Status::Stopped)
            {
                m_music.play();
            }
        }
    }

    m_level.handleInput();
}

void PlayState::update()
{
    m_level.update();
}

void PlayState::draw(sf::RenderTarget& target, float lag)
{
    drawBackgroundColor(target);

    m_level.draw(target, sf::RenderStates::Default, lag);

    target.draw(m_darkness);
    target.draw(m_muteButton);
}

void PlayState::pause()
{
    m_music.pause();
}

void PlayState::resume()
{
    m_music.play();
}

void PlayState::onWindowResize()
{
    m_darkness.setPosition(0, 0);
    m_darkness.setSize(getWindowDimensions());
    m_muteButton.setPosition(sf::Vector2f(getWindowDimensions().x - 48, 48));

    m_level.onWindowResize();
}
