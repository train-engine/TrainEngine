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
            input->CallFunction();
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
            input->CallFunction();
        }
    }
}

/// Clear every input binding
inline void InputContext::Clear()
{
    for (auto& input : m_actionInputs)
    {
        delete input;
    }
    m_actionInputs.resize(0);

    for (auto& input : m_stateInputs)
    {
        delete input;
    }
    m_stateInputs.resize(0);

    for (auto& input : m_rangeInputs)
    {
        delete input;
    }
    m_rangeInputs.resize(0);
}

/// Assign a key action to a function with no parameters.
/// \param callback     A function with no parameters.
/// \param key          The key bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Callable>
inline void InputContext::BindActionToKey(Callable callback, sf::Keyboard::Key key, EventType eventType)
{
    BindActionToKeyHelper(
        new CallbackFunctor<Callable>(callback),
        key,
        eventType
    );
}

/// Assign a key action to a function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
/// \param key          The key bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Object, typename Callable>
inline void InputContext::BindActionToKey(Object* object, Callable callback, sf::Keyboard::Key key, EventType eventType)
{
    BindActionToKeyHelper(
        new CallbackMember<Object, Callable>(object, callback),
        key,
        eventType
    );
}

/// Assign a mouse button action to a function with no parameters.
/// \param callback     A function with no parameters.
/// \param mouseButton  The mouse button bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Callable>
inline void InputContext::BindActionToMouseButton(Callable callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    BindActionToMouseButtonHelper(
        new CallbackFunctor<Callable>(callback),
        mouseButton,
        eventType
    );
}

/// Assign a mouse button action to a member function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
/// \param mouseButton  The mouse button bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseButton(Object* object, Callable callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    BindActionToMouseButtonHelper(
        new CallbackMember<Object, Callable>(object, callback),
        mouseButton,
        eventType
    );
}

/// Assign a joystick button action to a function with no parameters.
/// \param callback         A function with no parameters.
/// \param joystickButton   The joystick button bound to the action.
/// \param eventType        The type of event for the action binding.
template<typename Callable>
inline void InputContext::BindActionToJoystickButton(Callable callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    BindActionToJoystickButtonHelper(
        new CallbackFunctor<Callable>(callback),
        joystick,
        button,
        eventType
    );
}

/// Assign a joystick button action to a member function with no parameters.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with no parameters.
/// \param joystickButton   The joystick button bound to the action.
/// \param eventType        The type of event for the action binding.
template<typename Object, typename Callable>
inline void InputContext::BindActionToJoystickButton(Object* object, Callable callback, unsigned int joystick,
                                                     unsigned int button, EventType eventType)
{
    BindActionToJoystickButtonHelper(
        new CallbackMember<Object, Callable>(object, callback),
        joystick,
        button,
        eventType
    );
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
inline void InputContext::BindActionToMouseWheelScrolled(Callable callback, sf::Mouse::Wheel mouseWheelAxis,
                                                         EventType mouseWheelDirection)
{
    BindActionToMouseWheelScrolledHelper(
        new CallbackFunctor<Callable>(callback),
        mouseWheelAxis,
        mouseWheelDirection
    );
}

/// Assign a mouse wheel scrolled to a function with no parameters.
/// \param object               The object on which the member function must be called.
/// \param callback             A member function with no parameters.
/// \param mouseWheelAxis       The wheel Axis bound to the callback.
/// \param mouseWheelDirection  The wheelDirection bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindActionToMouseWheelScrolled(Object* object, Callable callback, sf::Mouse::Wheel mouseWheelAxis,
                                                         EventType mouseWheelDirection)
{
    BindActionToMouseWheelScrolledHelper(
        new CallbackMember<Object, Callable>(object, callback),
        mouseWheelAxis,
        mouseWheelDirection
    );
}

