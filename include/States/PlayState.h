#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "InputContext.h"
#include "Level.h"
#include "State.h"

class PlayState final : public State
{
private:
    sf::RectangleShape m_darkness;

    sf::Music m_music;

    GuiSpriteButton m_muteButton;

    Level m_level;

    InputContext m_inputContext;

    // Destructor
    virtual ~PlayState() override;

    // Functions
    void PauseStart();
    void ReadMusicSettings();

    // State functions
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void Pause() override;
    virtual void Resume() override;

    virtual void OnWindowResize() override;

    void test(bool test);

public:
    // Constructor
    PlayState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // PLAYSTATE_H
