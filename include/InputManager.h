#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>

class InputManager final
{
private:
    sf::RenderWindow& m_rWindow;

    // Input arrays
    std::array<bool, sf::Keyboard::KeyCount> m_keyStates;
    std::array<bool, sf::Keyboard::KeyCount> m_previousKeyStates;
    std::array<bool, sf::Mouse::ButtonCount> m_mouseButtonStates;
    std::array<bool, sf::Mouse::ButtonCount> m_previousMouseButtonStates;
    std::array<std::array<bool, sf::Joystick::ButtonCount>, sf::Joystick::Count> m_joystickButtonStates;
    std::array<std::array<bool, sf::Joystick::ButtonCount>, sf::Joystick::Count> m_previousJoystickButtonStates;

    // Window data and events
    bool m_isWindowFocused;

    bool m_closedEvent;
    bool m_resizedEvent;
    bool m_lostFocusEvent;
    bool m_gainedFocusEvent;
    bool m_mouseLeftEvent;
    bool m_mouseEnteredEvent;

    // Keyboard data and events
    sf::String m_enteredText;
    std::vector<sf::Keyboard::Key> m_eventPressedKeys;
    std::vector<sf::Keyboard::Key> m_eventReleasedKeys;

    // Mouse data and events
    std::vector<sf::Mouse::Button> m_eventPressedMouseButtons;
    std::vector<sf::Mouse::Button> m_eventReleasedMouseButtons;
    sf::Vector2i m_mousePosition;
    float m_mouseWheelDelta;

    bool m_mouseMovedEvent;

    // Joystick data and events
    std::array<std::vector<unsigned int>, sf::Joystick::Count> m_eventPressedJoystickButtons;
    std::array<std::vector<unsigned int>, sf::Joystick::Count> m_eventReleasedJoystickButtons;
    std::array<std::array<float, sf::Joystick::AxisCount>, sf::Joystick::Count> m_joystickAxesPosition;

    bool m_joystickMovedEvent;
    bool m_joystickConnectedEvent;
    bool m_joystickDisconnectedEvent;

    // Touch data and events
    bool m_isTouchHeld;

    bool m_touchBeganEvent;
    bool m_touchMovedEvent;
    bool m_touchEndedEvent;

    // Sensor data and events
    bool m_sensorChangedEvent;

    // Functions
    void UpdateInputStates();
    void PollSfmlEvents(sf::Window& rWindow);
    void ResetEvents();

#if defined(SFML_SYSTEM_MACOS)
    void MacOsCommandsToUnicode();
    std::string MacExec(const char* cmd);
#endif

public:
    // Constructor
    explicit InputManager(sf::RenderWindow& rWindow);

    // Functions
    void Update();

    // Window getters
    sf::Vector2u GetWindowDimensions() const { return m_rWindow.getSize(); }
    bool IsWindowFocused() const { return m_isWindowFocused; }

    const sf::Vector2f MapPixelToCoords(const sf::Vector2i& point, const sf::View& view) const
    {
        return m_rWindow.mapPixelToCoords(point, view);
    }
    const sf::Vector2f MapPixelToCoords(const sf::Vector2i& point) const { return m_rWindow.mapPixelToCoords(point); }
    const sf::Vector2i MapCoordsToPixel(const sf::Vector2f& position, const sf::View& view) const
    {
        return m_rWindow.mapCoordsToPixel(position, view);
    }
    const sf::Vector2i MapCoordsToPixel(const sf::Vector2f& position) const { return m_rWindow.mapCoordsToPixel(position); }

    bool DetectedClosedEvent() const { return m_closedEvent; }
    bool DetectedResizedEvent() const { return m_resizedEvent; }
    bool DetectedLostFocusEvent() const { return m_lostFocusEvent; }
    bool DetectedGainedFocusEvent() const { return m_gainedFocusEvent; }
    bool DetectedMouseLeftEvent() const { return m_mouseLeftEvent; }
    bool DetectedMouseEnteredEvent() const { return m_mouseEnteredEvent; }

    // Keyboard setters
    void SetVirtualKeyboardVisible(bool isVisible) const { sf::Keyboard::setVirtualKeyboardVisible(isVisible); }

    // Keyboard getters
    bool IsKeyHeld(sf::Keyboard::Key key) const;
    bool IsKeyDescending(sf::Keyboard::Key key, bool isRepeatEnabled = false) const;
    bool IsKeyAscending(sf::Keyboard::Key key) const;

    bool IsShiftKeyHeld() const { return (IsKeyHeld(sf::Keyboard::LShift) || IsKeyHeld(sf::Keyboard::RShift)); }
    bool IsShiftKeyDescending(bool isRepeatEnabled = false) const
    {
        return (IsKeyDescending(sf::Keyboard::LShift, isRepeatEnabled) || IsKeyDescending(sf::Keyboard::RShift, isRepeatEnabled));
    }
    bool IsShiftKeyAscending() const { return (IsKeyAscending(sf::Keyboard::LShift) || IsKeyAscending(sf::Keyboard::RShift)); }