/// Assign a key event to a function with a bool as a parameter.
/// The function will receive a bool representing if the key is pressed.
/// \param callback     A function with a bool as a parameter.
/// \param key          The key bound to the callback.
template<typename Callable>
inline void InputContext::BindStateToKey(Callable callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(
        new KeyEventStateInput(
            m_inputManager,
            new CallbackFunctor<Callable, bool>(callback),
            key
        )
    );
}

/// Assign a key event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the key is pressed.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a bool as a parameter.
/// \param key          The key bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindStateToKey(Object* object, Callable callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(
        new KeyEventStateInput(
            m_inputManager,
            new CallbackMember<Object, Callable, bool>(object, callback),
            key
        )
    );
}

/// Assign a mouse button event to a with a bool as a parameter.
/// The function will receive a bool representing if the mouse button is pressed.
/// \param callback     A function with a bool as a parameter.
/// \param button       The mouse button bound to the callback.
template<typename Callable>
inline void InputContext::BindStateToMouseButton(Callable callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(
        new MouseButtonEventStateInput(
            m_inputManager,
            new CallbackFunctor<Callable, bool>(callback),
            button
        )
    );
}

/// Assign a mouse button event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the mouse button is pressed.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a bool as a parameter.
/// \param button       The mouse button bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindStateToMouseButton(Object* object, Callable callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(
        new MouseButtonEventStateInput(
            m_inputManager,
            new CallbackMember<Object, Callable, bool>(object, callback),
            button
        )
    );
}

/// Assign a joystick button event to a function with a bool as a parameter.
/// The function will receive a bool representing if the joystick button is pressed.
/// \param callback         A function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param joystickButton   The joystick button bound to the callback.
template<typename Callable>
inline void InputContext::BindStateToJoyStickButton(Callable callback, unsigned int joystick, unsigned int button)
{
    m_stateInputs.push_back(
        new JoystickButtonEventStateInput(
            m_inputManager,
            new CallbackFunctor<Callable, bool>(callback),
            joystick,
            button
        )
    );
}

/// Assign a joystick button event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick button is pressed.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param joystickButton   The joystick button bound to the callback.
template<typename Object, typename Callable>
inline void InputContext::BindStateToJoystickButton(Object* object, Callable callback, unsigned int joystick,
                                                    unsigned int button)
{
    m_stateInputs.push_back(
        new JoystickButtonEventStateInput(
            m_inputManager,
            new CallbackMember<Object, Callable, bool>(object, callback),
            joystick,
            button
        )
    );
}

