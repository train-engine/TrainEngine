#include "GameEngine.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include "FileManager.h"
#include "State.h"

namespace
{
    const std::string windowName = "TrainEngine";
    const sf::Vector2u minWindowDimensions(800, 600);
    const int initialUps = 60;
    const sf::Time sleepImprecision = sf::microseconds(1500); // Uncertainty given to the sleep time
    const float maxUpdatesBehind = 10; // Max number of updates lagging behind before discarding update cycles if the State's canSkipUpdates is true
}

/// Initialize the window and main systems
GameEngine::GameEngine()
    : m_window(),
      m_stateChanged(false),
      m_isPowerSaverEnabled(true),
      m_loopDebugOverlay(m_resourceManager.GetFont("altFont")),
      m_inputManager(m_window)
{
    std::ofstream f("allo.bin", std::ios::binary);
    f.put('\n');

    // Output game info
    std::cout << "TrainEngine 0.5.0-dev - May 23 2017\n"
                 "Game development started on: March 15 2015, 7:53:27 PM\n"
                 "Lines of code: 9000\n"
                 "Library used: SFML 2.4.2\n"
                 "Made by Simon Gauvin, Misha Krieger-Raynauld, Guillaume Jones, and Ba Minh Nguyen.\n\n";

    // Graphics settings
    std::ifstream inputFile(FileManager::ResourcePath() + "data/settings/graphics_settings.txt");
    if (inputFile)
    {
        unsigned int fullscreenModeIndex = 0;
        inputFile >> fullscreenModeIndex;

        bool isFullscreen = false;
        inputFile >> isFullscreen;

        bool isVSyncEnabled = false;
        inputFile >> isVSyncEnabled;

        inputFile >> m_isPowerSaverEnabled;

        unsigned int targetFps = 60;
        inputFile >> targetFps;

        unsigned int antiAliasingLevel = 0;
        inputFile >> antiAliasingLevel;

        inputFile.close();

        fullscreenModeIndex = (fullscreenModeIndex < sf::VideoMode::getFullscreenModes().size() && fullscreenModeIndex > 0) ?
                              fullscreenModeIndex :
                              static_cast<unsigned int>(sf::VideoMode::getFullscreenModes().size()) - 1;

        unsigned int style = (isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);

        sf::ContextSettings contextSettings = sf::ContextSettings(0, 0, antiAliasingLevel);

        m_window.create(sf::VideoMode::getFullscreenModes()[fullscreenModeIndex], windowName, style, contextSettings);
        m_window.setVerticalSyncEnabled(isVSyncEnabled);
        SetTargetFps(targetFps);

        std::cout << "Successfully read graphics settings.\n";
    }
    else
    {
        m_window.create(sf::VideoMode(1280, 720), windowName);
        SetTargetFps(60);
        std::cout << "\nGameEngine error: Unable to open \"data/settings/graphics_settings.txt\".\n"
                     "Graphics settings loading failed.\n\n";
    }

    // Window initialization
    #if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
        m_window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
    #endif
    OnWindowResize();
    #if !defined(SFML_SYSTEM_IOS) && !defined(SFML_SYSTEM_ANDROID)
        // Center window
        m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2,
                                          sf::VideoMode::getDesktopMode().height / 2) - static_cast<sf::Vector2i>(m_window.getSize()) / 2);
    #endif
    m_window.setActive();

    // Icon
    if (m_icon.loadFromFile(FileManager::ResourcePath() + "res/icon.png"))
    {
        m_window.setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());
    }
    else
    {
        std::cout << "\nGameEngine error: Unable to open \"res/icon.png\".\n"
                     "Program icon loading failed.\n\n";
    }

    // Base State layout
    State::ResizeLayout(static_cast<sf::Vector2f>(m_window.getSize()));

    // Cursor
    m_window.setMouseCursorVisible(false);
    m_cursor.setTexture(m_resourceManager.GetTexture("cursor"));

    SetTargetUps(initialUps);
}

/// Call the destructor of each State by using Pop()
GameEngine::~GameEngine()
{
    while (!m_states.empty())
    {
        Pop();
    }
    std::cout << "TrainEngine quit successfully.\n";
}

/// Return a pointer to the topmost State in the stack
State* GameEngine::Peek()
{
    if (m_states.empty())
    {
        return nullptr;
    }
    return m_states.back();
}

/// Add a new State to the stack from the queue
void GameEngine::Push()
{
    m_states.emplace_back(m_pendingStates.back());
    m_states.back()->m_orderCreated = 0;
    m_pendingStates.pop_back();

    m_states.back()->OnWindowResize(); // Call OnWindowResize on State creation
}

