#ifndef PAUSESTATE_H_INCLUDED
#define PAUSESTATE_H_INCLUDED

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "State.h"

class PauseState : public State
{
private:
    sf::Text m_pausedText;

    std::vector<GuiRectSoundButton> m_buttons;

    int m_alpha;

protected:
    // Destructor
    ~PauseState() override;

    // Functions
    void HandleInput() override;
    void Update() override;
    void Draw(sf::RenderTarget& rTarget, float lag) override;

    void OnWindowResize() override;

public:
    // Constructor
    PauseState(GameEngine& rGame);
};

#endif // PAUSESTATE_H_INCLUDED