/// Assign a joystick axis position to a function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick axis passed the treshold.
/// Note: If the treshold is bigger or equal to zero, the function will receive true if the axis position is bigger than the treshold.
/// If the treshold is smaller than zero, the function will receive true if the axis position is smaller than the treshold.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param joystickAxis     The joystick axis bound to the callback.
/// \param treshold         The treshold to be exceeded for the callback to be called.
/// \param axisPosition     The position of the axis relative to the treshold.
template<typename Callable>
void InputContext::BindStateToJoystickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis, float treshold,
                                           JoystickAxisPosition axisPosition)
{
    BindStateToJoystickAxisHelper(
        new CallbackFunctor<Callable, bool>(callback),
        joystick,
        axis,
        treshold,
        axisPosition
    );
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
/// \param axisPosition     The position of the axis relative to the treshold.
template<typename Object, typename Callable>
void InputContext::BindStateToJoystickAxis(Object* object, Callable callback,  unsigned int joystick, sf::Joystick::Axis axis, float treshold,
                                           JoystickAxisPosition axisPosition)
{
    BindStateToJoystickAxisHelper(
        new CallbackMember<Object, Callable, bool>(object, callback),
        joystick,
        axis,
        treshold,
        axisPosition
    );
}

/// Assign a joystick axis to a function with a float as a parameter.
/// The function will receive the joystick axis' position as an argument.
/// Note: The function will receive an argument ranging in [0, 100] or [-100, 0] depending on the rangeRestriction.
/// \param callback         A function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param joystickAxis     The joystick axis bound to the callback.
/// \param rangeRestriction The restriction of the callback range of the joystick axis.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the axis.
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of the axis.
template<typename Callable>
inline void InputContext::BindRangeToJoystickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis,
                                                  RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new JoystickAxisBidirectionalRangeInput(
                m_inputManager,
                new CallbackFunctor<Callable, float>(callback),
                joystick,
                axis
            )
        );
    }
    else
    {
        m_rangeInputs.push_back(
            new JoystickAxisUnidirectionalRangeInput(
                m_inputManager,
                new CallbackFunctor<Callable, float>(callback),
                joystick,
                axis,
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
    }
}

/// Assign a joystick axis to a member function with a float as a parameter.
/// The member function will receive the joystick axis' position as an argument.
/// Note: The member function will receive an argument ranging in [0, 100] or [-100, 0] depending on the rangeRestriction.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param joystickAxis     The joystick axis bound to the callback.
/// \param rangeRestriction The restriction of the callback range of the joystick axis.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the axis.
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of the axis.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToJoystickAxis(Object* object, Callable callback, unsigned int joystick, sf::Joystick::Axis axis,
                                                  RangeRestriction rangeRestriction)
{
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new JoystickAxisBidirectionalRangeInput(
                m_inputManager,
                new CallbackMember<Object, Callable, float>(object, callback),
                joystick,
                axis
            )
        );
    }
    else
    {
        m_rangeInputs.push_back(
            new JoystickAxisUnidirectionalRangeInput(
                m_inputManager,
                new CallbackMember<Object, Callable, float>(object, callback),
                joystick,
                axis,
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
    }
}

/// Assign a mouse wheel axis to a function with a float as a parameter.
/// The function will receive the mouse scroll as an argument.
/// \param callback         A function with a float as a parameter.
/// \param wheelAxis        The mouse wheel axis bound to the callback.
/// \param rangeRestriction The restriction of the callback range of the mouse scoll.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of scroll
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of scroll
template<typename Callable>
inline void InputContext::BindRangeToMouseScroll(Callable callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        BindRangeToMouseScrollHelper(
            new CallbackFunctor<Callable, float>(callback),
            wheelAxis
        );
    }
    else
    {
        BindRangeToMouseScrollHelper(
            new CallbackFunctor<Callable, float>(callback),
            wheelAxis,
            rangeRestriction
        );
    }
}

/// Assign a mouse wheel axis to a member function with a float as a parameter.
/// The member function will receive the mouse scroll as an argument.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param wheelAxis        The mouse wheel axis bound to the member function.
/// \param rangeRestriction The restriction of the callback range of the mouse scoll.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of scroll
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of scroll
template<typename Object, typename Callable>
inline void InputContext::BindRangeToMouseScroll(Object* object, Callable callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        BindRangeToMouseScrollHelper(
            new CallbackMember<Object, Callable, float>(object, callback),
            wheelAxis
        );
    }
    else
    {
        BindRangeToMouseScrollHelper(
            new CallbackMember<Object, Callable, float>(object, callback),
            wheelAxis,
            rangeRestriction
        );
    }
}

