#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include "InputManager.h"

enum class ActionInputBinds
{
    KeyDescending = 0,
    KeyAscending,
    MouseButtonDescending,
    MouseButtonAscending,
    JoystickButtonDescending,
    JoystickButtonAscending,
    MouseMoved,
    MouseWheelUp,
    MouseWheelDown,
    MouseWheelLeft,
    MouseWheelRight
};

template<typename T>
class ActionInput
{
public:

    ActionInput(T callback, ActionInputBinds event, int inputId = 0);

    void CheckIsTriggered(const InputManager& inputManager) const;

    ActionInputBinds GetEventType() const {return m_event;}
    unsigned int GetInputId() const {return m_inputId;}

private:
    T m_callback;
    ActionInputBinds m_event;
    unsigned int m_inputId;
};

enum class StateInputBinds
{
    KeyEvent = 0,
    MouseButtonEvent,
    JoystickButtonEvent
};

template<typename T>
class StateInput
{
public:

    StateInput(T callback, StateInputBinds event, int inputId = 0);

    void CheckIsTriggered(const InputManager& inputManager) const;

    StateInputBinds GetEventType() const {return m_event;}
    unsigned int GetInputId() const {return m_inputId;}

private:
    T m_callback;
    StateInputBinds m_event;
    unsigned int m_inputId;
};

enum class RangeInputBinds
{
    JoystickAxis,
    VerticalMouseWheel,
    HorizontalMouseWheel,
    VertialMouseMovement,
    HorizontalMouseMovement,
    Keyboard
};

template<typename T>
class RangeInput
{
public:

    RangeInput(T callback, RangeInputBinds event, sf::Joystick::Axis joystickAxis = sf::Joystick::Axis::X);
    RangeInput(T callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);

    void CheckIsTriggered(const InputManager& inputManager) const;

    RangeInputBinds GetEventType() const {return m_event;}
    sf::Keyboard::Key GetNegativeKey() const {return m_negativeKey;}
    sf::Keyboard::Key GetPositiveKey() const {return m_positiveKey;}
    sf::Joystick::Axis GetJoystickAxis() const {return m_joystickAxis;}

private:
    T m_callback;
    RangeInputBinds m_event;

    sf::Joystick::Axis m_joystickAxis;

    sf::Keyboard::Key m_negativeKey;
    sf::Keyboard::Key m_positiveKey;
};

enum class EventType
{
    Ascending,
    Descending,
    Any
};

class InputContext
{
public:

    // Functions

    // InputAction
    void BindActionToKey(void (*const callBack)(), sf::Keyboard::Key key, EventType eventType);
    void BindActionToMouseButton(void (*const callBack)(), sf::Mouse::Button mouseButton, EventType eventType);
    void BindActionToJoystickButton(void (*const callBack)(), unsigned int joystickButton, EventType eventType);
    void BindActionToMouseMoved(void (*const callBack)());
    void BindActionToMouseWheel(void (*const callBack)(), sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);

    // InputState
    void BindStateToKey(void (*const callBack)(bool), sf::Keyboard::Key key);
    void BindStateToMouseButton(void (*const callBack)(bool), sf::Mouse::Button button);
    void BindStateToJoystickButton(void (*const callBack)(bool), unsigned int joystickButton);

    // InputRange
    void BindRangeToJoystick(void (*const callBack)(float), sf::Joystick::Axis joystickAxis);
    void BindRangeToMouseScroll(void (*const callBack)(float), sf::Mouse::Wheel wheelAxis);
    void BindRangeToHorizontalMouseMoved(void (*const callBack)(float));
    void BindRangeToVerticalMouseMoved(void (*const callBack)(float));
    void BindRangeToKeyboard(void (*const callBack)(float), sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);

    void LoadFromFile(const std::string& filename) = delete; // TODO
    void Clear();

    // Getters
    bool IsKeyUsed(sf::Keyboard::Key key) const;
    bool IsMouseButtonUsed(sf::Mouse::Button mouseButton) const;
    bool IsJoystickAxisUsed(sf::Joystick::Axis joystickAxis) const;
    bool IsJoystickButtonUsed(unsigned int joystickButton) const;
    bool IsMouseWheelUsed(sf::Mouse::Wheel mouseWheelAxis, EventType eventType = EventType::Any) const;
    bool IsMouseMovementUsed() const;

    void Update(const InputManager& inputManager);
private:

    // Attributes
    std::vector<ActionInput<void (*)()>> m_actionInputs;
    std::vector<StateInput<void (*)(bool)>> m_stateInputs;
    std::vector<RangeInput<void (*)(float)>> m_rangeInputs;
};

#include "InputContext.inl"

#endif // INPUTCONTEXT_H
