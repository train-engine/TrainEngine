#include "Core/InputManager.h"
#include <algorithm>
#include <string>
#include "Core/FileManager.h"
#if defined(SFML_SYSTEM_WINDOWS)
#include <windows.h>
#elif defined(SFML_SYSTEM_MACOS)
#include "Misc/MacClipboard.h"
#endif

InputManager::InputManager(sf::RenderWindow& rWindow)
    : m_rWindow(rWindow)
    , m_keyStates{}
    , m_previousKeyStates{}
    , m_mouseButtonStates{}
    , m_previousMouseButtonStates{}
    , m_joystickButtonStates{}
    , m_previousJoystickButtonStates{}
    , m_isWindowFocused(m_rWindow.hasFocus())
    , m_closedEvent(false)
    , m_resizedEvent(false)
    , m_lostFocusEvent(false)
    , m_gainedFocusEvent(false)
    , m_mouseLeftEvent(false)
    , m_mouseEnteredEvent(false)
    , m_mouseWheelDelta(0, 0)
    , m_mouseMovedEvent(false)
    , m_mouseWheelScrolledEvent(false)
    , m_eventPressedJoystickButtons{}
    , m_eventReleasedJoystickButtons{}
    , m_joystickAxesPosition{}
    , m_joystickMovedEvent(false)
    , m_joystickConnectedEvent(false)
    , m_joystickDisconnectedEvent(false)
    , m_joystickDeadZone(8.0)
    , m_isTouchHeld(false)
    , m_touchBeganEvent(false)
    , m_touchMovedEvent(false)
    , m_touchEndedEvent(false)
    , m_sensorChangedEvent(false)
{
#if defined(SFML_SYSTEM_WINDOWS)
    // Fix Windows focus on start issue (Merci Bill Gates)
    m_isWindowFocused = true;
#endif

    // Startup mouse position hack
    sf::Mouse::setPosition(sf::Mouse::getPosition() + sf::Vector2i(1, 1));
}

void InputManager::UpdateInputStates()
{
    m_previousKeyStates = m_keyStates;
    m_previousMouseButtonStates = m_mouseButtonStates;
    m_previousJoystickButtonStates = m_joystickButtonStates;
}