/// Assign the horizontal mouse movement to a function with a float as a parameter.
/// The function will receive the horizontal mouse movement as an argument.
/// \param callback         A function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the horizontal mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the mouse movement 
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of the mouse movement
template<typename Callable>
inline void InputContext::BindRangeToHorizontalMouseMovement(Callable callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new HorizontalMouseMovementBidirectionalRangeInput(
                m_inputManager,
                new CallbackFunctor<Callable, float>(callback)
            )
        );
    }
    else
    {
        m_rangeInputs.push_back(
            new HorizontalMouseMovementUnidirectionalRangeInput(
                m_inputManager,
                new CallbackFunctor<Callable, float>(callback),
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
    }
}

/// Assign the horizontal mouse movement to a member function with a float as a parameter.
/// The member function will receive the horizontal mouse movement as an argument.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the horizontal mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the mouse movement
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of the mouse movement
template<typename Object, typename Callable>
inline void InputContext::BindRangeToHorizontalMouseMovement(Object* object, Callable callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new HorizontalMouseMovementBidirectionalRangeInput(
                m_inputManager,
                new CallbackMember<Object, Callable, float>(object, callback)
            )
        );
    }
    else
    {
        m_rangeInputs.push_back(
            new HorizontalMouseMovementUnidirectionalRangeInput(
                m_inputManager,
                new CallbackMember<Object, Callable, float>(object, callback),
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
    }
}

/// Assign the vertical mouse movement to a function with a float as a parameter.
/// The function will receive the vertical mouse movement as an argument.
/// \param callback         A function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the vertical mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the mouse movement 
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of the mouse movement
template<typename Callable>
inline void InputContext::BindRangeToVerticalMouseMovement(Callable callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new VerticalMouseMovementBidirectionalRangeInput(
                m_inputManager,
                new CallbackFunctor<Callable, float>(callback)
            )
        );
    }
    else
    {
        m_rangeInputs.push_back(
            new VerticalMouseMovementUnidirectionalRangeInput(
                m_inputManager,
                new CallbackFunctor<Callable, float>(callback),
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
    }
}

/// Assign the vertical mouse movement to a member function with a float as a parameter.
/// The member function will receive the vertical mouse movement as an argument.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the vertical mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the mouse movement 
///                         If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values of the mouse movement
template<typename Object, typename Callable>
inline void InputContext::BindRangeToVerticalMouseMovement(Object* object, Callable callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new VerticalMouseMovementBidirectionalRangeInput(
                m_inputManager,
                new CallbackMember<Object, Callable, float>(object, callback)
            )
        );
    }
    else
    {
        m_rangeInputs.push_back(
            new VerticalMouseMovementUnidirectionalRangeInput(
                m_inputManager,
                new CallbackMember<Object, Callable, float>(object, callback),
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
    }
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
    m_rangeInputs.push_back(
        new KeyboardBidirectionalRangeInput(
            m_inputManager,
            new CallbackFunctor<Callable, float>(callback),
            negativeKey,
            positiveKey
        )
    );
}

/// Assign two keys to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument ranging in [-100, 100]
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a float as a parameter.
/// \param negativeKey  The key which sends a negative float to the member function.
/// \param positiveKey  The key which sends a positive float to the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToKeyboard(Object* object, Callable callback,
                                              sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.push_back(
        new KeyboardBidirectionalRangeInput(
            m_inputManager,
            new CallbackMember<Object, Callable, float>(object, callback),
            negativeKey,
            positiveKey
        )
    );
}

/// Assign a key to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument ranging in [0, 100]
/// \param callback         A function with a float as a parameter.
/// \param key              The key which triggers the function.
template<typename Callable>
inline void InputContext::BindRangeToKeyboard(Callable callback, sf::Keyboard::Key key)
{
    m_rangeInputs.push_back(
        new KeyboardUnidirectionalRangeInput(
            m_inputManager,
            new CallbackFunctor<Callable, float>(callback),
            key
        )
    );
}

/// Assign key to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument ranging in [0, 100]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param key              The key which triggers the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToKeyboard(Object* object, Callable callback, sf::Keyboard::Key key)
{
    m_rangeInputs.push_back(
        new KeyboardUnidirectionalRangeInput(
            m_inputManager,
            new CallbackMember<Object, Callable, float>(object, callback),
            key
        )
    );
}

/// Assign two joystick buttons to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument ranging in [-100, 100]
/// \param callback         A function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param negativeButton   The joystick button which sends a negative float to the function.
/// \param positiveButton   The joystick button which sends a positive float to the function.
template<typename Callable>
inline void InputContext::BindRangeToJoystickButtons(Callable callback, unsigned int joystick, unsigned int negativeButton, unsigned int positiveButton)
{
    m_rangeInputs.push_back(
        new JoystickButtonBidirectionalRangeInput(
            m_inputManager,
            new CallbackFunctor<Callable, float>(callback),
            joystick,
            negativeButton,
            positiveButton
        )
    );
}

