#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "State.h"

class MainMenuState final : public State
{
private:
    sf::Sprite m_backgroundSprite;
    sf::Text m_gameNameText;
    sf::Text m_creditsText;
    sf::Music m_music;

    std::vector<GuiRectSoundButton> m_buttons;
    GuiSpriteButton m_muteButton;

    unsigned long m_elapsedTicks;

    // Destructor
    virtual ~MainMenuState() override;

    // Functions
    void LoadPlayStart(const std::string& levelName);
    void CreatorStart();
    void ReadMusicSettings();

    // State functions
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void Pause() override;
    virtual void Resume() override;

    virtual void OnWindowResize() override;

public:
    // Constructor
    MainMenuState(GameEngine& rGame);
};

#endif // MAINMENUSTATE_H
