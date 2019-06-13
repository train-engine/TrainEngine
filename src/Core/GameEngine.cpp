#include "Core/GameEngine.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include "Core/FileManager.h"
#include "States/State.h"

namespace
{
    const std::string windowName = "TrainEngine";
    const sf::Vector2u minWindowDimensions(800, 600);
    const int initialUps = 60;
    const sf::Time sleepImprecision = sf::microseconds(1500); // Uncertainty given to the sleep time
    const float maxUpdatesBehind = 10; // Max number of updates lagging behind before discarding
                                       // update cycles if the State's canSkipUpdates is true
} // namespace

/// Initialize the window and main systems
GameEngine::GameEngine()
    : m_window()
    , m_isPowerSaverEnabled(true)
    , m_loopDebugOverlay(resourceManager.getFont("altFont"))
    , inputManager(m_window)
{
    // Output game info
    std::cout << "TrainEngine 0.5.0-dev - May 23 2017\n"
                 "Game development started on: March 15 2015, 7:53:27 PM\n"
                 "Lines of code: 10400\n"
                 "Library used: SFML 2.4.2\n"
                 "Made by Simon Gauvin, Misha Krieger-Raynauld, Guillaume Jones, and Ba Minh Nguyen.\n\n";

    // Graphics settings
    static const std::string graphicsSettingsFilename = "data/settings/graphics_settings.txt";
    std::ifstream inputFile(FileManager::resourcePath() + graphicsSettingsFilename);
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

        // Set to first fullscreen mode index (best mode) if saved fullscreen mode index is unavailable
        if (fullscreenModeIndex >= sf::VideoMode::getFullscreenModes().size())
        {
            fullscreenModeIndex = 0;
        }

        unsigned int style = (isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);

        sf::ContextSettings contextSettings = sf::ContextSettings(0, 0, antiAliasingLevel);

        m_window.create(sf::VideoMode::getFullscreenModes()[fullscreenModeIndex], windowName, style, contextSettings);
        m_window.setVerticalSyncEnabled(isVSyncEnabled);
        setTargetFps(targetFps);

        std::cout << "Successfully read graphics settings.\n";
    }
    else
    {
        m_window.create(sf::VideoMode(1280, 720), windowName);
        setTargetFps(60);
        std::cerr << "\nGameEngine error: Unable to open \"" << graphicsSettingsFilename << "\".\n"
                  << "Graphics settings loading failed.\n\n";
    }

    // Window initialization
#if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
    m_window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
#endif
    onWindowResize();
#if !defined(SFML_SYSTEM_IOS) && !defined(SFML_SYSTEM_ANDROID)
    // Center window
    m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2) -
                         static_cast<sf::Vector2i>(m_window.getSize()) / 2);
#endif
    m_window.setActive();

    // Icon
    static const std::string iconFilename = "res/icon.png";
    if (m_icon.loadFromFile(FileManager::resourcePath() + iconFilename))
    {
        m_window.setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());
    }
    else
    {
        std::cerr << "\nGameEngine error: Unable to open \"" << iconFilename << "\".\n"
                  << "Program icon loading failed.\n\n";
    }

    // Cursor
    m_window.setMouseCursorVisible(false);
    m_cursor.setTexture(resourceManager.getTexture("cursor"));

    setTargetUps(initialUps);
}

/// Call the destructor of each State by using Pop()
GameEngine::~GameEngine()
{
    while (!m_states.empty())
    {
        pop();
    }
    std::cout << "TrainEngine quit successfully.\n";
}

/// Add a new State to the stack from the queue
void GameEngine::push()
{
    // Push new State
    m_states.push_back(m_pendingStates.back());
    m_states.back()->m_orderCreated = 0;
    m_pendingStates.pop_back();

    // Call OnWindowResize() on State creation
    m_states.back()->onWindowResize();
}

/// Remove one State from the top of the stack
void GameEngine::pop()
{
    // Return if there are no States to remove
    if (m_states.empty())
    {
        return;
    }

    // Remove topmost State
    delete m_states.back();
    m_states.pop_back();
}

/// Process the tick's requested State handling
void GameEngine::handleRequests()
{
    // Sort new States by smallest order last, to later be able to simply call pop_back()
    if (m_pendingStates.size() > 1)
    {
        std::sort(m_pendingStates.begin(), m_pendingStates.end(), [](const State* a, const State* b) {
            return a->m_orderCreated > b->m_orderCreated;
        });
    }

    // Handle pending requests
    for (auto it = m_pendingRequests.cbegin(), end = m_pendingRequests.cend(); it != end; ++it)
    {
        switch (it->second)
        {
        case PendingRequest::Push:
            // Call Pause() on State about to be hidden before adding a new one on top of it
            if (!m_states.empty())
            {
                m_states.back()->pause();
            }
            push();
            break;
        case PendingRequest::Pop:
            pop();
            // Call Resume() on revealed State if this pop is the last request
            if (std::next(it) == end && !m_states.empty())
            {
                m_states.back()->resume();
            }
            break;
        case PendingRequest::Swap:
            pop();
            push();
            break;
        }
    }

    // Clear request stacks and reset order counter for next cycle
    m_pendingRequests.clear();
    m_pendingStates.clear();
    State::s_orderCounter = 0;

    // Force update on next cycle
    m_updateLag = m_timePerUpdate;
}

/// Actions to perform when the window is resized
void GameEngine::onWindowResize()
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
    resetWindowView();

    // Base State layout
    State::resizeLayout(static_cast<sf::Vector2f>(m_window.getSize()));
}

/// Set the window's view equal to a view the size of its dimensions and positioned at (0, 0)
void GameEngine::resetWindowView()
{
    m_window.setView(sf::View(sf::FloatRect(0, 0, m_window.getSize().x, m_window.getSize().y)));
}

