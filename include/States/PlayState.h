#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui/Gui.h"
#include "Level/Level.h"
#include "States/State.h"

class PlayState final : public State
{
private:
    sf::RectangleShape m_darkness;

    sf::Music m_music;

    GuiSpriteButton m_muteButton;

    Level m_level;

    // Destructor
    virtual ~PlayState() override;

    // Functions
    void pauseStart();
    void readMusicSettings();

    // State functions
    virtual void handleInput() override;
    virtual void update() override;
    virtual void draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void pause() override;
    virtual void resume() override;

    virtual void onWindowResize() override;

public:
    // Constructor
    PlayState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // PLAYSTATE_H
