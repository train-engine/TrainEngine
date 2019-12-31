#include "InputContext.h" // Not necessary: only for VS Code to not put errors everywhere

/// Constructor
inline InputContext::InputContext(const InputManager& inputManager)
    : m_inputManager(inputManager)
{
}

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

/// Update function which checks if callbacks must be called. This function must be called periodically for the InputContext to work.
/// The best place to call this function is in a HandleInput function.
inline void InputContext::update()
{
    for (auto& input : m_actionInputs)
    {
        if (input->detectedEvent())
        {
            input->callFunction();
        }
    }

    for (auto& input : m_stateInputs)
    {
        if (input->detectedEvent())
        {
            input->callAction();
        }
    }

    for (auto& input : m_rangeInputs)
    {
        if (input->detectedEvent())
        {
            input->callFunction();
        }
    }
}

/// Clear every input binding
inline void InputContext::clear()
{
    for (auto& input : m_actionInputs)
    {
        delete input;
    }
    m_actionInputs.clear();

    for (auto& input : m_stateInputs)
    {
        delete input;
    }
    m_stateInputs.clear();

    for (auto& input : m_rangeInputs)
    {
        delete input;
    }
    m_rangeInputs.clear();
}

// ActionInput

/// Assign a key action to a function with no parameters.
/// \param callback     A function with no parameters.
/// \param key          The key bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename F>
void InputContext::bindActionToKey(F callback, sf::Keyboard::Key key, EventType eventType)
{
    bindActionToKeyHelper(new Functor<F>(callback), key, eventType);
}

/// Assign a key action to a function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
/// \param key          The key bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename O, typename F>
void InputContext::bindActionToKey(O* object, F callback, sf::Keyboard::Key key, EventType eventType)
{
    bindActionToKeyHelper(new Method<O, F>(object, callback), key, eventType);
}

/// Assign a mouse button action to a function with no parameters.
/// \param callback     A function with no parameters.
/// \param mouseButton  The mouse button bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename F>
void InputContext::bindActionToMouseButton(F callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    bindActionToMouseButtonHelper(new Functor<F>(callback), mouseButton, eventType);
}

/// Assign a mouse button action to a member function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
/// \param mouseButton  The mouse button bound to the action.
/// \param eventType    The type of event for the action binding.
template<typename O, typename F>
void InputContext::bindActionToMouseButton(O* object, F callback, sf::Mouse::Button mouseButton, EventType eventType)
{
    bindActionToMouseButtonHelper(new Method<O, F>(object, callback), mouseButton, eventType);
}

/// Assign the mouse movement to a function with no parameters.
/// \param callback     A function with no parameters.
template<typename F>
void InputContext::bindActionToMouseMoved(F callback)
{
    bindActionToMouseMovedHelper(new Functor<F>(callback));
}

/// Assign the mouse movement to a member function with no parameters.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with no parameters.
template<typename O, typename F>
void InputContext::bindActionToMouseMoved(O* object, F callback)
{
    bindActionToMouseMovedHelper(new Method<O, F>(object, callback));
}

/// Assign a mouse wheel scrolled to a function with no parameters.
/// \param callback             A function with no parameters.
/// \param mouseWheelAxis       The wheel axis bound to the callback.
/// \param mouseWheelDirection  The wheel direction bound to the callback.
template<typename F>
void InputContext::bindActionToMouseWheelScrolled(F callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    bindActionToMouseWheelScrolledHelper(new Functor<F>(callback), mouseWheelAxis, mouseWheelDirection);
}

/// Assign a mouse wheel scrolled to a function with no parameters.
/// \param object               The object on which the member function must be called.
/// \param callback             A member function with no parameters.
/// \param mouseWheelAxis       The wheel axis bound to the callback.
/// \param mouseWheelDirection  The wheel direction bound to the callback.
template<typename O, typename F>
void InputContext::bindActionToMouseWheelScrolled(O* object, F callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    bindActionToMouseWheelScrolledHelper(new Method<O, F>(object, callback), mouseWheelAxis, mouseWheelDirection);
}

