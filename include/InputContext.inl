// Not necessary: only for vscode to not put errors everywhere.
#include "InputContext.h"

/// Destructor
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

/// Update function which check if callbacks must be called. This function must be called periodically for the InputContext to work.
/// The best place to call this function is in a HandleInput function.
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

/// Assign a key action to a function with no parameters.
/// \param callback     A function with no parameters.
/// \param key          The key bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Callable>
inline void InputContext::BindActionToKey(Callable callback, sf::Keyboard::Key key, EventType eventType)
{
    BindActionToKeyHelper(new CallbackFunctor<Callable>(callback), key, eventType);
}

/// Assign a key action to a function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
/// \param key          The key bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Object, typename Callable>
inline void InputContext::BindActionToKey(Object* object, Callable callback, sf::Keyboard::Key key, EventType eventType)
{
    BindActionToKeyHelper(new CallbackMember<Object, Callable>(object, callback), key, eventType);
}

/// Assign a mouse button action to a function with no parameters.
/// \param callback     A function with no parameters.
/// \param mouseButton  The mouse button bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Callable>
inline void InputContext::BindActionToMouseButton(Callable callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    BindActionToMouseButtonHelper(new CallbackFunctor<Callable>(callback), mouseButton, eventType);
}

/// Assign a mouse button action to a member function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
/// \param mouseButton  The mouse button bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseButton(Object* object, Callable callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    BindActionToMouseButtonHelper(new CallbackMember<Object, Callable>(object, callback), mouseButton, eventType);
}

/// Assign a joystick button action to a function with no parameters.
/// \param callback         A function with no parameters.
/// \param joystickButton   The joystick button bound to the action.
/// \param eventType        The type of event for the action binding.
template<typename Callable>
inline void InputContext::BindActionToJoystickButton(Callable callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    BindActionToJoystickButtonHelper(new CallbackFunctor<Callable>(callback), joystick, button, eventType);
}

/// Assign a joystick button action to a member function with no parameters.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with no parameters.
/// \param joystickButton   The joystick button bound to the action.
/// \param eventType        The type of event for the action binding.
template<typename Object, typename Callable>
inline void InputContext::BindActionToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    BindActionToJoystickButtonHelper(new CallbackMember<Object, Callable>(object, callback), joystick, button, eventType);
}

/// Assign the mouse movement to a function with no parameters.
/// \param callback     A function with no parameters.
/// \param mouseButton  The joystick button bound to the action.
template<typename Callable>
inline void InputContext::BindActionToMouseMoved(Callable callback)
{
    BindActionToMouseMovedHelper(new CallbackFunctor<Callable>(callback));
}

/// Assign the mouse movement to a member function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
/// \param mouseButton  The joystick button bound to the action.
template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseMoved(Object* object, Callable callback)
{
    BindActionToMouseMovedHelper(new CallbackMember<Object, Callable>(object, callback));
}

/// Assign a mouse wheel scrolled to a function with no parameters.
/// \param callback             A function with no parameters.
/// \param mouseWheelAxis       The wheel Axis bound to the callback.
/// \param mouseWheelDirection  The wheelDirection bound to the callback.
template<typename Callable>
inline void InputContext::BindActionToMouseWheelScrolled(Callable callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    BindActionToMouseWheelScrolledHelper(new CallbackFunctor<Callable>(callback), mouseWheelAxis, mouseWheelDirection);
}

/// Assign a mouse wheel scrolled to a function with no parameters.
/// \param object               The object on which the member function must be called.
/// \param callback             A member function with no parameters.
/// \param mouseWheelAxis       The wheel Axis bound to the callback.
/// \param mouseWheelDirection  The wheelDirection bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseWheelScrolled(Object* object, Callable callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    BindActionToMouseWheelScrolledHelper(new CallbackMember<Object, Callable>(object, callback), mouseWheelAxis, mouseWheelDirection);
}

/// Assign a key event to a function with a bool as a parameter.
/// The function will receive a bool representing if the key is pressed.
/// \param callback     A function with a bool as a parameter.
/// \param key          The key bound to the callback.
template<typename Callable>
inline void InputContext::BindStateToKey(Callable callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(new KeyEventStateInput(m_inputManager, new CallbackFunctor<Callable, bool>(callback), key));
}


/// Assign a key event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the key is pressed.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a bool as a parameter.
/// \param key          The key bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindStateToKey(Object* object, Callable callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(new KeyEventStateInput(m_inputManager, new CallbackMember<Object, Callable, bool>(object, callback), key));
}

/// Assign a mouse button event to a with a bool as a parameter.
/// The function will receive a bool representing if the mouse button is pressed.
/// \param callback     A function with a bool as a parameter.
/// \param button       The mouse button bound to the callback.
template<typename Callable>
inline void InputContext::BindStateToMouseButton(Callable callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(new MouseButtonEventStateInput(m_inputManager, new CallbackFunctor<Callable, bool>(callback), button));
}

