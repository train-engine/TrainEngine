#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui/Gui.h"
#include "States/State.h"

class PauseState final : public State
{
private:
    sf::Text m_pausedText;

    std::vector<GuiRectSoundButton> m_buttons;

    int m_alpha;

    // Destructor
    virtual ~PauseState() override;

    // State functions
    virtual void handleInput() override;
    virtual void update() override;
    virtual void draw(sf::RenderTarget& target, float lag) override;

    virtual void onWindowResize() override;

public:
    // Constructor
    explicit PauseState(GameEngine& game);
};

#endif // PAUSESTATE_H
