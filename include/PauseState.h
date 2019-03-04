#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "State.h"

class PauseState final : public State
{
private:
    sf::Text m_pausedText;

    std::vector<GuiRectSoundButton> m_buttons;

    int m_alpha;

    // Destructor
    virtual ~PauseState() override;

    // State functions
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void OnWindowResize() override;

public:
    // Constructor
    explicit PauseState(GameEngine& rGame);
};

#endif // PAUSESTATE_H
