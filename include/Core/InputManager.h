#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>

class InputManager final
{
private:
    sf::RenderWindow& m_window;

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
    sf::Vector2i m_previousMousePosition;
    sf::Vector2f m_mouseWheelDelta;

    bool m_mouseMovedEvent;
    bool m_mouseWheelScrolledEvent;

    // Joystick data and events
    std::array<std::vector<unsigned int>, sf::Joystick::Count> m_eventPressedJoystickButtons;
    std::array<std::vector<unsigned int>, sf::Joystick::Count> m_eventReleasedJoystickButtons;
    std::array<std::array<float, sf::Joystick::AxisCount>, sf::Joystick::Count> m_joystickAxesPosition;
    float m_joystickDeadZone;

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
    void updateInputStates();
    void pollSfmlEvents(sf::Window& window);
    void resetEvents();

#if defined(SFML_SYSTEM_MACOS)
    void macOsCommandsToUnicode();
    std::string macExec(const char* cmd);
#endif

public:
    // Constructor
    explicit InputManager(sf::RenderWindow& window);

    // Functions
    void update();

    // Window getters
    sf::Vector2u getWindowDimensions() const { return m_window.getSize(); }
    bool isWindowFocused() const { return m_isWindowFocused; }

    sf::Vector2f mapPixelToCoords(const sf::Vector2i& point, const sf::View& view) const;
    sf::Vector2f mapPixelToCoords(const sf::Vector2i& point) const;

    sf::Vector2i mapCoordsToPixel(const sf::Vector2f& position, const sf::View& view) const;
    sf::Vector2i mapCoordsToPixel(const sf::Vector2f& position) const;

    bool detectedClosedEvent() const { return m_closedEvent; }
    bool detectedResizedEvent() const { return m_resizedEvent; }
    bool detectedLostFocusEvent() const { return m_lostFocusEvent; }
    bool detectedGainedFocusEvent() const { return m_gainedFocusEvent; }
    bool detectedMouseLeftEvent() const { return m_mouseLeftEvent; }
    bool detectedMouseEnteredEvent() const { return m_mouseEnteredEvent; }

    // Keyboard setters
    void setVirtualKeyboardVisible(bool isVisible) { sf::Keyboard::setVirtualKeyboardVisible(isVisible); }

    // Keyboard getters
    bool isKeyHeld(sf::Keyboard::Key key) const;
    bool isKeyDescending(sf::Keyboard::Key key, bool isRepeatEnabled = false) const;
    bool isKeyAscending(sf::Keyboard::Key key) const;

    bool isShiftKeyHeld() const;
    bool isShiftKeyDescending(bool isRepeatEnabled = false) const;
    bool isShiftKeyAscending() const;

    bool isControlKeyHeld() const;
    bool isControlKeyDescending(bool isRepeatEnabled = false) const;
    bool isControlKeyAscending() const;

    bool isAltKeyHeld() const;
    bool isAltKeyDescending(bool isRepeatEnabled = false) const;
    bool isAltKeyAscending() const;

    bool isSystemKeyHeld() const;
    bool isSystemKeyDescending(bool isRepeatEnabled = false) const;
    bool isSystemKeyAscending() const;

    bool isModifierKeyHeld() const;
    bool isModifierKeyDescending(bool isRepeatEnabled = false) const;
    bool isModifierKeyAscending() const;

    const sf::String& getEnteredText() const { return m_enteredText; }

    bool detectedTextEnteredEvent() const { return !m_enteredText.isEmpty(); }
    bool detectedKeyPressedEvent() const { return !m_eventPressedKeys.empty(); }
    bool detectedKeyReleasedEvent() const { return !m_eventReleasedKeys.empty(); }

    // Mouse getters
    bool isMouseButtonHeld(sf::Mouse::Button button) const;
    bool isMouseButtonDescending(sf::Mouse::Button button, bool isRepeatEnabled = false) const;
    bool isMouseButtonAscending(sf::Mouse::Button button) const;

    const sf::Vector2i& getWindowMousePosition() const { return m_mousePosition; }
    sf::Vector2f getMousePosition(const sf::View& view) const;
    sf::Vector2i getMousePositionDelta() const { return m_mousePosition - m_previousMousePosition; }
    sf::Vector2f getMouseWheelDelta() const { return m_mouseWheelDelta; }

    bool detectedMouseButtonPressedEvent() const { return !m_eventPressedMouseButtons.empty(); }
    bool detectedMouseButtonReleasedEvent() const { return !m_eventReleasedMouseButtons.empty(); }
    bool detectedMouseMovedEvent() const { return m_mouseMovedEvent; }
    bool detectedMouseWheelScrolledEvent() const { return m_mouseWheelScrolledEvent; }

    // Joystick getters
    bool isJoystickButtonHeld(unsigned int joystick, unsigned int button) const;
    bool isJoystickButtonDescending(unsigned int joystick, unsigned int button, bool isRepeatEnabled = false) const;
    bool isJoystickButtonAscending(unsigned int joystick, unsigned int button) const;

    bool isJoystickConnected(unsigned int joystick) const;
    sf::Joystick::Identification getJoystickIdentification(unsigned int joystick) const;
    float getJoystickAxisPosition(unsigned int joystick, sf::Joystick::Axis axis) const;

    bool detectedJoystickButtonPressedEvent() const { return !m_eventPressedJoystickButtons.empty(); }
    bool detectedJoystickButtonReleasedEvent() const { return !m_eventReleasedJoystickButtons.empty(); }
    bool detectedJoystickMovedEvent() const { return m_joystickMovedEvent; }
    bool detectedJoystickConnectedEvent() const { return m_joystickConnectedEvent; }
    bool detectedJoystickDisconnectedEvent() const { return m_joystickDisconnectedEvent; }

    // Touch getters
    bool isTouchHeld() const { return m_isTouchHeld; }

    bool detectedTouchBeganEvent() const { return m_touchBeganEvent; }
    bool detectedTouchMovedEvent() const { return m_touchMovedEvent; }
    bool detectedTouchEndedEvent() const { return m_touchEndedEvent; }

    // Sensor getters
    bool detectedSensorChangedEvent() const { return m_sensorChangedEvent; }

    // Clipboard
    void setClipboardText(const sf::String& text);
    sf::String getClipboardText() const;
};

#endif // INPUTMANAGER_H