/// Assign a joystick button action to a function with no parameters.
/// \param callback         A function with no parameters.
/// \param button           The joystick button bound to the action.
/// \param eventType        The type of event for the action binding.
template<typename F>
void InputContext::bindActionToJoystickButton(F callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    bindActionToJoystickButtonHelper(new Functor<F>(callback), joystick, button, eventType);
}

/// Assign a joystick button action to a member function with no parameters.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with no parameters.
/// \param button           The joystick button bound to the action.
/// \param eventType        The type of event for the action binding.
template<typename O, typename F>
void InputContext::bindActionToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button, EventType eventType)
{
    bindActionToJoystickButtonHelper(new Method<O, F>(object, callback), joystick, button, eventType);
}

// StateInput

/// Assign a key event to a function with a bool as a parameter.
/// The function will receive a bool representing if the key is pressed.
/// \param callback     A function with a bool as a parameter.
/// \param key          The key bound to the callback.
template<typename F>
void InputContext::bindStateToKey(F callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(new KeyEventStateInput(m_inputManager, new Functor<F, bool>(callback), key));
}

/// Assign a key event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the key is pressed.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a bool as a parameter.
/// \param key          The key bound to the callback.
template<typename O, typename F>
void InputContext::bindStateToKey(O* object, F callback, sf::Keyboard::Key key)
{
    m_stateInputs.push_back(new KeyEventStateInput(m_inputManager, new Method<O, F, bool>(object, callback), key));
}

/// Assign a mouse button event to a with a bool as a parameter.
/// The function will receive a bool representing if the mouse button is pressed.
/// \param callback     A function with a bool as a parameter.
/// \param button       The mouse button bound to the callback.
template<typename F>
void InputContext::bindStateToMouseButton(F callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(new MouseButtonEventStateInput(m_inputManager, new Functor<F, bool>(callback), button));
}

/// Assign a mouse button event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the mouse button is pressed.
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a bool as a parameter.
/// \param button       The mouse button bound to the callback.
template<typename O, typename F>
void InputContext::bindStateToMouseButton(O* object, F callback, sf::Mouse::Button button)
{
    m_stateInputs.push_back(new MouseButtonEventStateInput(m_inputManager, new Method<O, F, bool>(object, callback), button));
}

/// Assign a joystick button event to a function with a bool as a parameter.
/// The function will receive a bool representing if the joystick button is pressed.
/// \param callback         A function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param button           The joystick button bound to the callback.
template<typename F>
void InputContext::bindStateToJoystickButton(F callback, unsigned int joystick, unsigned int button)
{
    m_stateInputs.push_back(new JoystickButtonEventStateInput(m_inputManager, new Functor<F, bool>(callback), joystick, button));
}

/// Assign a joystick button event to a member function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick button is pressed.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param button           The joystick button bound to the callback.
template<typename O, typename F>
void InputContext::bindStateToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button)
{
    m_stateInputs.push_back(new JoystickButtonEventStateInput(m_inputManager, new Method<O, F, bool>(object, callback), joystick, button));
}

/// Assign a joystick axis position to a function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick axis passed the threshold.
/// Note: If the threshold is bigger or equal to zero, the function will receive true if the axis position is bigger than the threshold.
/// If the threshold is smaller than zero, the function will receive true if the axis position is smaller than the threshold.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param axis             The joystick axis bound to the callback.
/// \param threshold         The threshold to be exceeded for the callback to be called.
/// \param axisPosition     The position of the axis relative to the threshold.
template<typename F>
void InputContext::bindStateToJoystickAxis(F callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold,
                                           JoystickAxisPosition axisPosition)
{
    bindStateToJoystickAxisHelper(new Functor<F, bool>(callback), joystick, axis, threshold, axisPosition);
}