/// Assign two joystick buttons to a member function with a float as a parameter.
/// The member function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument ranging in [-100, 100]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param negativeButton   The joystick button which sends a negative float to the member function.
/// \param positiveButton   The joystick button which sends a positive float to the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToJoystickButtons(Object* object, Callable callback, unsigned int joystick, unsigned int positiveButton,
                                                    unsigned int negativeButton)
{
    m_rangeInputs.push_back(
        new JoystickButtonBidirectionalRangeInput(
            m_inputManager,
            new CallbackMember<Object, Callable, float>(object, callback),
            joystick,
            positiveButton,
            negativeButton
        )
    );
}

/// Assign a joystick button to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument ranging in [0, 100]
/// \param callback         A function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param button           The joystick button which triggers the function.
template<typename Callable>
inline void InputContext::BindRangeToJoystickButton(Callable callback, unsigned int joystick, unsigned int button)
{
    m_rangeInputs.push_back(
        new JoystickButtonUnidirectionalRangeInput(
            m_inputManager,
            new CallbackFunctor<Callable, float>(callback),
            joystick,
            button
        )
    );
}

/// Assign a joystick button to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument ranging in [0, 100]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param button           The joystick button which triggers the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button)
{
    m_rangeInputs.push_back(
        new JoystickButtonUnidirectionalRangeInput(
            m_inputManager,
            new CallbackMember<Object, Callable, float>(object, callback),
            joystick,
            button
        )
    );
}

/// Assign two mouse buttons to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument ranging in [-100, 100]
/// \param callback         A function with a float as a parameter.
/// \param negativeButton   The mouse button which sends a negative float to the function.
/// \param positiveButton   The mouse button which sends a positive float to the function.
template<typename Callable>
inline void InputContext::BindRangeToMouseButtons(Callable callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton)
{
    m_rangeInputs.push_back(
        new MouseButtonBidirectionalRangeInput(
            m_inputManager,
            new CallbackFunctor<Callable, float>(callback),
            negativeButton,
            positiveButton
        )
    );
}

/// Assign two mouse buttons to a member function with a float as a parameter.
/// The member function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument ranging in [-100, 100]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param negativeButton   The mouse button which sends a negative float to the member function.
/// \param positiveButton   The mouse button which sends a positive float to the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToMouseButtons(Object* object, Callable callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton)
{
    m_rangeInputs.push_back(
        new MouseButtonBidirectionalRangeInput(
            m_inputManager,
            new CallbackMember<Object, Callable, float>(object, callback),
            negativeButton,
            positiveButton
        )
    );
}

/// Assign a mouse button to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument ranging in [0, 100]
/// \param callback         A function with a float as a parameter.
/// \param button           The mouse button which triggers the function.
template<typename Callable>
inline void InputContext::BindRangeToMouseButton(Callable callback, sf::Mouse::Button button)
{
    m_rangeInputs.push_back(
        new MouseButtonUnidirectionalRangeInput(
            m_inputManager,
            new CallbackFunctor<Callable, float>(callback),
            button
        )
    );
}

/// Assign a mouse button to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument ranging in [0, 100]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param button           The mouse button which triggers the member function.
template<typename Object, typename Callable>
inline void InputContext::BindRangeToMouseButton(Object* object, Callable callback, sf::Mouse::Button button)
{
    m_rangeInputs.push_back(
        new MouseButtonUnidirectionalRangeInput(
            m_inputManager,
            new CallbackMember<Object, Callable, float>(object, callback),
            button
        )
    );
}

inline void InputContext::BindActionToKeyHelper(Callback<>* callback, sf::Keyboard::Key key, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(
            new KeyAscendingActionInput(
                m_inputManager,
                callback,
                key
            )
        );
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(
            new KeyDescendingActionInput(
                m_inputManager,
                callback,
                key
            )
        );
    }
}

