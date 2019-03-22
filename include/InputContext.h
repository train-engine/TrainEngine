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

class ActionInput
{
public:

    ActionInput(void (*const callBack)(), ActionInputBinds event, int inputId = 0);

    void CheckIsTriggered(const InputManager& inputManager) const;

    ActionInputBinds GetEventType() const {return m_event;}
    int GetInputId() const {return m_inputId;}

private:
    void (*const m_callBack)();
    ActionInputBinds m_event;
    int m_inputId;
};

enum class StateInputBinds
{
    KeyEvent = 0,
    MouseButtonEvent,
    JoystickButtonEvent
};

class StateInput
{
public:

    StateInput(void (*const callBack)(bool), StateInputBinds event, int inputId = 0);

    void CheckIsTriggered(const InputManager& inputManager) const;

    StateInputBinds GetEventType() const {return m_event;}
    int GetInputId() const {return m_inputId;}

private:
    void (*const m_callBack)(bool);
    StateInputBinds m_event;
    int m_inputId;
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

class RangeInput
{
public:

    RangeInput(void (*const callBack)(float), RangeInputBinds event, sf::Joystick::Axis joystickAxis = sf::Joystick::Axis::X);
    RangeInput(void (*const callBack)(float), sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);

    void CheckIsTriggered(const InputManager& inputManager) const;

    RangeInputBinds GetEventType() const {return m_event;}
    sf::Keyboard::Key GetNegativeKey() const {return m_negativeKey;}
    sf::Keyboard::Key GetPositiveKey() const {return m_positiveKey;}
    sf::Joystick::Axis GetJoystickAxis() const {return m_joystickAxis;}

private:
    void (*const m_callBack)(float);
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
    void BindActionToJoystickButton(void (*const callBack)(), int joystick, EventType eventType);
    void BindActionToMouseMoved(void (*const callBack)());
    void BindActionToMouseWheel(void (*const callBack)(), sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);

    // InputState
    void BindStateToKey(void (*const callBack)(bool), sf::Keyboard::Key key);
    void BindStateToMouseButton(void (*const callBack)(bool), sf::Mouse::Button button);
    void BindStateToJoystickButton(void (*const callBack)(bool), int joystickId);

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
    std::vector<ActionInput> m_actionInputs;
    std::vector<StateInput> m_stateInputs;
    std::vector<RangeInput> m_rangeInputs;
};

#endif // INPUTCONTEXT_H