/// Assign a joystick axis position to a function with a bool as a parameter.
/// The member function will receive a bool representing if the joystick axis passed the threshold.
/// Note: If the threshold is bigger or equal to zero, the function will receive true if the axis position is bigger than the threshold.
/// If the threshold is smaller than zero, the function will receive true if the axis position is smaller than the threshold.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a bool as a parameter.
/// \param joystick         The joystick id.
/// \param axis             The joystick axis bound to the callback.
/// \param threshold         The threshold to be exceeded for the callback to be called.
/// \param axisPosition     The position of the axis relative to the threshold.
template<typename O, typename F>
void InputContext::bindStateToJoystickAxis(O* object, F callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold,
                                           JoystickAxisPosition axisPosition)
{
    bindStateToJoystickAxisHelper(new Method<O, F, bool>(object, callback), joystick, axis, threshold, axisPosition);
}

// RangeInput

/// Assign two keys to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument in range [-1.0, 1.0]
/// \param callback     A function with a float as a parameter.
/// \param negativeKey  The key which sends a negative float to the function.
/// \param positiveKey  The key which sends a positive float to the function.
template<typename F>
void InputContext::bindRangeToKeyboard(F callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.push_back(new KeyboardBidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback), negativeKey, positiveKey));
}

/// Assign two keys to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument in range [-1.0, 1.0]
/// \param object       The object on which the member function must be called.
/// \param callback     A member function with a float as a parameter.
/// \param negativeKey  The key which sends a negative float to the member function.
/// \param positiveKey  The key which sends a positive float to the member function.
template<typename O, typename F>
void InputContext::bindRangeToKeyboard(O* object, F callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.push_back(
        new KeyboardBidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback), negativeKey, positiveKey));
}

/// Assign a key to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument in range [0.0, 1.0]
/// \param callback         A function with a float as a parameter.
/// \param key              The key which triggers the function.
template<typename F>
void InputContext::bindRangeToKeyboard(F callback, sf::Keyboard::Key key)
{
    m_rangeInputs.push_back(new KeyboardUnidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback), key));
}

/// Assign key to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument in range [0.0, 1.0]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param key              The key which triggers the member function.
template<typename O, typename F>
void InputContext::bindRangeToKeyboard(O* object, F callback, sf::Keyboard::Key key)
{
    m_rangeInputs.push_back(new KeyboardUnidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback), key));
}

/// Assign two mouse buttons to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument in range [-1.0, 1.0]
/// \param callback         A function with a float as a parameter.
/// \param negativeButton   The mouse button which sends a negative float to the function.
/// \param positiveButton   The mouse button which sends a positive float to the function.
template<typename F>
void InputContext::bindRangeToMouseButtons(F callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton)
{
    m_rangeInputs.push_back(
        new MouseButtonBidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback), negativeButton, positiveButton));
}

/// Assign two mouse buttons to a member function with a float as a parameter.
/// The member function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument in range [-1.0, 1.0]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param negativeButton   The mouse button which sends a negative float to the member function.
/// \param positiveButton   The mouse button which sends a positive float to the member function.
template<typename O, typename F>
void InputContext::bindRangeToMouseButtons(O* object, F callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton)
{
    m_rangeInputs.push_back(
        new MouseButtonBidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback), negativeButton, positiveButton));
}

/// Assign a mouse button to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument in range [0.0, 1.0]
/// \param callback         A function with a float as a parameter.
/// \param button           The mouse button which triggers the function.
template<typename F>
void InputContext::bindRangeToMouseButton(F callback, sf::Mouse::Button button)
{
    m_rangeInputs.push_back(new MouseButtonUnidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback), button));
}

/// Assign a mouse button to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument in range [0.0, 1.0]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param button           The mouse button which triggers the member function.
template<typename O, typename F>
void InputContext::bindRangeToMouseButton(O* object, F callback, sf::Mouse::Button button)
{
    m_rangeInputs.push_back(new MouseButtonUnidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback), button));
}

