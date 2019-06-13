#ifndef MENUOPTIONSSTATE_H
#define MENUOPTIONSSTATE_H

#include <SFML/Graphics.hpp>
#include "Gui/Gui.h"
#include "States/State.h"

class MenuOptionsState final : public State
{
private:
    sf::Sprite m_backgroundSprite;
    sf::Text m_titleText;
    sf::Text m_soundSliderText;

    GuiTextSlider m_soundSlider;

    bool m_mustUpdateSoundSettings;

    // Destructor
    virtual ~MenuOptionsState() override;

    // State functions
    virtual void handleInput() override;
    virtual void update() override;
    virtual void draw(sf::RenderTarget& target, float lag) override;

    virtual void onWindowResize() override;

public:
    // Constructor
    explicit MenuOptionsState(GameEngine& game);
};

#endif // MENUOPTIONSSTATE_H