/// Remove one State from the top of the stack, and call Resume on the State below if desired
void GameEngine::Pop(bool callResume)
{
    if (m_states.empty())
    {
        return;
    }

    delete m_states.back();
    m_states.pop_back();
    if (!m_states.empty() && callResume == true)
    {
        Peek()->Resume();
    }
}

/// Swap the topmost State with a new one from the queue
void GameEngine::Swap()
{
    if (!m_states.empty())
    {
        delete m_states.back();
        m_states.pop_back();
    }
    Push();
}

/// Process the tick's requested State handling
void GameEngine::HandleRequests()
{
    // Sort new States by smallest order last, to later be able to simply call pop_back()
    if (m_pendingStates.size() > 1)
    {
        std::sort(m_pendingStates.begin(), m_pendingStates.end(), [](const State* a, const State* b)
        {
            return a->m_orderCreated > b->m_orderCreated;
        });
    }

    // Handle pending requests
    for (auto it = m_pendingRequests.begin(); it != m_pendingRequests.end(); ++it)
    {
        switch (it->second)
        {
        case PendingRequest::Pop:
            Pop(std::next(it) == m_pendingRequests.end()); // Send true if the Pop is the last request, so that Resume is called on the State below it
            break;
        case PendingRequest::Push:
            Push();
            break;
        case PendingRequest::Swap:
            Swap();
            break;
        }
    }

    m_pendingRequests.clear();
    m_pendingStates.clear();
    State::s_orderCounter = 0;
}

/// Reset loop timing on State change
void GameEngine::OnStateChange()
{
    m_updateLag = m_timePerUpdate; // Force update on next cycle
    m_stateChanged = false;
}

/// Actions to perform when the window is resized
void GameEngine::OnWindowResize()
{
    #if !defined(SFML_SYSTEM_IOS) && !defined(SFML_SYSTEM_ANDROID)
        // Window minimum dimensions
        if (m_window.getSize().x < minWindowDimensions.x)
        {
            m_window.setSize(sf::Vector2u(minWindowDimensions.x, m_window.getSize().y));
        }
        if (m_window.getSize().y < minWindowDimensions.y)
        {
            m_window.setSize(sf::Vector2u(m_window.getSize().x, minWindowDimensions.y));
        }
    #endif

    // View resizing
    ResetWindowView();
}

/// Set the window's view equal to a view the size of its dimensions and positioned at (0, 0)
void GameEngine::ResetWindowView()
{
    m_window.setView(sf::View(sf::FloatRect(0, 0, m_window.getSize().x, m_window.getSize().y)));
}


/// Main game loop
void GameEngine::GameLoop()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        if (m_stateChanged == true)
        {
            HandleRequests();
            OnStateChange();
        }

        if (Peek() != nullptr)
        {
            // CPU sleep
            if (m_isPowerSaverEnabled == true)
            {
                // Sleep if the next update is sooner than the next draw and the time before the next update is greater than the sleep imprecision
                if (m_timePerUpdate - m_updateLag <= m_timePerDraw - m_drawLag && m_timePerUpdate - m_updateLag > sleepImprecision)
                {
                    sf::Time timeToSleepFor = m_timePerUpdate - m_updateLag - sleepImprecision;
                    std::this_thread::sleep_for(std::chrono::microseconds(timeToSleepFor.asMicroseconds()));
                }
                // Sleep if the next draw is sooner than the next update and the time before the next draw is greater than the sleep imprecision
                else if (m_timePerUpdate - m_updateLag > m_timePerDraw - m_drawLag && m_timePerDraw - m_drawLag > sleepImprecision)
                {
                    sf::Time timeToSleepFor = m_timePerDraw - m_drawLag - sleepImprecision;
                    std::this_thread::sleep_for(std::chrono::microseconds(timeToSleepFor.asMicroseconds()));
                }
            }

            // Restart clock and update lag times
            sf::Time elapsedTime = clock.restart();
            m_updateLag += elapsedTime;
            m_drawLag += elapsedTime;

            // Output warning to console if canSkipUpdates is false and the CPU can't keep up with the cycles
            if (m_updateLag >= m_timePerUpdate * maxUpdatesBehind && Peek()->m_stateSettings.canSkipUpdates == false)
            {
                std::cout << "GameEngine warning: Unable to keep up, catching up with " << static_cast<unsigned int>(m_updateLag / m_timePerUpdate) << " ticks\n";
            }

            // HandleInput and Update on a fixed timestep (skip draw until caught up)
            while (m_updateLag >= m_timePerUpdate && m_stateChanged == false)
            {
                sf::Time startTime = clock.getElapsedTime();

                // InputManager
                m_inputManager.Update();

                // Window resizing
                if (m_inputManager.DetectedResizedEvent())
                {
                    OnWindowResize();
                    State::ResizeLayout(static_cast<sf::Vector2f>(m_window.getSize()));
                    for (const auto& pState : m_states)
                    {
                        pState->OnWindowResize();
                    }
                    m_loopDebugOverlay.OnWindowResize();
                }

                // HandleInput
                Peek()->BaseHandleInput();
                Peek()->HandleInput();

                // Update
                if (m_stateChanged == false)
                {
                    Peek()->Update();
                }

                m_loopDebugOverlay.RecordUpdate(clock.getElapsedTime() - startTime);

                m_updateLag -= m_timePerUpdate;

                // Skip updates if current State does not rely on fixed updates
                if (m_updateLag >= m_timePerUpdate * maxUpdatesBehind && Peek()->m_stateSettings.canSkipUpdates == true)
                {
                    m_updateLag %= m_timePerUpdate;
                }
            }

            // Restart clock and update lag times
            elapsedTime = clock.restart();
            m_updateLag += elapsedTime;
            m_drawLag += elapsedTime;

            // Draw
            if (m_drawLag >= m_timePerDraw && m_stateChanged == false)
            {
                sf::Time startTime = clock.getElapsedTime();

                m_window.clear();
                ResetWindowView();

                // Draw with fraction of cycle elapsed before the next update (for interpolation)
                Peek()->Draw(m_window, static_cast<float>(m_updateLag.asMicroseconds()) / m_timePerUpdate.asMicroseconds());

                ResetWindowView();

                m_window.draw(m_loopDebugOverlay);
                m_cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)));
                m_window.draw(m_cursor);
                m_window.display();

                if (m_timePerDraw == sf::Time::Zero) // Prevent overflow if FPS is uncapped
                {
                    m_drawLag = sf::Time::Zero;
                }
                else
                {
                    m_drawLag %= m_timePerDraw; // Extra lag is not created if the GPU cannot keep up
                }

                m_loopDebugOverlay.RecordDraw(clock.getElapsedTime() - startTime);
            }
        }
        else
        {
            Quit();
        }
    }
}