/// Assign the horizontal mouse movement to a function with a float as a parameter.
/// The function will receive the horizontal mouse movement as an argument.
/// \param callback         A function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the horizontal mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the
///                         mouse movement. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the
///                         negative values of the mouse movement.
template<typename F>
void InputContext::bindRangeToHorizontalMouseMovement(F callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(new HorizontalMouseMovementBidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback)));
    }
    else
    {
        m_rangeInputs.push_back(new HorizontalMouseMovementUnidirectionalRangeInput(m_inputManager,
                                                                                    new Functor<F, float>(callback),
                                                                                    rangeRestriction == RangeRestriction::PositiveOnly));
    }
}

/// Assign the horizontal mouse movement to a member function with a float as a parameter.
/// The member function will receive the horizontal mouse movement as an argument.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the horizontal mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the
///                         mouse movement. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the
///                         negative values of the mouse movement.
template<typename O, typename F>
void InputContext::bindRangeToHorizontalMouseMovement(O* object, F callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new HorizontalMouseMovementBidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback)));
    }
    else
    {
        m_rangeInputs.push_back(new HorizontalMouseMovementUnidirectionalRangeInput(m_inputManager,
                                                                                    new Method<O, F, float>(object, callback),
                                                                                    rangeRestriction == RangeRestriction::PositiveOnly));
    }
}

/// Assign the vertical mouse movement to a function with a float as a parameter.
/// The function will receive the vertical mouse movement as an argument.
/// \param callback         A function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the vertical mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the
///                         mouse movement. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the
///                         negative values of the mouse movement.
template<typename F>
void InputContext::bindRangeToVerticalMouseMovement(F callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(new VerticalMouseMovementBidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback)));
    }
    else
    {
        m_rangeInputs.push_back(new VerticalMouseMovementUnidirectionalRangeInput(m_inputManager,
                                                                                  new Functor<F, float>(callback),
                                                                                  rangeRestriction == RangeRestriction::PositiveOnly));
    }
}

/// Assign the vertical mouse movement to a member function with a float as a parameter.
/// The member function will receive the vertical mouse movement as an argument.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param rangeRestriction The restriction of the callback range of the vertical mouse movement.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the
///                         mouse movement. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the
///                         negative values of the mouse movement.
template<typename O, typename F>
void InputContext::bindRangeToVerticalMouseMovement(O* object, F callback, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new VerticalMouseMovementBidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback)));
    }
    else
    {
        m_rangeInputs.push_back(new VerticalMouseMovementUnidirectionalRangeInput(m_inputManager,
                                                                                  new Method<O, F, float>(object, callback),
                                                                                  rangeRestriction == RangeRestriction::PositiveOnly));
    }
}

/// Assign a mouse wheel axis to a function with a float as a parameter.
/// The function will receive the mouse scroll as an argument.
/// \param callback         A function with a float as a parameter.
/// \param wheelAxis        The mouse wheel axis bound to the callback.
/// \param rangeRestriction The restriction of the callback range of the mouse scroll.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of
///                         scroll. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative
///                         values of scroll.
template<typename F>
void InputContext::bindRangeToMouseWheelScroll(F callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        bindRangeToMouseWheelScrollHelper(new Functor<F, float>(callback), wheelAxis);
    }
    else
    {
        bindRangeToMouseWheelScrollHelper(new Functor<F, float>(callback), wheelAxis, rangeRestriction);
    }
}

/// Assign a mouse wheel axis to a member function with a float as a parameter.
/// The member function will receive the mouse scroll as an argument.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param wheelAxis        The mouse wheel axis bound to the member function.
/// \param rangeRestriction The restriction of the callback range of the mouse scroll.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of
///                         scroll. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative
///                         values of scroll.
template<typename O, typename F>
void InputContext::bindRangeToMouseWheelScroll(O* object, F callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        bindRangeToMouseWheelScrollHelper(new Method<O, F, float>(object, callback), wheelAxis);
    }
    else
    {
        bindRangeToMouseWheelScrollHelper(new Method<O, F, float>(object, callback), wheelAxis, rangeRestriction);
    }
}

