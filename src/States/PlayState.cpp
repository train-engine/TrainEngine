#include "PlayState.h"
#include <fstream>
#include <iostream>
#include "FileManager.h"
#include "PauseState.h"

#include <iostream>

PlayState::PlayState(GameEngine& rGame, const std::string& levelDirectory)
    : State(rGame)
    , m_darkness(GetWindowDimensions())
    , m_muteButton(m_rGame.resourceManager.GetTexture("muteNormal"), m_rGame.resourceManager.GetTexture("muteHovered"),
                   m_rGame.resourceManager.GetTexture("muteClicked"), sf::Vector2f(GetWindowDimensions().x - 48, 48), sf::Vector2f(64, 64))
    , m_level(m_rGame.resourceManager, m_rGame.inputManager)
    , m_inputContext(rGame.inputManager)
{
    // Content settings
    m_stateSettings.backgroundColor = sf::Color(238, 241, 244);
    m_darkness.setFillColor(sf::Color(0, 0, 0, 20));

    // Music
    m_music.openFromFile(FileManager::ResourcePath() + "res/music/theme_song_8_bit.wav");
    ReadMusicSettings();
    m_music.setLoop(true);
    m_music.play();

    m_level.Load(levelDirectory);

    m_inputContext.BindActionToKey(this, &PlayState::PauseStart, sf::Keyboard::Escape, EventType::Descending);
}

PlayState::~PlayState()
{
}

void PlayState::ReadMusicSettings()
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

void PlayState::PauseStart()
{
    m_music.setVolume(m_music.getVolume() * 0.20);
    m_rGame.RequestPush(new PauseState(m_rGame));
}

void PlayState::HandleInput()
{
    m_inputContext.Update();
    m_level.SetFocus(true); // Reset focus back to true to give back control to the level after actions with GUI

    if (m_rGame.inputManager.DetectedLostFocusEvent())
    {
        PauseStart();
        return;
    }

    if (m_rGame.inputManager.DetectedMouseMovedEvent())
    {
        m_muteButton.OnMouseHover(GetWindowMousePosition());
    }
    if (m_rGame.inputManager.IsMouseButtonDescending(sf::Mouse::Left))
    {
        m_muteButton.OnMouseClick(GetWindowMousePosition());
    }
    if (m_rGame.inputManager.IsMouseButtonAscending(sf::Mouse::Left))
    {
        if (m_muteButton.OnMouseUnclick(GetWindowMousePosition()) == true)
        {
            m_level.SetFocus(false);
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

    m_level.HandleInput();
}

void PlayState::Update()
{
    m_level.Update();
}

void PlayState::Draw(sf::RenderTarget& rTarget, float lag)
{
    DrawBackgroundColor(rTarget);

    m_level.Draw(rTarget, sf::RenderStates::Default, lag);

    rTarget.draw(m_darkness);
    rTarget.draw(m_muteButton);
}

void PlayState::Pause()
{
    m_music.stop();
}

void PlayState::Resume()
{
    ReadMusicSettings();
}

void PlayState::OnWindowResize()
{
    m_darkness.setPosition(0, 0);
    m_darkness.setSize(GetWindowDimensions());
    m_muteButton.SetPosition(sf::Vector2f(GetWindowDimensions().x - 48, 48));

    m_level.OnWindowResize();
}
void PlayState::test(bool test)
{
    std::cout << "test: " << test << std::endl;
}