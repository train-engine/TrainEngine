// Not necessary: only for vscode to not put errors everywhere.
#include "InputContext.h"

inline InputContext::~InputContext()
{
    for (auto& input : m_actionInputs)
    {
        delete input;
    }

    for (auto& input : m_stateInputs)
    {
        delete input;
    }

    for (auto& input : m_rangeInputs)
    {
        delete input;
    }
}

inline void InputContext::Update()
{
    for (auto& input : m_actionInputs)
    {
        if (input->DetectedEvent())
        {
            input->CallAction();
        }
    }

    for (auto& input : m_stateInputs)
    {
        if (input->DetectedEvent())
        {
            input->CallAction();
        }
    }

    for (auto& input : m_rangeInputs)
    {
        if (input->DetectedEvent())
        {
            input->CallAction();
        }
    }
}

template<typename Callable>
inline void InputContext::BindActionToKey(Callable callback, sf::Keyboard::Key key, EventType eventType)
{
    BindActionToKeyHelper(new CallbackFunctor<Callable>(callback), key, eventType);
}

template<typename Object, typename Callable>
inline void InputContext::BindActionToKey(Object* object, Callable callback, sf::Keyboard::Key key, EventType eventType)
{
    BindActionToKeyHelper(new CallbackMember<Object, Callable>(object, callback), key, eventType);
}

template<typename Callable>
inline void InputContext::BindActionToMouseButton(Callable callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    BindActionToMouseButtonHelper(new CallbackFunctor<Callable>(callback), mouseButton, eventType);
}

template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseButton(Object* object, Callable callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    BindActionToMouseButtonHelper(new CallbackMember<Object, Callable>(object, callback), mouseButton, eventType);
}

template<typename Callable>
inline void InputContext::BindActionToJoystickButton(Callable callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    BindActionToJoystickButtonHelper(new CallbackFunctor<Callable>(callback), joystick, button, eventType);
}

template<typename Object, typename Callable>
inline void InputContext::BindActionToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    BindActionToJoystickButtonHelper(new CallbackMember<Object, Callable>(object, callback), joystick, button, eventType);
}

template<typename Callable>
inline void InputContext::BindActionToMouseMoved(Callable callback)
{
    BindActionToMouseMovedHelper(new CallbackFunctor<Callable>(callback));
}

template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseMoved(Object* object, Callable callback)
{
    BindActionToMouseMovedHelper(new CallbackMember<Object, Callable>(object, callback));
}

template<typename Callable>
inline void InputContext::BindActionToMouseWheelScrolled(Callable callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    BindActionToMouseWheelScrolledHelper(new CallbackFunctor<Callable>(callback), mouseWheelAxis, mouseWheelDirection);
}

template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseWheelScrolled(Object* object, Callable callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    BindActionToMouseWheelScrolledHelper(new CallbackMember<Object, Callable>(object, callback), mouseWheelAxis, mouseWheelDirection);
}

template<typename Callable>
inline void InputContext::BindStateToKey(Callable callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(new KeyEventState(m_inputManager, new CallbackFunctor<Callable, bool>(callback), key));
}

template<typename Object, typename Callable>
inline void InputContext::BindStateToKey(Object* object, Callable callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(new KeyEventState(m_inputManager, new CallbackMember<Object, Callable, bool>(object, callback), key));
}

template<typename Callable>
inline void InputContext::BindStateToMouseButton(Callable callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(new MouseButtonEventState(m_inputManager, new CallbackFunctor<Callable, bool>(callback), button));
}

template<typename Object, typename Callable>
inline void InputContext::BindStateToMouseButton(Object* object, Callable callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(new MouseButtonEventState(m_inputManager, new CallbackMember<Object, Callable, bool>(object, callback), button));
}

template<typename Callable>
inline void InputContext::BindStateToJoyStickButton(Callable callback, unsigned int joystick, unsigned int button)
{
    m_stateInputs.push_back(new JoystickButtonEventState(m_inputManager, new CallbackFunctor<Callable, bool>(callback), joystick, button));
}

template<typename Object, typename Callable>
inline void InputContext::BindStateToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button)
{
    m_stateInputs.push_back(new JoystickButtonEventState(m_inputManager, new CallbackMember<Object, Callable, bool>(object, callback), joystick, button));
}

