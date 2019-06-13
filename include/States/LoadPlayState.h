#ifndef LOADPLAYSTATE_H
#define LOADPLAYSTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui/Gui.h"
#include "States/State.h"

class LoadPlayState final : public State
{
private:
    sf::Thread m_thread;

    sf::Sprite m_backgroundSprite;
    sf::Text m_loadingText;
    sf::Sound m_startSound;
    ProgressBar m_loadingBar;

    unsigned int m_progress;
    unsigned int m_total;

    std::string m_levelDirectory;
    bool m_isLoaded;

    // Destructor
    virtual ~LoadPlayState() override;

    // Functions
    void playStart();
    void loadResources();

    // State functions
    virtual void handleInput() override;
    virtual void update() override;
    virtual void draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void onWindowResize() override;

public:
    // Constructor
    LoadPlayState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // LOADPLAYSTATE_H
