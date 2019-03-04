#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "LoopDebugOverlay.h"
#include "ResourceManager.h"

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
    bool m_stateChanged;

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
    State* Peek();
    void Push();
    void Pop(bool callResume = false);
    void Swap();
    void HandleRequests();
    void OnStateChange();
    void OnWindowResize();
    void ResetWindowView();

public:
    InputManager inputManager;

    // Constructor and destructor
    GameEngine();
    ~GameEngine();

    // Main game loop
    void GameLoop();

    // Functions to request handling of States
    void RequestPush(State* pState);
    void RequestPop(unsigned int statesToPop = 1);
    void RequestSwap(State* pState);

    // Draw the State under the current State
    void DrawPreviousState(const State* pCurrentState);

    // Loop clock functions
    void SetTargetUps(unsigned int updatesPerSecond);
    void SetTargetFps(unsigned int drawsPerSecond);
    double GetTargetUps() const;
    double GetTargetFps() const;
    double GetRecordedUps() const { return m_loopDebugOverlay.GetRecordedUps(); }
    double GetRecordedFps() const { return m_loopDebugOverlay.GetRecordedFps(); }

    // Loop debug overlay functions
    void ToggleDebugOverlay() { m_loopDebugOverlay.ToggleVisible(); }

    // Quit game
    void Quit();
};

#endif // GAMEENGINE_H