/// Assign a mouse button event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the mouse button is pressed.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a bool as a parameter.
/// \param button       The mouse button bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindStateToMouseButton(Object* object, Callable callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(new MouseButtonEventStateInput(m_inputManager, new CallbackMember<Object, Callable, bool>(object, callback), button));
}

/// Assign a joystick button event to a function with a bool as a parameter.
/// The function will receive a bool representing if the joystick button is pressed.
/// \param callback         A function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param joystickButton   The joystick button bound to the callback.
template<typename Callable>
inline void InputContext::BindStateToJoyStickButton(Callable callback, unsigned int joystick, unsigned int button)
{
    m_stateInputs.push_back(new JoystickButtonEventStateInput(m_inputManager, new CallbackFunctor<Callable, bool>(callback), joystick, button));
}

/// Assign a joystick button event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick button is pressed.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param joystickButton   The joystick button bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindStateToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button)
{
    m_stateInputs.push_back(new JoystickButtonEventStateInput(m_inputManager, new CallbackMember<Object, Callable, bool>(object, callback), joystick, button));
}

/// Assign a joystick axis position to a function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick axis passed the treshold.
/// Note: If the treshold is bigger or equal to zero, the function will receive true if the axis position is bigger than the treshold.
/// If the treshold is smaller than zero, the function will receive true if the axis position is smaller than the treshold.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param joystickAxis     The joystick axis bound to the callback.
/// \param treshold         The treshold to be exceeded for the callback to be called.
///                         If the treshold is negative, the axis value must be lower for the input to be triggered.
///                         If the treshold is positive or zero, the axis value must be higher for the input to be triggered.
template<typename Callable>
void InputContext::BindStateToJoyStickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis, float treshold)
{
    m_stateInputs.push_back(new JoystickAxisStateInput(m_inputManager, new CallbackFunctor<Callable, bool>(callback), joystick, axis, treshold));
}

/// Assign a joystick axis position to a function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick axis passed the treshold.
/// Note: If the treshold is bigger or equal to zero, the function will receive true if the axis position is bigger than the treshold.
/// If the treshold is smaller than zero, the function will receive true if the axis position is smaller than the treshold.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param joystickAxis     The joystick axis bound to the callback.
/// \param treshold         The treshold to be exceeded for the callback to be called.
///                         If the treshold is negative, the axis value must be lower for the input to be triggered.
///                         If the treshold is positive or zero, the axis value must be higher for the input to be triggered.
template<typename Object, typename Callable>
void InputContext::BindStateToJoystickAxis(Object* object, Callable callback,  unsigned int joystick, sf::Joystick::Axis axis, float treshold)
{
    m_stateInputs.push_back(new JoystickAxisStateInput(m_inputManager, new CallbackMember<Object, Callable, bool>(object, callback), joystick, axis, treshold));
}

/// Assign a joystick axis to a function with a float as a parameter.
/// The function will receive the joystick axis' position as an argument.
/// Note: The member function will receive an argument ranging in [-100, 100]
/// \param callback     A function with a float as a parameter.
/// \param joystick     The joystick id.
/// \param joystickAxis The joystick axis bound to the callback.
template<typename Callable>
inline void InputContext::BindRangeToJoystickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis)
{
    m_rangeInputs.push_back(new JoystickAxisRangeInput(m_inputManager, new CallbackFunctor<Callable, double>(callback), joystick, axis));
}

/// Assign a joystick axis to a function with a float as a parameter.
/// The member function will receive the joystick axis' position as an argument.
/// Note: The member function will receive an argument ranging in [-100, 100]
/// \param object       The object on which the member function must be called.
/// \param callback     A function with a float as a parameter.
/// \param joystick     The joystick bound to the callback.
/// \param joystickAxis The joystick axis bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToJoystickAxis(Object* object, Callable callback, unsigned int joystick, sf::Joystick::Axis axis)
{
    m_rangeInputs.push_back(new JoystickAxisRangeInput(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback), joystick, axis));
}

/// Assign a mouse wheel axis to a function with a float as a parameter.
/// The function will receive the mouse scroll as an argument.
/// \param callback     A function with a float as a parameter.
/// \param wheelAxis    The mouse wheel axis bound to the callback.
template<typename Callable>
inline void InputContext::BindRangeToMouseScroll(Callable callback, sf::Mouse::Wheel wheelAxis)
{
    BindRangeToMouseScrollHelper(new CallbackFunctor<Callable, double>(callback), wheelAxis);
}

