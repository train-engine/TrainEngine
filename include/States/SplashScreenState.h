#ifndef SPLASHSCREENSTATE_H
#define SPLASHSCREENSTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "States/State.h"

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
    void mainMenuStart();

    // State functions
    virtual void handleInput() override;
    virtual void update() override;
    virtual void draw(sf::RenderTarget& target, float lag) override;

    virtual void onWindowResize() override;

public:
    // Constructor
    explicit SplashScreenState(GameEngine& game);
};

#endif // SPLASHSCREENSTATE_H
