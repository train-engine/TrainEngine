#include "PlayState.h"
#include <fstream>
#include <iostream>
#include "PauseState.h"
#include "ResourcePath.h"

PlayState::PlayState(GameEngine& rGame, const std::string& levelDirectory)
    : State(rGame),
      m_darkness(GetWindowDimensions()),
      m_muteButton(m_rGame.m_resourceManager.GetTexture("muteNormal"),
                   m_rGame.m_resourceManager.GetTexture("muteHovered"),
                   m_rGame.m_resourceManager.GetTexture("muteClicked"),
                   sf::Vector2f(GetWindowDimensions().x - 48, 48), sf::Vector2f(64, 64)),
      m_level(m_rGame.m_resourceManager, m_rGame.m_inputManager)
{
    // Content settings
    SetBackgroundColor(sf::Color(238, 241, 244));
    m_darkness.setFillColor(sf::Color(0, 0, 0, 20));

    // Music
    m_music.openFromFile(ResourcePath() + "res/music/theme_song_8_bit.wav");
    ReadMusicSettings();
    m_music.setLoop(true);
    m_music.play();

    m_level.Load(levelDirectory);
}

PlayState::~PlayState()
{
    UnloadResources();
}

void PlayState::ReadMusicSettings()
{
    std::ifstream inf;
    inf.open(ResourcePath() + "data/settings/sound_settings.txt");
    if (inf.is_open())
    {
        bool isMuted = 0;
        inf >> isMuted;
        unsigned int volume = 50;
        inf >> volume;
        m_music.setVolume(volume);
        inf.close();
        std::cout << "Successfully read sound settings.\n";
        return;
    }
    std::cout << "Failed to read sound settings.\n";
}

void PlayState::UnloadResources() const
{
    m_rGame.m_resourceManager.UnloadTexture("parallaxMountains1");
    m_rGame.m_resourceManager.UnloadTexture("parallaxMountains2");
    m_rGame.m_resourceManager.UnloadTexture("parallaxMountains3");
    m_rGame.m_resourceManager.UnloadTexture("parallaxMountains4");
    m_rGame.m_resourceManager.UnloadTexture("parallaxMountains5");
}

void PlayState::PauseStart()
{
    m_music.setVolume(m_music.getVolume() * 0.20);
    m_rGame.RequestPush(new PauseState(m_rGame));
}

void PlayState::HandleInput()
{
    if (m_rGame.m_inputManager.DetectLostFocusEvent() || m_rGame.m_inputManager.KeyDown(sf::Keyboard::Escape))
    {
        PauseStart();
        return;
    }

    if (m_rGame.m_inputManager.DetectMouseMovedEvent())
    {
        m_muteButton.OnMouseHover(GetWindowMousePosition());
    }
    if (m_rGame.m_inputManager.MouseButtonDown(sf::Mouse::Left))
    {
        m_muteButton.OnMouseClick(GetWindowMousePosition());
    }
    if (m_rGame.m_inputManager.MouseButtonUp(sf::Mouse::Left))
    {
        if (m_muteButton.OnMouseUnclick(GetWindowMousePosition()) == true)
        {
            m_level.SetHasFocus(false);
            if (m_music.getStatus() == sf::SoundSource::Status::Playing)
            {
                m_music.pause();
            }
            else if (m_music.getStatus() == sf::SoundSource::Status::Paused || m_music.getStatus() == sf::SoundSource::Status::Stopped)
            {
                m_music.play();
            }
        }
        else
        {
            m_level.SetHasFocus(true);
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
    rTarget.setView(GetDefaultView());
    DrawBackgroundColor(rTarget, sf::RenderStates());
    
    m_level.Draw(rTarget, sf::RenderStates(), lag);

    rTarget.setView(GetDefaultView());
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
