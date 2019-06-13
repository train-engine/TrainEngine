#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Core/InputManager.h"
#include "Core/LoopDebugOverlay.h"
#include "Core/ResourceManager.h"

class State;

class GameEngine final
{
public:
    ResourceManager resourceManager; // Placed here for constructor initializer list order

private:
    // Window
    sf::RenderWindow m_window;

    // States
    std::vector<State*> m_states;

    // State-related actions handling
    enum class PendingRequest
    {
        Push,
        Pop,
        Swap
    };
    std::unordered_map<unsigned int, PendingRequest> m_pendingRequests;
    std::vector<State*> m_pendingStates;

    // Icon
    sf::Image m_icon;

    // Cursor
    sf::Sprite m_cursor;

    // Loop clock
    sf::Time m_timePerUpdate; // Amount of time there should be between updates
    sf::Time m_timePerDraw; // Amount of time there should be between draws
    sf::Time m_updateLag; // Amount of time since last update (can be much greater than m_timePerUpdate)
    sf::Time m_drawLag; // Amount of time since last draw
    bool m_isPowerSaverEnabled; // Sleep when there is enough time before the next update/draw
    LoopDebugOverlay m_loopDebugOverlay;

    // Functions
    void push();
    void pop();
    void handleRequests();
    void onWindowResize();
    void resetWindowView();

public:
    InputManager inputManager;

    // Constructor and destructor
    GameEngine();
    ~GameEngine();

    // Main game loop
    void startGameLoop();

    // Functions to request handling of States
    void requestPush(State* state);
    void requestPop(unsigned int statesToPop = 1);
    void requestSwap(State* state);

    // Draw the State under the current State
    void drawPreviousState(const State* currentState);

    // Loop clock functions
    void setTargetUps(unsigned int updatesPerSecond);
    void setTargetFps(unsigned int drawsPerSecond);
    double getTargetUps() const;
    double getTargetFps() const;
    double getRecordedUps() const { return m_loopDebugOverlay.getRecordedUps(); }
    double getRecordedFps() const { return m_loopDebugOverlay.getRecordedFps(); }

    // Loop debug overlay functions
    void toggleDebugOverlay() { m_loopDebugOverlay.toggleVisible(); }

    // Quit game
    void quit();
};

#endif // GAMEENGINE_H
