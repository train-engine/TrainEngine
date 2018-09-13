#include "SplashScreenState.h"
#include "MainMenuState.h"
#include "Utility.h"

SplashScreenState::SplashScreenState(GameEngine& rGame)
    : State(rGame),
      m_splash(m_rGame.m_resourceManager.LoadTexture("splash", "res/images/backgrounds/engine_splash.png")),
      m_mask(m_rGame.m_resourceManager.LoadTexture("mask", "res/images/backgrounds/mask.png")),
      m_sound(m_rGame.m_resourceManager.LoadSoundBuffer("splashScreenSound", "res/sounds/splash_screen_sound.wav")),
      m_alpha(255)
{
    // Content settings
    m_splash.setOrigin(static_cast<sf::Vector2f>(m_splash.getTexture()->getSize()) / 2.0f);
    m_mask.setOrigin(static_cast<sf::Vector2f>(m_mask.getTexture()->getSize()) / 2.0f);

    // Music settings
    m_sound.setVolume(10);
    m_sound.play();
}

SplashScreenState::~SplashScreenState()
{
    // Unload resources
    m_rGame.m_resourceManager.UnloadTexture("splash");
    m_rGame.m_resourceManager.UnloadTexture("mask");
    m_rGame.m_resourceManager.UnloadSoundBuffer("splashScreenSound");
}

void SplashScreenState::MainMenuStart()
{
    m_rGame.RequestSwap(new MainMenuState(m_rGame));
}

void SplashScreenState::HandleInput()
{ 
    if (m_rGame.m_inputManager.DetectKeyDownEvent() || m_rGame.m_inputManager.DetectMouseButtonUpEvent())
    {
        // Go to main menu
        MainMenuStart();
    }
}

void SplashScreenState::Update()
{
    if (m_alpha > 0)
    {
        m_alpha -= 2;
        m_splash.setColor(sf::Color(255, 255, 255, m_alpha));
        m_mask.setColor(sf::Color(255, 255, 255, m_alpha));
    }
    else
    {
        MainMenuStart();
        return;
    }
}

void SplashScreenState::Draw(sf::RenderTarget& rTarget, float lag)
{
    rTarget.setView(GetDefaultView());

    rTarget.draw(m_mask);
    rTarget.draw(m_splash);
}

void SplashScreenState::OnWindowResize()
{
    m_splash.setPosition(GetAbsolutePosition(0.5, 0.5));

    m_mask.setPosition(GetAbsolutePosition(0.5, 0.5));
    Utility::SetSpriteScaleToFit(m_mask, GetWindowDimensions());
}
