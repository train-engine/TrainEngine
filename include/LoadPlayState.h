#ifndef LOADPLAYSTATE_H_INCLUDED
#define LOADPLAYSTATE_H_INCLUDED

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "State.h"

class LoadPlayState : public State
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
    
    // Functions
    void PlayStart();
    void LoadResources();
    
protected:
    // Destructor
    ~LoadPlayState() override;
    
    // Functions
    void HandleInput() override;
    void Update() override;
    void Draw(sf::RenderTarget& rTarget, float lag) override;

public:
    // Constructor
    LoadPlayState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // LOADPLAYSTATE_H_INCLUDED