/// Assign a mouse wheel axis to a member function with a float as a parameter.
/// The member function will receive the mouse scroll as an argument.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a float as a parameter.
/// \param wheelAxis    The mouse wheel axis bound to the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToMouseScroll(Object* object, Callable callback, sf::Mouse::Wheel wheelAxis)
{
    BindRangeToMouseScrollHelper(new CallbackMember<Object, Callable, double>(object, callback), wheelAxis);
}

/// Assign the horizontal mouse movement to a function with a float as a parameter.
/// The function will receive the horizontal mouse movement as an argument.
/// \param callback     A function with a float as a parameter.
template<typename Callable>
inline void InputContext::BindRangeToHorizontalMouseMovement(Callable callback)
{
    m_rangeInputs.push_back(new HorizontalMouseMovementRangeInput(m_inputManager, new CallbackFunctor<Callable, double>(callback)));
}

/// Assign the horizontal mouse movement to a member function with a float as a parameter.
/// The member function will receive the horizontal mouse movement as an argument.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a float as a parameter.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToHorizontalMouseMovement(Object* object, Callable callback)
{
    m_rangeInputs.push_back(new HorizontalMouseMovementRangeInput(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback)));
}

/// Assign the vertical mouse movement to a function with a float as a parameter.
/// The function will receive the vertical mouse movement as an argument.
/// \param callback     A member function with a float as a parameter.
template<typename Callable>
inline void InputContext::BindRangeToVerticalMouseMovement(Callable callback)
{
    m_rangeInputs.push_back(new VerticalMouseMovementRangeInput(m_inputManager, new CallbackFunctor<Callable, double>(callback)));
}

/// Assign the vertical mouse movement to a member function with a float as a parameter.
/// The function will receive the vertical mouse movement as an argument.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a float as a parameter.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToVerticalMouseMovement(Object* object, Callable callback)
{
    m_rangeInputs.push_back(new VerticalMouseMovementRangeInput(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback)));
}

/// Assign two keys to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument ranging in [-100, 100]
/// \param callback     A function with a float as a parameter.
/// \param negativeKey  The key which sends a negative float to the function.
/// \param positiveKey  The key which sends a positive float to the function.
template<typename Callable>
inline void InputContext::BindRangeToKeyboard(Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.push_back(new KeyboardRangeInput(m_inputManager, new CallbackFunctor<Callable, double>(callback), negativeKey, positiveKey));
}

/// Assign two keys to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument ranging in [-100, 100]
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a float as a parameter.
/// \param negativeKey  The key which sends a negative float to the member function.
/// \param positiveKey  The key which sends a positive float to the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToKeyboard(Object* object, Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.push_back(new KeyboardRangeInput(m_inputManager, new CallbackMember<Object, Callable, double>(object, callback), negativeKey, positiveKey));
}

inline void InputContext::BindActionToKeyHelper(Callback<>* callback, sf::Keyboard::Key key, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new KeyAscendingActionInput(m_inputManager, callback, key));
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new KeyDescendingActionInput(m_inputManager, callback, key));
    }
}

inline void InputContext::BindActionToMouseButtonHelper(Callback<>* callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new MouseButtonAscendingActionInput(m_inputManager, callback, mouseButton));
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new MouseButtonDescendingActionInput(m_inputManager, callback, mouseButton));
    }
}

inline void InputContext::BindActionToJoystickButtonHelper(Callback<>* callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new JoystickButtonAscendingActionInput(m_inputManager, callback, joystick, button));
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(new JoystickButtonDescendingActionInput(m_inputManager, callback, joystick, button));
    }
}

inline void InputContext::BindActionToMouseMovedHelper(Callback<>* callback)
{
    m_actionInputs.push_back(new MouseMovedActionInput(m_inputManager, callback));
}

inline void InputContext::BindActionToMouseWheelScrolledHelper(Callback<>* callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    switch (mouseWheelAxis)
    {
    case sf::Mouse::VerticalWheel:
        if (mouseWheelDirection == EventType::Ascending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelUpActionInput(m_inputManager, callback));
        }
        if (mouseWheelDirection == EventType::Descending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelDownActionInput(m_inputManager, callback));
        }
        break;
    case sf::Mouse::HorizontalWheel:
        if (mouseWheelDirection == EventType::Ascending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelRightActionInput(m_inputManager, callback));
        }
        if (mouseWheelDirection == EventType::Descending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(new MouseWheelLeftActionInput(m_inputManager, callback));
        }
        break;
    }
}

inline void InputContext::BindRangeToMouseScrollHelper(Callback<double>* callback, sf::Mouse::Wheel wheelAxis)
{
    switch (wheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        m_rangeInputs.push_back(new HorizontalMouseWheelRangeInput(m_inputManager, callback));
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        m_rangeInputs.push_back(new VerticalMouseWheelRangeInput(m_inputManager, callback));
        break;
    }
}