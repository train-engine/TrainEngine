#ifndef PLAYSTATE_H_INCLUDED
#define PLAYSTATE_H_INCLUDED

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gui.h"
#include "Level.h"
#include "State.h"

class PlayState : public State
{
private:
    sf::RectangleShape m_darkness;

    sf::Music m_music;

    GuiSpriteButton m_muteButton;

    Level m_level;

    // Functions
    void PauseStart();
    void ReadMusicSettings();
    void UnloadResources() const;

protected:
    // Destructor
    ~PlayState() override;

    // Functions
    void HandleInput() override;
    void Update() override;
    void Draw(sf::RenderTarget& rTarget, float lag) override;

    void Pause() override;
    void Resume() override;

    void OnWindowResize() override;

public:
    // Constructor
    PlayState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // PLAYSTATE_H_INCLUDED
