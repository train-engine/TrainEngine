#ifndef SPLASHSCREENSTATE_H_INCLUDED
#define SPLASHSCREENSTATE_H_INCLUDED

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "State.h"

class SplashScreenState : public State
{
private:   
    sf::Sprite m_splash;
    sf::Sprite m_mask;
    sf::Sound m_sound;

    int m_alpha;

    // Functions
    void MainMenuStart();
    void LoadResources();

protected:
    // Destructor
    ~SplashScreenState() override;

    // Functions
    void HandleInput() override;
    void Update() override;
    void Draw(sf::RenderTarget& rTarget, float lag) override;

    void OnWindowResize() override;

public:
    // Constructor
    SplashScreenState(GameEngine& rGame);
};

#endif // SPLASHSCREENSTATE_H_INCLUDED