void InputManager::PollSfmlEvents(sf::Window& rWindow)
{
    sf::Event event;
    while (rWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_closedEvent = true;
            break;
        case sf::Event::Resized:
            m_resizedEvent = true;
            break;
        case sf::Event::LostFocus:
            m_lostFocusEvent = true;
            m_isWindowFocused = false;
            break;
        case sf::Event::GainedFocus:
            m_gainedFocusEvent = true;
            m_isWindowFocused = true;
            break;
        case sf::Event::TextEntered:
            m_enteredText += event.text.unicode;
            break;
        case sf::Event::KeyPressed:
            m_eventPressedKeys.push_back(event.key.code);
            // If key is known
            if (event.key.code != sf::Keyboard::Unknown)
            {
                m_keyStates[event.key.code] = true;
            }
            break;
        case sf::Event::KeyReleased:
            m_eventReleasedKeys.push_back(event.key.code);
            // If key is known
            if (event.key.code != sf::Keyboard::Unknown)
            {
                m_keyStates[event.key.code] = false;
            }
            break;
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                m_mouseWheelDelta.y += event.mouseWheelScroll.delta;
            }
            else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            {
                m_mouseWheelDelta.x += event.mouseWheelScroll.delta;
            }
            m_mouseWheelScrolledEvent = true;
            break;
        case sf::Event::MouseButtonPressed:
            m_eventPressedMouseButtons.push_back(event.mouseButton.button);
            m_mouseButtonStates[event.mouseButton.button] = true;
            break;
        case sf::Event::MouseButtonReleased:
            m_eventReleasedMouseButtons.push_back(event.mouseButton.button);
            m_mouseButtonStates[event.mouseButton.button] = false;
            break;
        case sf::Event::MouseMoved:
            m_mouseMovedEvent = true;
            m_previousMousePosition = m_mousePosition;
            m_mousePosition = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            break;
        case sf::Event::MouseEntered:
            m_mouseEnteredEvent = true;
            break;
        case sf::Event::MouseLeft:
            m_mouseLeftEvent = true;
            break;
        case sf::Event::JoystickButtonPressed:
            m_eventPressedJoystickButtons[event.joystickButton.joystickId].push_back(event.joystickButton.button);
            m_joystickButtonStates[event.joystickButton.joystickId][event.joystickButton.button] = true;
            break;
        case sf::Event::JoystickButtonReleased:
            m_eventReleasedJoystickButtons[event.joystickButton.joystickId].push_back(event.joystickButton.button);
            m_joystickButtonStates[event.joystickButton.joystickId][event.joystickButton.button] = false;
            break;
        case sf::Event::JoystickMoved:
            m_joystickMovedEvent = true;
            break;
        case sf::Event::JoystickConnected:
            m_joystickConnectedEvent = true;
            break;
        case sf::Event::JoystickDisconnected:
            m_joystickDisconnectedEvent = true;
            break;
        case sf::Event::TouchBegan:
            m_touchBeganEvent = true;
            m_mousePosition = sf::Vector2i(event.touch.x, event.touch.y);
            m_previousMousePosition = m_mousePosition; // Set equal to mouse position to have no delta because touch began
            m_isTouchHeld = true;
            break;
        case sf::Event::TouchMoved:
            m_touchMovedEvent = true;
            m_previousMousePosition = m_mousePosition;
            m_mousePosition = sf::Vector2i(event.touch.x, event.touch.y);
            break;
        case sf::Event::TouchEnded:
            m_touchEndedEvent = true;
            m_previousMousePosition = m_mousePosition;
            m_mousePosition = sf::Vector2i(event.touch.x, event.touch.y);
            m_isTouchHeld = false;
            break;
        case sf::Event::SensorChanged:
            m_sensorChangedEvent = true;
            break;
        default:
            break;
        }
    }

    for (std::size_t i = 0; i < m_joystickAxesPosition.size(); i++)
    {
        for (std::size_t j = 0; j < m_joystickAxesPosition[i].size(); j++)
        {
            m_joystickAxesPosition[i][j] = sf::Joystick::getAxisPosition(static_cast<unsigned int>(i), static_cast<sf::Joystick::Axis>(j));
        }
    }

#if defined(SFML_SYSTEM_MACOS)
    MacOsCommandsToUnicode();
#endif
}

void InputManager::ResetEvents()
{
    // Window data
    m_closedEvent = false;
    m_resizedEvent = false;
    m_lostFocusEvent = false;
    m_gainedFocusEvent = false;
    m_mouseLeftEvent = false;
    m_mouseEnteredEvent = false;

    // Keyboard data
    m_enteredText.clear();
    m_eventPressedKeys.clear();
    m_eventReleasedKeys.clear();

    // Mouse data
    m_eventPressedMouseButtons.clear();
    m_eventReleasedMouseButtons.clear();
    m_mouseWheelDelta = sf::Vector2f(0, 0);
    m_mouseMovedEvent = false;
    m_mouseWheelScrolledEvent = false;

    // Joystick data
    for (unsigned int i = 0; i < sf::Joystick::Count; i++)
    {
        m_eventPressedJoystickButtons[i].clear();
        m_eventReleasedJoystickButtons[i].clear();
    }
    m_joystickMovedEvent = false;
    m_joystickConnectedEvent = false;
    m_joystickDisconnectedEvent = false;

    // Touch data
    m_touchBeganEvent = false;
    m_touchMovedEvent = false;
    m_touchEndedEvent = false;

    // Sensor data
    m_sensorChangedEvent = false;
}

#if defined(SFML_SYSTEM_MACOS)
// Support Mac commands as Unicode characters included in the entered text
void InputManager::MacOsCommandsToUnicode()
{
    if (IsModifierKeyHeld())
    {
        if (IsKeyDescending(sf::Keyboard::A))
        {
            m_enteredText += static_cast<char>(1);
        }
        else if (IsKeyDescending(sf::Keyboard::C))
        {
            m_enteredText += static_cast<char>(3);
        }
        else if (IsKeyDescending(sf::Keyboard::V))
        {
            m_enteredText += static_cast<char>(22);
        }
        else if (IsKeyDescending(sf::Keyboard::X))
        {
            m_enteredText += static_cast<char>(24);
        }
    }
}

