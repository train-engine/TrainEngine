#ifndef INPUTMANAGER_H_INCLUDED
#define INPUTMANAGER_H_INCLUDED

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>

class InputManager
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
    bool m_windowHasFocus;

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
    InputManager(sf::RenderWindow& rWindow);

    // Functions
    void Update();

    // Window getters
    sf::Vector2u GetWindowDimensions() const {return m_rWindow.getSize();}
    bool WindowHasFocus() const {return m_windowHasFocus;}

    const sf::Vector2f MapPixelToCoords(const sf::Vector2i& point, const sf::View& view) const {return m_rWindow.mapPixelToCoords(point, view);}
    const sf::Vector2f MapPixelToCoords(const sf::Vector2i& point) const {return m_rWindow.mapPixelToCoords(point);}
    const sf::Vector2i MapCoordsToPixel(const sf::Vector2f& position, const sf::View& view) const {return m_rWindow.mapCoordsToPixel(position, view);}
    const sf::Vector2i MapCoordsToPixel(const sf::Vector2f& position) const {return m_rWindow.mapCoordsToPixel(position);}

    bool DetectClosedEvent() const {return m_closedEvent;}
    bool DetectResizedEvent() const {return m_resizedEvent;}
    bool DetectLostFocusEvent() const {return m_lostFocusEvent;}
    bool DetectGainedFocusEvent() const {return m_gainedFocusEvent;}
    bool DetectMouseLeftEvent() const {return m_mouseLeftEvent;}
    bool DetectMouseEnteredEvent() const {return m_mouseEnteredEvent;}

    // Keyboard setters
    void SetVirtualKeyboardVisible(bool isVisible) const {sf::Keyboard::setVirtualKeyboardVisible(isVisible);}

    // Keyboard getters
    bool KeyHeld(sf::Keyboard::Key key) const;
    bool KeyDown(sf::Keyboard::Key key, bool enableRepeat = false) const;
    bool KeyUp(sf::Keyboard::Key key) const;

    bool ShiftKeyHeld() const {return (KeyHeld(sf::Keyboard::LShift) || KeyHeld(sf::Keyboard::RShift));}
    bool ShiftKeyDown(bool enableRepeat = false) const {return (KeyDown(sf::Keyboard::LShift, enableRepeat) || KeyDown(sf::Keyboard::RShift, enableRepeat));}
    bool ShiftKeyUp() const {return (KeyUp(sf::Keyboard::LShift) || KeyUp(sf::Keyboard::RShift));}

    bool ControlKeyHeld() const {return (KeyHeld(sf::Keyboard::LControl) || KeyHeld(sf::Keyboard::RControl));}
    bool ControlKeyDown(bool enableRepeat = false) const {return (KeyDown(sf::Keyboard::LControl, enableRepeat) || KeyDown(sf::Keyboard::RControl, enableRepeat));}
    bool ControlKeyUp() const {return (KeyUp(sf::Keyboard::LControl) || KeyUp(sf::Keyboard::RControl));}

    bool AltKeyHeld() const {return (KeyHeld(sf::Keyboard::LAlt) || KeyHeld(sf::Keyboard::RAlt));}
    bool AltKeyDown(bool enableRepeat = false) const {return (KeyDown(sf::Keyboard::LAlt, enableRepeat) || KeyDown(sf::Keyboard::RAlt, enableRepeat));}
    bool AltKeyUp() const {return (KeyUp(sf::Keyboard::LAlt) || KeyUp(sf::Keyboard::RAlt));}

    bool SystemKeyHeld() const {return (KeyHeld(sf::Keyboard::LSystem) || KeyHeld(sf::Keyboard::RSystem));}
    bool SystemKeyDown(bool enableRepeat = false) const {return (KeyDown(sf::Keyboard::LSystem, enableRepeat) || KeyDown(sf::Keyboard::RSystem, enableRepeat));}
    bool SystemKeyUp() const {return (KeyUp(sf::Keyboard::LSystem) || KeyUp(sf::Keyboard::RSystem));}

    bool ModifierKeyHeld() const;
    bool ModifierKeyDown(bool enableRepeat = false) const;
    bool ModifierKeyUp() const;

    const sf::String& GetTextEntered() const {return m_enteredText;}

    bool DetectTextEnteredEvent() const {return !m_enteredText.isEmpty();}
    bool DetectKeyDownEvent() const {return !m_eventPressedKeys.empty();}
    bool DetectKeyUpEvent() const {return !m_eventReleasedKeys.empty();}

    // Mouse getters
    bool MouseButtonHeld(sf::Mouse::Button button) const;
    bool MouseButtonDown(sf::Mouse::Button button, bool enableRepeat = false) const;
    bool MouseButtonUp(sf::Mouse::Button button) const;

    const sf::Vector2i& GetWindowMousePosition() const {return m_mousePosition;}
    sf::Vector2f GetMousePosition(const sf::View& view) const;
    float GetMouseWheelDelta() const {return m_mouseWheelDelta;}

    bool DetectMouseButtonDownEvent() const {return !m_eventPressedMouseButtons.empty();}
    bool DetectMouseButtonUpEvent() const {return !m_eventReleasedMouseButtons.empty();}
    bool DetectMouseMovedEvent() const {return m_mouseMovedEvent;}
    bool DetectMouseWheelScrolledEvent() const {return (m_mouseWheelDelta != 0);}

    // Joystick getters
    bool JoystickButtonHeld(unsigned int joystick, unsigned int button) const;
    bool JoystickButtonDown(unsigned int joystick, unsigned int button, bool enableRepeat = false) const;
    bool JoystickButtonUp(unsigned int joystick, unsigned int button) const;

    bool IsJoystickConnected(unsigned int joystick) const {return sf::Joystick::isConnected(joystick);}
    sf::Joystick::Identification GetJoystickIdentification(unsigned int joystick) const {return sf::Joystick::getIdentification(joystick);}
    float GetJoystickAxisPosition(unsigned int joystick, sf::Joystick::Axis axis) const;

    bool DetectJoystickButtonPressedEvent() const {return !m_eventPressedJoystickButtons.empty();}
    bool DetectJoystickButtonReleasedEvent() const {return !m_eventReleasedJoystickButtons.empty();}
    bool DetectJoystickMovedEvent() const {return m_joystickMovedEvent;}
    bool DetectJoystickConnectedEvent() const {return m_joystickConnectedEvent;}
    bool DetectJoystickDisconnectedEvent() const {return m_joystickDisconnectedEvent;}

    // Touch getters
    bool TouchHeld() const {return m_isTouchHeld;}

    bool DetectTouchBeganEvent() const {return m_touchBeganEvent;}
    bool DetectTouchMovedEvent() const {return m_touchMovedEvent;}
    bool DetectTouchEndedEvent() const {return m_touchEndedEvent;}

    // Sensor getters
    bool DetectSensorChangedEvent() const {return m_sensorChangedEvent;}

    // Clipboard
    void SetClipboardText(const sf::String& text);
    sf::String GetClipboardText();
};

#endif // INPUTMANAGER_H_INCLUDED
