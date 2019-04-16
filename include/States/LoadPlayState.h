#ifndef LOADPLAYSTATE_H
#define LOADPLAYSTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "State.h"

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
    void PlayStart();
    void LoadResources();

    // State functions
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void OnWindowResize() override;

public:
    // Constructor
    LoadPlayState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // LOADPLAYSTATE_H