// Output commands to the Mac command line
std::string InputManager::MacExec(const char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
    {
        return "ERROR";
    }

    char buffer[128];
    std::string result;
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
        {
            unsigned int i = 0;
            while (buffer[i] != '\n')
            {
                result += buffer[i];
                i++;
            }
        }
    }
    pclose(pipe);
    return result;
}
#endif

void InputManager::Update()
{
    UpdateInputStates();
    ResetEvents();
    PollSfmlEvents(m_rWindow);
}

// Keyboard getters

bool InputManager::IsKeyHeld(sf::Keyboard::Key key) const
{
    if (m_isWindowFocused == false)
    {
        return false;
    }

    return m_keyStates[static_cast<int>(key)];
}

bool InputManager::IsKeyDescending(sf::Keyboard::Key key, bool isRepeatEnabled) const
{
    if (m_isWindowFocused == false)
    {
        return false;
    }
    // If repeating KeyPressed events should be ignored
    if (isRepeatEnabled == false && m_previousKeyStates[static_cast<int>(key)] == true)
    {
        return false;
    }

    return (std::find(m_eventPressedKeys.cbegin(), m_eventPressedKeys.cend(), key) != m_eventPressedKeys.cend());
}

bool InputManager::IsKeyAscending(sf::Keyboard::Key key) const
{
    return (std::find(m_eventReleasedKeys.cbegin(), m_eventReleasedKeys.cend(), key) != m_eventReleasedKeys.cend());
}

bool InputManager::IsModifierKeyHeld() const
{
#if defined(SFML_SYSTEM_WINDOWS) || defined(SFML_SYSTEM_LINUX)
    return IsControlKeyHeld();
#elif defined(SFML_SYSTEM_MACOS) || defined(SFML_SYSTEM_IOS)
    return IsSystemKeyHeld();
#endif
}

bool InputManager::IsModifierKeyDescending(bool isRepeatEnabled) const
{
#if defined(SFML_SYSTEM_WINDOWS) || defined(SFML_SYSTEM_LINUX)
    return IsControlKeyDescending(isRepeatEnabled);
#elif defined(SFML_SYSTEM_MACOS) || defined(SFML_SYSTEM_IOS)
    return IsSystemKeyDescending(isRepeatEnabled);
#endif
}

bool InputManager::IsModifierKeyAscending() const
{
#if defined(SFML_SYSTEM_WINDOWS) || defined(SFML_SYSTEM_LINUX)
    return IsControlKeyAscending();
#elif defined(SFML_SYSTEM_MACOS) || defined(SFML_SYSTEM_IOS)
    return IsSystemKeyAscending();
#endif
}

// Mouse getters

bool InputManager::IsMouseButtonHeld(sf::Mouse::Button button) const
{
    if (m_isWindowFocused == false)
    {
        return false;
    }

    return (m_mouseButtonStates[static_cast<int>(button)] || IsMouseButtonDescending(button));
}

bool InputManager::IsMouseButtonDescending(sf::Mouse::Button button, bool isRepeatEnabled) const
{
    if (m_isWindowFocused == false)
    {
        return false;
    }
    // If repeating MouseButtonPressed events should be ignored
    if (isRepeatEnabled == false && m_previousMouseButtonStates[static_cast<int>(button)] == true)
    {
        return false;
    }

    return (std::find(m_eventPressedMouseButtons.cbegin(), m_eventPressedMouseButtons.cend(), button) != m_eventPressedMouseButtons.cend());
}

bool InputManager::IsMouseButtonAscending(sf::Mouse::Button button) const
{
    return (std::find(m_eventReleasedMouseButtons.cbegin(), m_eventReleasedMouseButtons.cend(), button) !=
            m_eventReleasedMouseButtons.cend());
}

// Return the mouse position relative to a given view
sf::Vector2f InputManager::GetMousePosition(const sf::View& view) const
{
    return m_rWindow.mapPixelToCoords(m_mousePosition, view);
}