    bool IsControlKeyHeld() const { return (IsKeyHeld(sf::Keyboard::LControl) || IsKeyHeld(sf::Keyboard::RControl)); }
    bool IsControlKeyDescending(bool isRepeatEnabled = false) const
    {
        return (IsKeyDescending(sf::Keyboard::LControl, isRepeatEnabled) || IsKeyDescending(sf::Keyboard::RControl, isRepeatEnabled));
    }
    bool IsControlKeyAscending() const { return (IsKeyAscending(sf::Keyboard::LControl) || IsKeyAscending(sf::Keyboard::RControl)); }

    bool IsAltKeyHeld() const { return (IsKeyHeld(sf::Keyboard::LAlt) || IsKeyHeld(sf::Keyboard::RAlt)); }
    bool IsAltKeyDescending(bool isRepeatEnabled = false) const
    {
        return (IsKeyDescending(sf::Keyboard::LAlt, isRepeatEnabled) || IsKeyDescending(sf::Keyboard::RAlt, isRepeatEnabled));
    }
    bool IsAltKeyAscending() const { return (IsKeyAscending(sf::Keyboard::LAlt) || IsKeyAscending(sf::Keyboard::RAlt)); }

    bool IsSystemKeyHeld() const { return (IsKeyHeld(sf::Keyboard::LSystem) || IsKeyHeld(sf::Keyboard::RSystem)); }
    bool IsSystemKeyDescending(bool isRepeatEnabled = false) const
    {
        return (IsKeyDescending(sf::Keyboard::LSystem, isRepeatEnabled) || IsKeyDescending(sf::Keyboard::RSystem, isRepeatEnabled));
    }
    bool IsSystemKeyAscending() const { return (IsKeyAscending(sf::Keyboard::LSystem) || IsKeyAscending(sf::Keyboard::RSystem)); }

    bool IsModifierKeyHeld() const;
    bool IsModifierKeyDescending(bool isRepeatEnabled = false) const;
    bool IsModifierKeyAscending() const;

    const sf::String& GetEnteredText() const { return m_enteredText; }

    bool DetectedTextEnteredEvent() const { return !m_enteredText.isEmpty(); }
    bool DetectedKeyPressedEvent() const { return !m_eventPressedKeys.empty(); }
    bool DetectedKeyReleasedEvent() const { return !m_eventReleasedKeys.empty(); }

    // Mouse getters
    bool IsMouseButtonHeld(sf::Mouse::Button button) const;
    bool IsMouseButtonDescending(sf::Mouse::Button button, bool isRepeatEnabled = false) const;
    bool IsMouseButtonAscending(sf::Mouse::Button button) const;

    const sf::Vector2i& GetWindowMousePosition() const { return m_mousePosition; }
    sf::Vector2f GetMousePosition(const sf::View& view) const;
    float GetMouseWheelDelta() const { return m_mouseWheelDelta; }

    bool DetectedMouseButtonPressedEvent() const { return !m_eventPressedMouseButtons.empty(); }
    bool DetectedMouseButtonReleasedEvent() const { return !m_eventReleasedMouseButtons.empty(); }
    bool DetectedMouseMovedEvent() const { return m_mouseMovedEvent; }
    bool DetectedMouseWheelScrolledEvent() const { return (m_mouseWheelDelta != 0); }

    // Joystick getters
    bool IsJoystickButtonHeld(unsigned int joystick, unsigned int button) const;
    bool IsJoystickButtonDescending(unsigned int joystick, unsigned int button, bool isRepeatEnabled = false) const;
    bool IsJoystickButtonAscending(unsigned int joystick, unsigned int button) const;

    bool IsJoystickConnected(unsigned int joystick) const { return sf::Joystick::isConnected(joystick); }
    sf::Joystick::Identification GetJoystickIdentification(unsigned int joystick) const
    {
        return sf::Joystick::getIdentification(joystick);
    }
    float GetJoystickAxisPosition(unsigned int joystick, sf::Joystick::Axis axis) const;

    bool DetectedJoystickButtonPressedEvent() const { return !m_eventPressedJoystickButtons.empty(); }
    bool DetectedJoystickButtonReleasedEvent() const { return !m_eventReleasedJoystickButtons.empty(); }
    bool DetectedJoystickMovedEvent() const { return m_joystickMovedEvent; }
    bool DetectedJoystickConnectedEvent() const { return m_joystickConnectedEvent; }
    bool DetectedJoystickDisconnectedEvent() const { return m_joystickDisconnectedEvent; }

    // Touch getters
    bool IsTouchHeld() const { return m_isTouchHeld; }

    bool DetectedTouchBeganEvent() const { return m_touchBeganEvent; }
    bool DetectedTouchMovedEvent() const { return m_touchMovedEvent; }
    bool DetectedTouchEndedEvent() const { return m_touchEndedEvent; }

    // Sensor getters
    bool DetectedSensorChangedEvent() const { return m_sensorChangedEvent; }

    // Clipboard
    void SetClipboardText(const sf::String& text);
    sf::String GetClipboardText() const;
};

#endif // INPUTMANAGER_H
