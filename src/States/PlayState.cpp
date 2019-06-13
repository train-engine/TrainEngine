#include "States/PlayState.h"
#include <fstream>
#include <iostream>
#include "Core/FileManager.h"
#include "States/PauseState.h"

PlayState::PlayState(GameEngine& rGame, const std::string& levelDirectory)
    : State(rGame)
    , m_darkness(getWindowDimensions())
    , m_muteButton(m_rGame.resourceManager.getTexture("muteNormal"), m_rGame.resourceManager.getTexture("muteHovered"),
                   m_rGame.resourceManager.getTexture("muteClicked"), sf::Vector2f(getWindowDimensions().x - 48, 48), sf::Vector2f(64, 64))
    , m_level(m_rGame.resourceManager, m_rGame.inputManager)
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
    m_rGame.requestPush(new PauseState(m_rGame));
}

void PlayState::handleInput()
{
    m_level.setFocus(true); // Reset focus back to true to give back control to the level after actions with GUI

    if (m_rGame.inputManager.detectedLostFocusEvent() || m_rGame.inputManager.isKeyDescending(sf::Keyboard::Escape))
    {
        pauseStart();
        return;
    }

    if (m_rGame.inputManager.detectedMouseMovedEvent())
    {
        m_muteButton.onMouseHover(getWindowMousePosition());
    }
    if (m_rGame.inputManager.isMouseButtonDescending(sf::Mouse::Left))
    {
        m_muteButton.onMouseClick(getWindowMousePosition());
    }
    if (m_rGame.inputManager.isMouseButtonAscending(sf::Mouse::Left))
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

void PlayState::draw(sf::RenderTarget& rTarget, float lag)
{
    drawBackgroundColor(rTarget);

    m_level.draw(rTarget, sf::RenderStates::Default, lag);

    rTarget.draw(m_darkness);
    rTarget.draw(m_muteButton);
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
