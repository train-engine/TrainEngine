#include "States/SplashScreenState.h"
#include "Misc/Utility.h"
#include "States/MainMenuState.h"

SplashScreenState::SplashScreenState(GameEngine& rGame)
    : State(rGame)
    , m_splash(m_rGame.resourceManager.loadTexture("splash", "res/images/backgrounds/engine_splash.png"))
    , m_mask(m_rGame.resourceManager.loadTexture("mask", "res/images/backgrounds/mask.png"))
    , m_sound(m_rGame.resourceManager.loadSoundBuffer("splashScreenSound", "res/sounds/splash_screen_sound.wav"))
    , m_alpha(255)
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
    m_rGame.resourceManager.unloadTexture("splash");
    m_rGame.resourceManager.unloadTexture("mask");
    m_rGame.resourceManager.unloadSoundBuffer("splashScreenSound");
}

void SplashScreenState::mainMenuStart()
{
    m_rGame.requestSwap(new MainMenuState(m_rGame));
}

void SplashScreenState::handleInput()
{
    if (m_rGame.inputManager.detectedKeyPressedEvent() || m_rGame.inputManager.detectedMouseButtonReleasedEvent())
    {
        // Go to main menu
        mainMenuStart();
    }
}

void SplashScreenState::update()
{
    if (m_alpha > 0)
    {
        m_alpha -= 2;
        m_splash.setColor(sf::Color(255, 255, 255, m_alpha));
        m_mask.setColor(sf::Color(255, 255, 255, m_alpha));
    }
    else
    {
        mainMenuStart();
        return;
    }
}

void SplashScreenState::draw(sf::RenderTarget& rTarget, float lag)
{
    rTarget.draw(m_mask);
    rTarget.draw(m_splash);
}

void SplashScreenState::onWindowResize()
{
    m_splash.setPosition(getAbsolutePosition(0.5, 0.5));

    m_mask.setPosition(getAbsolutePosition(0.5, 0.5));
    Utility::setSpriteScaleToFit(m_mask, getWindowDimensions());
}