/// Main game loop
void GameEngine::startGameLoop()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        if (!m_pendingRequests.empty())
        {
            handleRequests();
        }

        if (!m_states.empty())
        {
            // CPU sleep
            if (m_isPowerSaverEnabled == true)
            {
                // Sleep if the next update is sooner than the next draw and
                // the time before the next update is greater than the sleep imprecision
                if (m_timePerUpdate - m_updateLag <= m_timePerDraw - m_drawLag && m_timePerUpdate - m_updateLag > sleepImprecision)
                {
                    sf::Time timeToSleepFor = m_timePerUpdate - m_updateLag - sleepImprecision;
                    std::this_thread::sleep_for(std::chrono::microseconds(timeToSleepFor.asMicroseconds()));
                }
                // Sleep if the next draw is sooner than the next update and
                // the time before the next draw is greater than the sleep imprecision
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
            if (m_updateLag >= m_timePerUpdate * maxUpdatesBehind && m_states.back()->m_stateSettings.canSkipUpdates == false)
            {
                std::cout << "GameEngine warning: Unable to keep up, skipping " << static_cast<unsigned int>(m_updateLag / m_timePerUpdate)
                          << " ticks to catch up.\n";
            }

            // HandleInput and Update on a fixed timestep (skip draw until caught up)
            while (m_updateLag >= m_timePerUpdate && m_pendingRequests.empty())
            {
                sf::Time startTime = clock.getElapsedTime();

                // InputManager update
                inputManager.update();

                // Window resizing
                if (inputManager.detectedResizedEvent())
                {
                    onWindowResize();
                    for (const auto& pState : m_states)
                    {
                        pState->onWindowResize();
                    }
                    m_loopDebugOverlay.onWindowResize();
                }

                // HandleInput
                m_states.back()->baseHandleInput();
                m_states.back()->handleInput();

                // Update
                if (m_pendingRequests.empty())
                {
                    m_states.back()->update();
                }

                m_loopDebugOverlay.recordUpdate(clock.getElapsedTime() - startTime);

                m_updateLag -= m_timePerUpdate;

                // Skip updates if current State does not rely on fixed updates
                if (m_updateLag >= m_timePerUpdate * maxUpdatesBehind && m_states.back()->m_stateSettings.canSkipUpdates == true)
                {
                    m_updateLag %= m_timePerUpdate;
                }
            }

            // Restart clock and update lag times
            elapsedTime = clock.restart();
            m_updateLag += elapsedTime;
            m_drawLag += elapsedTime;

            // Draw
            if (m_drawLag >= m_timePerDraw && m_pendingRequests.empty())
            {
                sf::Time startTime = clock.getElapsedTime();

                m_window.clear();
                resetWindowView();

                // Draw with fraction of cycle elapsed before the next update (for interpolation)
                m_states.back()->draw(m_window, static_cast<float>(m_updateLag.asMicroseconds()) / m_timePerUpdate.asMicroseconds());

                resetWindowView();

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

                m_loopDebugOverlay.recordDraw(clock.getElapsedTime() - startTime);
            }
        }
        else
        {
            quit();
        }
    }
}

/// Request a State's addition and add it to the queue
void GameEngine::requestPush(State* pState)
{
    m_pendingRequests.emplace(pState->m_orderCreated, PendingRequest::Push);
    m_pendingStates.push_back(pState);
}

/// Request a State's removal
void GameEngine::requestPop(unsigned int statesToPop)
{
    while (statesToPop > 0)
    {
        m_pendingRequests.emplace(State::s_orderCounter++, PendingRequest::Pop);
        statesToPop--;
    }
}

/// Request a State's swapping and add the new State to the queue
void GameEngine::requestSwap(State* pState)
{
    m_pendingRequests.emplace(pState->m_orderCreated, PendingRequest::Swap);
    m_pendingStates.push_back(pState);
}

/// Draw the State under the current State (takes the calling State's "this" pointer
/// to enable drawing multiple states on top of one another)
void GameEngine::drawPreviousState(const State* pCurrentState)
{
    resetWindowView(); // Reset the view to guarantee that the previous State has a predictable and normal view

    // Search for the current State, and make sure it can be found and that it is not first in the stack
    // (as we need to draw the State before it)
    auto it = std::find(m_states.begin(), m_states.end(), pCurrentState);
    if (it != m_states.end() && it != m_states.begin())
    {
        --it;
        (*it)->draw(m_window);

        // Reset the view to guarantee that the current State continues to use a predictable and normal view
        resetWindowView();
    }
}

// Loop clock functions

void GameEngine::setTargetUps(unsigned int updatesPerSecond)
{
    if (updatesPerSecond == 0)
    {
        return;
    }

    m_timePerUpdate = sf::microseconds(1000000 / static_cast<double>(updatesPerSecond));
    m_updateLag = m_timePerUpdate;
}

void GameEngine::setTargetFps(unsigned int drawsPerSecond)
{
    m_timePerDraw = (drawsPerSecond != 0 ? sf::microseconds(1000000 / static_cast<double>(drawsPerSecond)) : sf::Time::Zero);
    m_drawLag = m_timePerDraw;
}

double GameEngine::getTargetUps() const
{
    return 1000000 / static_cast<double>(m_timePerUpdate.asMicroseconds());
}

double GameEngine::getTargetFps() const
{
    if (m_timePerDraw == sf::Time::Zero)
    {
        return 0;
    }

    return 1000000 / static_cast<double>(m_timePerDraw.asMicroseconds());
}

/// Quit game
void GameEngine::quit()
{
    m_window.close();
}
