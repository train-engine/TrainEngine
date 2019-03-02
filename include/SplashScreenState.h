#ifndef SPLASHSCREENSTATE_H
#define SPLASHSCREENSTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "State.h"

class SplashScreenState final : public State
{
private:   
    sf::Sprite m_splash;
    sf::Sprite m_mask;
    sf::Sound m_sound;

    int m_alpha;

    // Destructor
    virtual ~SplashScreenState() override;

    // Functions
    void MainMenuStart();
    void LoadResources();

    // State functions
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void OnWindowResize() override;

public:
    // Constructor
    SplashScreenState(GameEngine& rGame);
};

#endif // SPLASHSCREENSTATE_H