/// Request a State's addition and add it to the queue
void GameEngine::RequestPush(State* pState)
{
    m_pendingRequests.emplace(pState->m_orderCreated, PendingRequest::Push);
    m_pendingStates.emplace_back(pState);
    pState = nullptr;
    m_stateChanged = true;
}

/// Request a State's removal
void GameEngine::RequestPop(unsigned int statesToPop)
{
    if (statesToPop > 0)
    {
        while (statesToPop > 0)
        {
            m_pendingRequests.emplace(State::s_orderCounter++, PendingRequest::Pop);
            statesToPop--;
        }
        m_stateChanged = true;
    }
}

/// Request a State's swapping and add the new State to the queue
void GameEngine::RequestSwap(State* pState)
{
    m_pendingRequests.emplace(pState->m_orderCreated, PendingRequest::Swap);
    m_pendingStates.emplace_back(pState);
    pState = nullptr;
    m_stateChanged = true;
}

/// Draw the State under the current State (takes the calling State's "this" pointer to enable drawing multiple states on top of one another)
void GameEngine::DrawPreviousState(const State* pCurrentState)
{
    ResetWindowView(); // Reset the view to guarantee that the previous State has a predictable and normal view

    // Search for the current State, and make sure it can be found and that it is not first in the stack (as we need to draw the State before it)
    std::vector<State*>::iterator it = std::find(m_states.begin(), m_states.end(), pCurrentState);
    if (it != m_states.end() && it != m_states.begin())
    {
        --it;
        (*it)->Draw(m_window);

        ResetWindowView(); // Reset the view to guarantee that the current State continues to use a predictable and normal view
    }
}

// Loop clock functions

void GameEngine::SetTargetUps(unsigned int updatesPerSecond)
{
    if (updatesPerSecond == 0)
    {
        return;
    }
    m_timePerUpdate = sf::microseconds(1000000 / static_cast<double>(updatesPerSecond));
    m_updateLag = m_timePerUpdate;
}

void GameEngine::SetTargetFps(unsigned int drawsPerSecond)
{
    m_timePerDraw = (drawsPerSecond != 0) ? sf::microseconds(1000000 / static_cast<double>(drawsPerSecond)) : sf::Time::Zero;
    m_drawLag = m_timePerDraw;
}

double GameEngine::GetTargetUps() const
{
    return 1000000 / static_cast<double>(m_timePerUpdate.asMicroseconds());
}

double GameEngine::GetTargetFps() const
{
    if (m_timePerDraw == sf::Time::Zero)
    {
        return 0;
    }
    return 1000000 / static_cast<double>(m_timePerDraw.asMicroseconds());
}

/// Quit game
void GameEngine::Quit()
{
    m_window.close();
}