/// Assign two joystick buttons to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument in range [-1.0, 1.0]
/// \param callback         A function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param negativeButton   The joystick button which sends a negative float to the function.
/// \param positiveButton   The joystick button which sends a positive float to the function.
template<typename F>
void InputContext::bindRangeToJoystickButtons(F callback, unsigned int joystick, unsigned int negativeButton, unsigned int positiveButton)
{
    m_rangeInputs.push_back(new JoystickButtonBidirectionalRangeInput(m_inputManager,
                                                                      new Functor<F, float>(callback),
                                                                      joystick,
                                                                      negativeButton,
                                                                      positiveButton));
}

/// Assign two joystick buttons to a member function with a float as a parameter.
/// The member function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument in range [-1.0, 1.0]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param negativeButton   The joystick button which sends a negative float to the member function.
/// \param positiveButton   The joystick button which sends a positive float to the member function.
template<typename O, typename F>
void InputContext::bindRangeToJoystickButtons(O* object, F callback, unsigned int joystick, unsigned int negativeButton,
                                              unsigned int positiveButton)
{
    m_rangeInputs.push_back(new JoystickButtonBidirectionalRangeInput(m_inputManager,
                                                                      new Method<O, F, float>(object, callback),
                                                                      joystick,
                                                                      negativeButton,
                                                                      positiveButton));
}

/// Assign a joystick button to a function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The function will receive an argument in range [0.0, 1.0]
/// \param callback         A function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param button           The joystick button which triggers the function.
template<typename F>
void InputContext::bindRangeToJoystickButton(F callback, unsigned int joystick, unsigned int button)
{
    m_rangeInputs.push_back(new JoystickButtonUnidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback), joystick, button));
}

/// Assign a joystick button to a member function with a float as a parameter.
/// The function will receive the simulated analog input as an argument.
/// Note: The member function will receive an argument in range [0.0, 1.0]
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param button           The joystick button which triggers the member function.
template<typename O, typename F>
void InputContext::bindRangeToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button)
{
    m_rangeInputs.push_back(
        new JoystickButtonUnidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback), joystick, button));
}

/// Assign a joystick axis to a function with a float as a parameter.
/// The function will receive the joystick axis' position as an argument.
/// Note: The function will receive an argument in range [0.0, 1.0] or [-1.0, 0.0] or [-1.0, 1.0] depending on the rangeRestriction.
/// \param callback         A function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param axis             The joystick axis bound to the callback.
/// \param rangeRestriction The restriction of the callback range of the joystick axis.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the
///                         axis. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values
///                         of the axis. If range Restriction is RangeRestriction::None, the callback will receive both positive dans negatives
///                         values of the axis.
template<typename F>
void InputContext::bindRangeToJoystickAxis(F callback, unsigned int joystick, sf::Joystick::Axis axis, RangeRestriction rangeRestriction)
{
    // Bidirectional input if no range restriction
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(new JoystickAxisBidirectionalRangeInput(m_inputManager, new Functor<F, float>(callback), joystick, axis));
    }
    else
    {
        m_rangeInputs.push_back(new JoystickAxisUnidirectionalRangeInput(m_inputManager,
                                                                         new Functor<F, float>(callback),
                                                                         joystick,
                                                                         axis,
                                                                         rangeRestriction == RangeRestriction::PositiveOnly));
    }
}