template<typename Callable>
inline void InputContext::BindRangeToJoystickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis)
{
    m_rangeInputs.push_back(new JoystickAxisRange(m_inputManager, new CallbackFunctor<Callable, double>(callback), joystick, axis));
}

template<typename Object, typename Callable>
inline void InputContext::BindRangeToJoystickAxis(Object* object, Callable callback, unsigned int joystick, sf::Joystick::Axis axis)
{
    m_rangeInputs.push_back(new JoystickAxisRange(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback), joystick, axis));
}

template<typename Callable>
inline void InputContext::BindRangeToMouseScroll(Callable callback, sf::Mouse::Wheel wheelAxis)
{
    BindRangeToMouseScrollHelper(new CallbackFunctor<Callable, double>(callback), wheelAxis);
}

template<typename Object, typename Callable>
inline void InputContext::BindRangeToMouseScroll(Object* object, Callable callback, sf::Mouse::Wheel wheelAxis)
{
    BindRangeToMouseScrollHelper(new CallbackMember<Object, Callable, double>(object, callback), wheelAxis);
}

template<typename Callable>
inline void InputContext::BindRangeToHorizontalMouseMovement(Callable callback)
{
    m_rangeInputs.push_back(new HorizontalMouseMovementRange(m_inputManager, new CallbackFunctor<Callable, double>(callback)));
}

template<typename Object, typename Callable>
inline void InputContext::BindRangeToHorizontalMouseMovement(Object* object, Callable callback)
{
    m_rangeInputs.push_back(new HorizontalMouseMovementRange(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback)));
}

template<typename Callable>
inline void InputContext::BindRangeToVerticalMouseMovement(Callable callback)
{
    m_rangeInputs.push_back(new VerticalMouseMovementRange(m_inputManager, new CallbackFunctor<Callable, double>(callback)));
}

template<typename Object, typename Callable>
inline void InputContext::BindRangeToVerticalMouseMovement(Object* object, Callable callback)
{
    m_rangeInputs.push_back(new VerticalMouseMovementRange(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback)));
}

template<typename Callable>
inline void InputContext::BindRangeToKeyboard(Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.push_back(new KeyboardRange(m_inputManager, new CallbackFunctor<Callable, double>(callback), negativeKey, positiveKey));
}

template<typename Object, typename Callable>
inline void InputContext::BindRangeToKeyboard(Object* object, Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.push_back(new KeyboardRange(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback), negativeKey, positiveKey));
}

inline void InputContext::BindActionToKeyHelper(Callback<>* callback, sf::Keyboard::Key key, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new KeyAscendingAction(m_inputManager, callback, key));
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new KeyDescendingAction(m_inputManager, callback, key));
    }
}

inline void InputContext::BindActionToMouseButtonHelper(Callback<>* callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new MouseButtonAscendingAction(m_inputManager, callback, mouseButton));
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new MouseButtonDescendingAction(m_inputManager, callback, mouseButton));
    }
}

inline void InputContext::BindActionToJoystickButtonHelper(Callback<>* callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new JoystickButtonAscendingAction(m_inputManager, callback, joystick, button));
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new JoystickButtonDescendingAction(m_inputManager, callback, joystick, button));
    }
}

inline void InputContext::BindActionToMouseMovedHelper(Callback<>* callback)
{
    m_actionInputs.push_back(new MouseMovedAction(m_inputManager, callback));
}

inline void InputContext::BindActionToMouseWheelScrolledHelper(Callback<>* callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    switch (mouseWheelAxis)
    {
    case sf::Mouse::VerticalWheel:
        if (mouseWheelDirection == EventType::Ascending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelUpAction(m_inputManager, callback));
        }
        if (mouseWheelDirection == EventType::Descending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelDownAction(m_inputManager, callback));
        }
        break;
    case sf::Mouse::HorizontalWheel:
        if (mouseWheelDirection == EventType::Ascending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelRightAction(m_inputManager, callback));
        }
        if (mouseWheelDirection == EventType::Descending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelLeftAction(m_inputManager, callback));
        }
        break;
    }
}

inline void InputContext::BindRangeToMouseScrollHelper(Callback<double>* callback, sf::Mouse::Wheel wheelAxis)
{
    switch (wheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        m_rangeInputs.push_back(new HorizontalMouseWheelRange(m_inputManager, callback));
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        m_rangeInputs.push_back(new VerticalMouseWheelRange(m_inputManager, callback));
        break;
    }
}