// Joystick getters

bool InputManager::IsJoystickButtonHeld(unsigned int joystick, unsigned int button) const
{
    if (m_isWindowFocused == false)
    {
        return false;
    }

    return (m_joystickButtonStates[joystick][button] || IsJoystickButtonDescending(joystick, button));
}

bool InputManager::IsJoystickButtonDescending(unsigned int joystick, unsigned int button, bool isRepeatEnabled) const
{
    if (m_isWindowFocused == false)
    {
        return false;
    }
    // If repeating JoystickButtonPressed events should be ignored
    if (isRepeatEnabled == false && m_previousJoystickButtonStates[joystick][button] == true)
    {
        return false;
    }

    return (std::find(m_eventPressedJoystickButtons[joystick].cbegin(), m_eventPressedJoystickButtons[joystick].cend(), button) !=
            m_eventPressedJoystickButtons[joystick].cend());
}

bool InputManager::IsJoystickButtonAscending(unsigned int joystick, unsigned int button) const
{
    return (std::find(m_eventReleasedJoystickButtons[joystick].cbegin(), m_eventReleasedJoystickButtons[joystick].cend(), button) !=
            m_eventReleasedJoystickButtons[joystick].cend());
}

float InputManager::GetJoystickAxisPosition(unsigned int joystick, sf::Joystick::Axis axis) const
{
    // Return 0 if the joystick does not have the specified axis
    if (sf::Joystick::hasAxis(joystick, axis) == false)
    {
        return 0;
    }

    // If joystick value is inside the deadzone, return 0
    if (m_joystickAxesPosition[joystick][axis] >= -m_joystickDeadZone &&
        m_joystickAxesPosition[joystick][axis] <= m_joystickDeadZone)
    {
        return 0;
    }

    // Defined by SFML
    static constexpr float maximumJoystickAxisValue = 100.0;

    // If the axis value is positive
    if (m_joystickAxesPosition[joystick][axis] > m_joystickDeadZone)
    {
        // Subtract m_joystickDeadZone to the real axis value
        return maximumJoystickAxisValue * (m_joystickAxesPosition[joystick][axis] - m_joystickDeadZone) /
               (maximumJoystickAxisValue - m_joystickDeadZone);
    }
    // If the axis value is negative
    else
    {
        // Add m_joystickDeadZone to the real axis value
        return maximumJoystickAxisValue * (m_joystickAxesPosition[joystick][axis] + m_joystickDeadZone) /
               (maximumJoystickAxisValue - m_joystickDeadZone);
    }
}

// Clipboard

void InputManager::SetClipboardText(const sf::String& text)
{
#if defined(SFML_SYSTEM_WINDOWS)
    HWND handle = m_rWindow.getSystemHandle();
    if (OpenClipboard(handle))
    {
        EmptyClipboard();
        HGLOBAL hClipboardData;

        const wchar_t* wcharCString = text.toWideString().c_str();

        hClipboardData = GlobalAlloc(GMEM_MOVEABLE, sizeof(wchar_t) * (wcslen(wcharCString) + 1));

        wchar_t* buffer;
        buffer = (wchar_t*)GlobalLock(hClipboardData);

        wcscpy(buffer, wcharCString);
        GlobalUnlock(hClipboardData);

        SetClipboardData(CF_UNICODETEXT, hClipboardData);
        CloseClipboard();
    }
#elif defined(SFML_SYSTEM_MACOS)
    SetMacClipboardText(text);
#endif
}

sf::String InputManager::GetClipboardText() const
{
#if defined(SFML_SYSTEM_WINDOWS)
    HWND handle = m_rWindow.getSystemHandle();
    if (OpenClipboard(handle))
    {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData != nullptr)
        {
            wchar_t* pText = static_cast<wchar_t*>(GlobalLock(hData));
            if (pText != nullptr)
            {
                CloseClipboard();
                return sf::String(pText);
            }
        }
        CloseClipboard();
    }
    return "";
#elif defined(SFML_SYSTEM_MACOS)
    return GetMacClipboardText();
#else
    return "";
#endif
}
