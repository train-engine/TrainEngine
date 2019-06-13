#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui/Gui.h"
#include "States/State.h"

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
    void loadPlayStart(const std::string& levelName);
    void creatorStart();
    void readMusicSettings();

    // State functions
    virtual void handleInput() override;
    virtual void update() override;
    virtual void draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void resume() override;

    virtual void onWindowResize() override;

public:
    // Constructor
    explicit MainMenuState(GameEngine& rGame);
};

#endif // MAINMENUSTATE_H