/// Assign a joystick axis to a member function with a float as a parameter.
/// The member function will receive the joystick axis' position as an argument.
/// Note: The member function will receive an argument in range [0.0, 1.0] or [-1.0, 0.0] or [-1.0, 1.0] depending on the rangeRestriction.
/// \param object           The object on which the member function must be called.
/// \param callback         A member function with a float as a parameter.
/// \param joystick         The joystick id.
/// \param axis     The joystick axis bound to the callback.
/// \param rangeRestriction The restriction of the callback range of the joystick axis.
///                         If rangeRestriction is RangeRestriction::PositiveOnly, the callback will only receive the positive values of the
///                         axis. If rangeRestriction is RangeRestriction::NegativeOnly, the callback will only receive the negative values
///                         of the axis. If range Restriction is RangeRestriction::None, the callback will receive both positive dans negatives
///                         values of the axis.
template<typename O, typename F>
void InputContext::bindRangeToJoystickAxis(O* object, F callback, unsigned int joystick, sf::Joystick::Axis axis,
                                           RangeRestriction rangeRestriction)
{
    if (rangeRestriction == RangeRestriction::None)
    {
        m_rangeInputs.push_back(
            new JoystickAxisBidirectionalRangeInput(m_inputManager, new Method<O, F, float>(object, callback), joystick, axis));
    }
    else
    {
        m_rangeInputs.push_back(new JoystickAxisUnidirectionalRangeInput(m_inputManager,
                                                                         new Method<O, F, float>(object, callback),
                                                                         joystick,
                                                                         axis,
                                                                         rangeRestriction == RangeRestriction::PositiveOnly));
    }
}

// Helpers

inline void InputContext::bindActionToKeyHelper(Callable<>* callback, sf::Keyboard::Key key, EventType eventType)
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

inline void InputContext::bindActionToMouseButtonHelper(Callable<>* callback, sf::Mouse::Button mouseButton, EventType eventType)
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

inline void InputContext::bindActionToJoystickButtonHelper(Callable<>* callback, unsigned int joystick, unsigned int button,
                                                           EventType eventType)
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

inline void InputContext::bindActionToMouseMovedHelper(Callable<>* callback)
{
    m_actionInputs.push_back(new MouseMovedActionInput(m_inputManager, callback));
}

inline void InputContext::bindActionToMouseWheelScrolledHelper(Callable<>* callback, sf::Mouse::Wheel mouseWheelAxis,
                                                               EventType mouseWheelDirection)
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

inline void InputContext::bindStateToJoystickAxisHelper(Callable<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis,
                                                        float threshold, JoystickAxisPosition axisPosition)
{
    if (axisPosition == JoystickAxisPosition::AboveThreshold || axisPosition == JoystickAxisPosition::Any)
    {
        m_stateInputs.push_back(new JoystickAxisAboveThresholdStateInput(m_inputManager, callback, joystick, axis, threshold));
    }
    if (axisPosition == JoystickAxisPosition::BelowThreshold || axisPosition == JoystickAxisPosition::Any)
    {
        m_stateInputs.push_back(new JoystickAxisBelowThresholdStateInput(m_inputManager, callback, joystick, axis, threshold));
    }
}

inline void InputContext::bindRangeToMouseWheelScrollHelper(Callable<float>* callback, sf::Mouse::Wheel wheelAxis)
{
    switch (wheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        m_rangeInputs.push_back(new HorizontalMouseWheelBidirectionalRangeInput(m_inputManager, callback));
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        m_rangeInputs.push_back(new VerticalMouseWheelBidirectionalRangeInput(m_inputManager, callback));
        break;
    }
}

inline void InputContext::bindRangeToMouseWheelScrollHelper(Callable<float>* callback, sf::Mouse::Wheel wheelAxis,
                                                            RangeRestriction rangeRestriction)
{
    switch (wheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        m_rangeInputs.push_back(
            new HorizontalMouseWheelUnidirectionalRangeInput(m_inputManager, callback, rangeRestriction == RangeRestriction::PositiveOnly));
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        m_rangeInputs.push_back(
            new VerticalMouseWheelUnidirectionalRangeInput(m_inputManager, callback, rangeRestriction == RangeRestriction::PositiveOnly));
        break;
    }
}