inline void InputContext::BindActionToMouseButtonHelper(Callback<>* callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(
            new MouseButtonAscendingActionInput(
                m_inputManager,
                callback,
                mouseButton
            )
        );
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(
            new MouseButtonDescendingActionInput(
                m_inputManager,
                callback,
                mouseButton
            )
        );
    }
}

inline void InputContext::BindActionToJoystickButtonHelper(Callback<>* callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    if (eventType == EventType::Ascending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(
            new JoystickButtonAscendingActionInput(
                m_inputManager,
                callback,
                joystick,
                button
            )
        );
    }
    if (eventType == EventType::Descending || eventType == EventType::Any)
    {
        m_actionInputs.push_back(
            new JoystickButtonDescendingActionInput(
                m_inputManager,
                callback,
                joystick,
                button
            )
        );
    }
}

inline void InputContext::BindActionToMouseMovedHelper(Callback<>* callback)
{
    m_actionInputs.push_back(
        new MouseMovedActionInput(
            m_inputManager,
            callback
        )
        );
}

inline void InputContext::BindActionToMouseWheelScrolledHelper(Callback<>* callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    switch (mouseWheelAxis)
    {
    case sf::Mouse::VerticalWheel:
        if (mouseWheelDirection == EventType::Ascending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(
                new MouseWheelUpActionInput(
                    m_inputManager,
                    callback
                )
            );
        }
        if (mouseWheelDirection == EventType::Descending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(
                new MouseWheelDownActionInput(
                    m_inputManager,
                    callback
                )
            );
        }
        break;
    case sf::Mouse::HorizontalWheel:
        if (mouseWheelDirection == EventType::Ascending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(
                new MouseWheelRightActionInput(
                    m_inputManager,
                    callback
                )
            );
        }
        if (mouseWheelDirection == EventType::Descending || mouseWheelDirection == EventType::Any)
        {
            m_actionInputs.push_back(
                new MouseWheelLeftActionInput(
                    m_inputManager,
                    callback
                )
            );
        }
        break;
    }
}

inline void InputContext::BindStateToJoystickAxisHelper(Callback<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis,
                                                        float treshold, JoystickAxisPosition axisPosition)
{
    if (axisPosition == JoystickAxisPosition::AboveTreshold || axisPosition == JoystickAxisPosition::Any)
    {
        m_stateInputs.push_back(
            new JoystickAxisAboveTresholdStateInput(
                m_inputManager,
                callback,
                joystick,
                axis,
                treshold
            )
        );
    }
    if (axisPosition == JoystickAxisPosition::BelowTreshold || axisPosition == JoystickAxisPosition::Any)
    {
        m_stateInputs.push_back(
            new JoystickAxisBelowTresholdStateInput(
                m_inputManager,
                callback,
                joystick,
                axis,
                treshold
            )
        );
    }
}

inline void InputContext::BindRangeToMouseScrollHelper(Callback<float>* callback, sf::Mouse::Wheel wheelAxis)
{
    switch (wheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        m_rangeInputs.push_back(
            new HorizontalMouseWheelBidirectionalRangeInput(
                m_inputManager,
                callback
            )
        );
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        m_rangeInputs.push_back(
            new VerticalMouseWheelBidirectionalRangeInput(
                m_inputManager,
                callback
            )
        );
        break;
    }
}

inline void InputContext::BindRangeToMouseScrollHelper(Callback<float>* callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction)
{
    switch (wheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        m_rangeInputs.push_back(
            new HorizontalMouseWheelUnidirectionalRangeInput(
                m_inputManager,
                callback,
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        m_rangeInputs.push_back(
            new VerticalMouseWheelUnidirectionalRangeInput(
                m_inputManager,
                callback,
                rangeRestriction == RangeRestriction::PositiveOnly
            )
        );
        break;
    }
}