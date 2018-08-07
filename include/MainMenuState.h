#ifndef MAINMENUSTATE_H_INCLUDED
#define MAINMENUSTATE_H_INCLUDED

#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "State.h"

class MainMenuState : public State
{
private:
    sf::Sprite m_backgroundSprite;
    sf::Text m_gameNameText;
    sf::Text m_creditsText;
    sf::Music m_music;

    std::vector<GuiRectSoundButton> m_buttons;
    GuiSpriteButton m_muteButton;
    
    unsigned long m_elapsedTicks;

    // Functions
    void LoadPlayStart(const std::string& levelName);
    void CreatorStart();
    void ReadMusicSettings();

protected:
    // Destructor
    ~MainMenuState() override;

    // Functions
    void HandleInput() override;
    void Update() override;
    void Draw(sf::RenderTarget& rTarget, float lag) override;

    void Pause() override;
    void Resume() override;

    void OnWindowResize() override;

public:
    // Constructor
    MainMenuState(GameEngine& rGame);
};

#endif // MAINMENUSTATE_H_INCLUDED
