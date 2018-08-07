#ifndef MENUOPTIONSSTATE_H_INCLUDED
#define MENUOPTIONSSTATE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "State.h"

class MenuOptionsState : public State
{
private:
    sf::Sprite m_backgroundSprite;
    sf::Text m_titleText;
    sf::Text m_soundSliderText;

    GuiTextSlider m_soundSlider;

    bool m_updateSoundSettings;

protected:
    // Destructor
    ~MenuOptionsState();

    // Functions
    void HandleInput() override;
    void Update() override;
    void Draw(sf::RenderTarget& rTarget, float lag) override;

    void OnWindowResize() override;

public:
    // Constructor
    MenuOptionsState(GameEngine& rGame);
};

#endif // MENUOPTIONSSTATE_H_INCLUDED
