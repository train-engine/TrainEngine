# include "InputContext.h"

namespace
{
    bool IsKeyInput(ActionInputBinds input)
    {
        return input == ActionInputBinds::KeyAscending || input == ActionInputBinds::KeyDescending;
    }

    bool IsKeyInput(StateInputBinds input)
    {
        return input == StateInputBinds::KeyEvent;
    }

    bool IsKeyInput(RangeInputBinds input)
    {
        return input == RangeInputBinds::Keyboard;
    }

    bool IsMouseButtonInput(ActionInputBinds input)
    {
        return input == ActionInputBinds::MouseButtonAscending || input == ActionInputBinds::MouseButtonDescending;
    }

    bool IsMouseButtonInput(StateInputBinds input)
    {
        return input == StateInputBinds::MouseButtonEvent;
    }

    bool IsJoystickButtonInput(ActionInputBinds input)
    {
        return input == ActionInputBinds::JoystickButtonAscending || input == ActionInputBinds::JoystickButtonDescending;
    }

    bool IsJoystickButtonInput(StateInputBinds input)
    {
        return input == StateInputBinds::JoystickButtonEvent;
    }

    bool IsJoystickAxisInput(RangeInputBinds input)
    {
        return input == RangeInputBinds::JoystickAxis;
    }
}

/// Class to make a callback to a function with no parameters when an action is performed.
/// \param callback     A void callback with no parameters.
/// \param event        Event bound to the callback.
/// \param inputId      The id (sf::Keyboard::Key or sf::Mouse::Button or int joystickButton) of the input.
ActionInput::ActionInput(void (*callBack)(), ActionInputBinds event, int inputId)
    : m_callBack(callBack),
      m_event(event),
      m_inputId(inputId)
{

}

/// Check if an input is activated and call the callback when an action is performed.
/// \param inputManager A constant reference to the inputManager for input handling.
void ActionInput::CheckIsTriggered(const InputManager& inputManager) const
{
    switch(m_event)
    {
    case ActionInputBinds::KeyDescending:
        if (inputManager.IsKeyDescending(static_cast<sf::Keyboard::Key>(m_inputId)))
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::KeyAscending:
        if (inputManager.IsKeyAscending(static_cast<sf::Keyboard::Key>(m_inputId)))
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::MouseButtonDescending:
        if (inputManager.IsMouseButtonDescending(static_cast<sf::Mouse::Button>(m_inputId)))
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::MouseButtonAscending:
        if (inputManager.IsMouseButtonAscending(static_cast<sf::Mouse::Button>(m_inputId)))
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::JoystickButtonDescending:
        if (inputManager.IsJoystickButtonDescending(0, m_inputId))
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::JoystickButtonAscending:
        if (inputManager.IsJoystickButtonAscending(0, m_inputId))
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::MouseMoved:
        if (inputManager.DetectedMouseMovedEvent())
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::MouseWheelUp:
        if (inputManager.DetectedMouseWheelScrolledEvent() && inputManager.GetVerticalMouseWheelDelta() > 0)
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::MouseWheelDown:
        if (inputManager.DetectedMouseWheelScrolledEvent() && inputManager.GetVerticalMouseWheelDelta() < 0)
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::MouseWheelLeft:
        if (inputManager.DetectedMouseWheelScrolledEvent() && inputManager.GetHorizontalMouseWheelDelta() < 0)
        {
            m_callBack();
        }
        break;
    case ActionInputBinds::MouseWheelRight:
        if (inputManager.DetectedMouseWheelScrolledEvent() && inputManager.GetHorizontalMouseWheelDelta() > 0)
        {
            m_callBack();
        }
        break;
    }
}

/// Class to make a callback to a function with a bool as a parameter when an action is performed.
/// \param callback     Pointer to a void function taking a boolean as a parameter.
/// \param event        Event bound to the callback.
/// \param inputId      The id (sf::Keyboard::Key or sf::Mouse::Button or int joystickButton) of the input.\
                        This parameter can be ignored if event is a mouse event, since it is not relevant.
StateInput::StateInput(void (* callBack)(bool), StateInputBinds event, int inputId)
    : m_callBack(callBack),
      m_event(event),
      m_inputId(inputId)
{

}

/// Check if an input is activated and call the callback when an action is performed.
/// \param inputManager A constant reference to the inputManager for input handling.
void StateInput::CheckIsTriggered(const InputManager& inputManager) const
{
    switch(m_event)
    {
    case StateInputBinds::KeyEvent:
        if (inputManager.IsKeyDescending(static_cast<sf::Keyboard::Key>(m_inputId)))
        {
            // Send isKeyDescending to callback
            m_callBack(true);
        }
        else if (inputManager.IsKeyAscending(static_cast<sf::Keyboard::Key>(m_inputId)))
        {
            // Send isKeyAscending to callback
            m_callBack(false);
        }
        break;
    case StateInputBinds::MouseButtonEvent:
        if (inputManager.IsMouseButtonDescending(static_cast<sf::Mouse::Button>(m_inputId)))
        {
            // Send isMouseButtonDescending to callback
            m_callBack(true);
        }
        else if (inputManager.IsMouseButtonAscending(static_cast<sf::Mouse::Button>(m_inputId)))
        {
            // Send isMouseButtonAscending to callback
            m_callBack(false);
        }
        break;
    case StateInputBinds::JoystickButtonEvent:
        if (inputManager.IsJoystickButtonAscending(0, m_inputId))
        {
            // Send isJoystickButtonDescending to callback
            m_callBack(true);
        }
        else if (inputManager.IsJoystickButtonDescending(0, m_inputId))
        {
            // Send isJoystickButtonAscending to callback
            m_callBack(false);
        }
        break;
    }
}

/// Class to make a callback to a function with a float as a parameter when a joystick or mouse action is performed.
/// \param callback     Pointer to a void function taking a float as a parameter.
/// \param event        Event bound to the callback.
/// \param inputId      The id of the controller.\
                        This parameter can be ignored if event is a mouse or scroll event, since it is not relevant.
// \param joystickAxis  The axis to use for the input binding.\
                        This parameter can be ignored if event is a mouse or scroll event, since it is not relevant.
RangeInput::RangeInput(void (* callBack)(float), RangeInputBinds event, sf::Joystick::Axis joystickAxis)
    : m_callBack(callBack),
      m_event(event),
      m_joystickAxis(joystickAxis)
{

}

/// Class to make a callback to a function with a float as a parameter when a keyboard action is performed.
/// \param callback     Pointer to a void function taking a float as a parameter.
/// \param negativeKey  The key wich will send a negative value (-1) to the callback.
/// \param positiveKey  The key wich will send a positive value (+1) to the callback.
RangeInput::RangeInput(void (* callBack)(float), sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
    : m_callBack(callBack),
      m_event(RangeInputBinds::Keyboard),
      m_negativeKey(negativeKey),
      m_positiveKey(positiveKey)
{

}

/// Check if an input is activated and call the callback if necessary.
/// \param inputManager A constant reference to the inputManager for input handling.
void RangeInput::CheckIsTriggered(const InputManager& inputmanager) const
{
    switch(m_event)
    {
    case RangeInputBinds::JoystickAxis:
        if (inputmanager.DetectedJoystickMovedEvent())
        {
            // Send joystickAxisPosition to the callback
            m_callBack(inputmanager.GetJoystickAxisPosition(0, m_joystickAxis));
        }
        break;
    case RangeInputBinds::VerticalMouseWheel:
        if (inputmanager.DetectedMouseWheelScrolledEvent())
        {
            // Send vertical mouseWheelDelta to callback
            float delta = inputmanager.GetVerticalMouseWheelDelta();
            int a = 0;
            m_callBack(delta);
        }
        break;
    case RangeInputBinds::HorizontalMouseWheel:
        if (inputmanager.DetectedMouseWheelScrolledEvent())
        {
            // Send horizontal mouseWheelDelta to callback
            m_callBack(inputmanager.GetHorizontalMouseWheelDelta());
        }
        break;
    case RangeInputBinds::HorizontalMouseMovement:
        if (inputmanager.DetectedMouseMovedEvent())
        {
            // Send horizontal mouse mouvement to callback
            m_callBack(inputmanager.GetMousePositionDelta().x);
        }
        break;
    case RangeInputBinds::VertialMouseMovement:
        if (inputmanager.DetectedMouseMovedEvent())
        {
            // Send vertical mouse mouvement to callback
            m_callBack(inputmanager.GetMousePositionDelta().y);
        }
        break;
    case RangeInputBinds::Keyboard:
        // If there is an event for one of the two keys
        if (inputmanager.IsKeyAscending(m_negativeKey) || inputmanager.IsKeyAscending(m_positiveKey) ||
            inputmanager.IsKeyDescending(m_negativeKey) || inputmanager.IsKeyDescending(m_positiveKey))
        {
            bool isNegativeKeyHeld = inputmanager.IsKeyHeld(m_negativeKey);
            bool isPositiveKeyHeld = inputmanager.IsKeyHeld(m_positiveKey);

            // If  only negative key is held
            if (isNegativeKeyHeld == true && isPositiveKeyHeld == false)
            {
                m_callBack(-1.0f);
                return;
            }

            // If  only positive key is held
            if (isPositiveKeyHeld == true && isNegativeKeyHeld == false)
            {
                m_callBack(1.0f);
                return;
            }

            m_callBack(0.0f);
            return;
        }
        break;
    }
}

/// Assign a key action to a void callback with no parameters.
/// \param callback     A void callback with no parameters.
/// \param key          The key bound to the action.
/// \param eventType    The type of event for the action binding.
void InputContext::BindActionToKey(void (* callBack)(), sf::Keyboard::Key key, EventType eventType)
{
    switch(eventType)
    {
    case EventType::Ascending:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::KeyAscending, static_cast<int>(key));
        break;
    case EventType::Descending:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::KeyDescending, static_cast<int>(key));
        break;
    case EventType::Any:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::KeyAscending, static_cast<int>(key));
        m_actionInputs.emplace_back(callBack, ActionInputBinds::KeyDescending, static_cast<int>(key));
        break;
    }
}

/// Assign a mouse button action to a void callback with no parameters.
/// \param callback     A void callback with no parameters.
/// \param mouseButton  The mouse button bound to the action.
/// \param eventType    The type of event for the action binding.
void InputContext::BindActionToMouseButton(void (* callBack)(), sf::Mouse::Button mouseButton, EventType eventType)
{
    switch(eventType)
    {
    case EventType::Ascending:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseButtonAscending, static_cast<int>(mouseButton));
        break;
    case EventType::Descending:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseButtonDescending, static_cast<int>(mouseButton));
        break;
    case EventType::Any:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseButtonAscending, static_cast<int>(mouseButton));
        m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseButtonDescending, static_cast<int>(mouseButton));
        break;
    }
}

/// Assign a joystick button action to a void callback with no parameters.
/// \param callback     A void callback with no parameters.
/// \param joystick     The joystick button bound to the action.
/// \param eventType    The type of event for the action binding.
void InputContext::BindActionToJoystickButton(void (* callBack)(), int joystickButton, EventType eventType)
{
    switch(eventType)
    {
    case EventType::Ascending:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::JoystickButtonAscending, joystickButton);
        break;
    case EventType::Descending:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::JoystickButtonDescending, joystickButton);
        break;
    case EventType::Any:
        m_actionInputs.emplace_back(callBack, ActionInputBinds::JoystickButtonAscending, joystickButton);
        m_actionInputs.emplace_back(callBack, ActionInputBinds::JoystickButtonDescending, joystickButton);
        break;
    }
}

/// Assign the mouse movement to a void callback with no parameters.
/// \param callback     A void callback with no parameters.
/// \param mouseButton  The joystick button bound to the action.
void InputContext::BindActionToMouseMoved(void (* callBack)())
{
    m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseMoved);
}

/// Assign a mouse wheel scrolled to a void callback with no parameters.
/// \param callback             A void callback with no parameters.
/// \param mouseWheelAxis       The wheel Axis bound to the callback.
/// \param mouseWheelDirection  The wheelDirection bound to the callback.
void InputContext::BindActionToMouseWheel(void (* callBack)(), sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection)
{
    switch (mouseWheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        switch (mouseWheelDirection)
        {
        case EventType::Ascending:
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelLeft);
            break;
        case EventType::Descending:
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelRight);
            break;
        case EventType::Any:
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelLeft);
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelRight);
            break;
        }
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        switch (mouseWheelDirection)
        {
        case EventType::Ascending:
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelDown);
            break;
        case EventType::Descending:
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelUp);
            break;
        case EventType::Any:
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelDown);
            m_actionInputs.emplace_back(callBack, ActionInputBinds::MouseWheelUp);
            break;
        }
        break;
    }
}

/// Assign a key event to a void callback with an int as a parameter.
/// The callback will receive a bool representing if the key is pressed.
/// \param callback     A void callback with a bool as a parameter.
/// \param key          The key bound to the callback.
void InputContext::BindStateToKey(void (* callBack)(bool), sf::Keyboard::Key key)
{
    m_stateInputs.emplace_back(callBack, StateInputBinds::KeyEvent, static_cast<int>(key));
}

/// Assign a mouse button event to a void callback with an int as a parameter.
/// The callback will receive a bool representing if the mouse button is pressed.
/// \param callback     A void callback with a bool as a parameter.
/// \param button       The mouse button bound to the callback.
void InputContext::BindStateToMouseButton(void (* callBack)(bool), sf::Mouse::Button button)
{
    m_stateInputs.emplace_back(callBack, StateInputBinds::MouseButtonEvent, static_cast<int>(button));
}

/// Assign a joystick button event to a void callback with an int as a parameter.
/// The callback will receive a bool representing if the joystick button is pressed.
/// \param callback     A void callback with a bool as a parameter.
/// \param joystick     The joystick bound to the callback.
void InputContext::BindStateToJoystickButton(void (* callBack)(bool), int joyStick)
{
    m_stateInputs.emplace_back(callBack, StateInputBinds::JoystickButtonEvent, joyStick);
}

/// Assign a joystick to a void callback with a float as a parameter.
/// \param callback     A void callback with a float as a parameter.
/// \param joystick     The joystick bound to the callback.
/// \param joystickAxis The joystick axis bound to the callback.
void InputContext::BindRangeToJoystick(void (* callBack)(float), sf::Joystick::Axis joystickAxis)
{
    m_rangeInputs.emplace_back(callBack, RangeInputBinds::JoystickAxis, joystickAxis);
}

/// Assign a joystick to a void callback with a float as a parameter.
/// \param callback     A void callback with a float as a parameter.
/// \param wheelAxis    The mouse wheel axis bound to the callback.
void InputContext::BindRangeToMouseScroll(void (* callBack)(float), sf::Mouse::Wheel wheelAxis)
{
    switch(wheelAxis)
    {
    case sf::Mouse::Wheel::HorizontalWheel:
        m_rangeInputs.emplace_back(callBack, RangeInputBinds::HorizontalMouseWheel);
        break;
    case sf::Mouse::Wheel::VerticalWheel:
        m_rangeInputs.emplace_back(callBack, RangeInputBinds::VerticalMouseWheel);
        break;
    }
}

/// Assign the horizontal mouse movement to a void callback with a float as a parameter.
/// The callback will receive the horizontal mouse movement as an argument.
/// \param callback     A void callback with a float as a parameter.
void InputContext::BindRangeToHorizontalMouseMoved(void (* callBack)(float))
{
    m_rangeInputs.emplace_back(callBack, RangeInputBinds::VertialMouseMovement);
}

/// Assign the vertical mouse movement to a void callback with a float as a parameter.
/// The callback will receive the vertical mouse movement as an argument.
/// \param callback     A void callback with a float as a parameter.
void InputContext::BindRangeToVerticalMouseMoved(void (* callBack)(float))
{
    m_rangeInputs.emplace_back(callBack, RangeInputBinds::HorizontalMouseMovement);
}

/// Assign the vertical mouse movement to a void callback with a float as a parameter.
/// The callback will receive the vertical mouse movement as an argument.
/// \param callback     A void callback with a float as a parameter.
void InputContext::BindRangeToKeyboard(void (* callBack)(float), sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
{
    m_rangeInputs.emplace_back(callBack, negativeKey, positiveKey);
}

/// Clear all the input bindings.
void InputContext::Clear()
{
    m_actionInputs.clear();
    m_stateInputs.clear();
    m_rangeInputs.clear();
}

/// Check if a key is used in the InputContext
/// \param key  The key to check
/// \return true if the key is used
bool InputContext::IsKeyUsed(sf::Keyboard::Key key) const
{
    // Action inputs
    for (const auto& input : m_actionInputs)
    {
        // If one of the inputs in m_acionInputs is a key input
        // and the eventId is the same as the key the function is looking for.
        if (IsKeyInput(input.GetEventType()) && static_cast<int>(key) == input.GetInputId())
        {
            return true;
        }
    }

    // State inputs
    for (const auto& input : m_stateInputs)
    {
        // If one of the inputs in m_stateInputs is a key input
        // and the eventId is the same as the key the function is looking for.
        if (IsKeyInput(input.GetEventType()) && static_cast<int>(key) == input.GetInputId())
        {
            return true;
        }
    }

    // Range inputs
    for (const auto& input : m_rangeInputs)
    {
        // If one of the inputs in m_rangeInputs is a key input
        // and at least one of the two keys is the same as key the function is looking for.
        if (IsKeyInput(input.GetEventType()) && (key == input.GetNegativeKey() || key == input.GetPositiveKey()))
        {
            return true;
        }
    }

    return false;
}

/// Check if a mouse button is used in the InputContext
/// \param mouseButton  The mouse button to check
/// \return true if tje mouse button is used
bool InputContext::IsMouseButtonUsed(sf::Mouse::Button mouseButton) const
{
    // Action inputs
    for (const auto& input : m_actionInputs)
    {
        // If one of the inputs in m_acionInputs is a mouse button input
        // and the eventId is the same as the mouse button the function is looking for.
        if (IsMouseButtonInput(input.GetEventType()) && static_cast<int>(mouseButton) == input.GetInputId())
        {
            return true;
        }
    }

    // State inputs
    for (const auto& input : m_stateInputs)
    {
        // If one of the inputs in m_stateInputs is a mouse button input
        // and the eventId is the same as the mouse button the function is looking for.
        if (IsMouseButtonInput(input.GetEventType()) && static_cast<int>(mouseButton) == input.GetInputId())
        {
            return true;
        }
    }

    // There is no need to look for a mouse button in m_rangeInputs since the engine doesn't support
    // mouse button for range inputs, and it doesn't really make sense. Maybe we could add support
    // for it in the future.

    return false;
}

/// Check if a joystick axis is used in the InputContext
/// \param joystickAxis the joystick axis to check
/// \return true if the joystick axis is used
bool InputContext::IsJoystickAxisUsed(sf::Joystick::Axis joystickAxis) const
{
    // There is no need to look for a joystick axis in m_actionInputs since the input manager doesn't support it for now.
    // The input manager would need to detect if a joystick position is not 0 and was 0 the last tick, and create an event for it.
    // It would also need the detect if a joystick position is 0 and was not 0 the last tick, and create an event for it.
    // This would allow the joystick to act as a button.

    // There is no need to look for a joystick axis in m_stateInputs since the input manager doesn't support it for now.
    // The input manager would need to detect if a joystick position is not 0 and was 0 the last tick, and create an event for it.
    // It would also need the detect if a joystick position is 0 and was not 0 the last tick, and create an event for it.
    // This would allow the joystick to act as a button.

    // Range inputs
    for (const auto& input : m_rangeInputs)
    {
        // If one of the inputs in m_rangeInputs is a joystick axis
        // and the eventId is the same as the joystick axis the function is looking for.
        if (IsJoystickAxisInput(input.GetEventType()) && joystickAxis == input.GetJoystickAxis())
        {
            return true;
        }
    }

    return false;
}

/// Check if a joystick button is used in the InputContext
/// \param joystickButton the joystick nutton to check
/// \return true if the joystick button is used
bool InputContext::IsJoystickButtonUsed(unsigned int joystickButton) const
{
    // Action inputs
    for (const auto& input : m_actionInputs)
    {
        // If one of the inputs in m_acionInputs is a joystick button
        // and the eventId is the same as the joystick button the function is looking for.
        if (IsJoystickButtonInput(input.GetEventType()) && joystickButton == input.GetInputId())
        {
            return true;
        }
    }

    // State inputs
    for (const auto& input : m_stateInputs)
    {
        // If one of the inputs in m_stateInputs is a key input
        // and the eventId is the same as the key the function is looking for.
        if (IsJoystickButtonInput(input.GetEventType()) && joystickButton == input.GetInputId())
        {
            return true;
        }
    }

    // There is no need to look for a joystick button in m_rangeInputs since the engine doesn't support
    // joystick button for range inputs, and it doesn't really make sense. Maybe we could add support
    // for it in the future.

    return false;
}

/// Check if a mouse wheel direction is used in the InputContext
/// \param mouseWheelAxis   The axis of the mouse wheel to check
/// \param eventType        The type of event to check
/// \return true if the mouse wheel direction button is used
bool InputContext::IsMouseWheelUsed(sf::Mouse::Wheel mouseWheelAxis, EventType eventType) const
{
    // Action inputs
    for (const auto& input : m_actionInputs)
    {
        // Since a state input can be bound to four different mouse wheel direction, we need to check them all
        switch (eventType)
        {
        case EventType::Ascending:
            if (mouseWheelAxis == sf::Mouse::Wheel::HorizontalWheel && input.GetEventType() == ActionInputBinds::MouseWheelRight)
            {
                return true;
            }
            else if (mouseWheelAxis == sf::Mouse::Wheel::VerticalWheel && input.GetEventType() == ActionInputBinds::MouseWheelUp)
            {
                return true;
            }
            break;
        case EventType::Descending:
            if (mouseWheelAxis == sf::Mouse::Wheel::HorizontalWheel && input.GetEventType() == ActionInputBinds::MouseWheelLeft)
            {
                return true;
            }
            else if (mouseWheelAxis == sf::Mouse::Wheel::VerticalWheel && input.GetEventType() == ActionInputBinds::MouseWheelDown)
            {
                return true;
            }
            break;
        case EventType::Any:
            if (mouseWheelAxis == sf::Mouse::Wheel::HorizontalWheel &&
               (input.GetEventType() == ActionInputBinds::MouseWheelRight || input.GetEventType() == ActionInputBinds::MouseWheelLeft))
            {
                return true;
            }
            if (mouseWheelAxis == sf::Mouse::Wheel::VerticalWheel &&
               (input.GetEventType() == ActionInputBinds::MouseWheelUp || input.GetEventType() == ActionInputBinds::MouseWheelDown))
            {
                return true;
            }
            break;
        }
    }

    // There is no need to look for a mouse wheel event in m_stateInputs since the engine doesn't support
    // mousewheel button for state inputs, and it doesn't really make sense.

    // Range inputs
    for (const auto& input : m_rangeInputs)
    {
        if (mouseWheelAxis == sf::Mouse::Wheel::VerticalWheel && input.GetEventType() == RangeInputBinds::VerticalMouseWheel)
        {
            return true;
        }
        if (mouseWheelAxis == sf::Mouse::Wheel::HorizontalWheel && input.GetEventType() == RangeInputBinds::HorizontalMouseWheel)
        {
            return true;
        }
    }

    return false;
}

/// Check if a mouse movement is used in the InputContext
/// \return true if the mouse movement is used
bool InputContext::IsMouseMovementUsed() const
{
    // Action inputs
    for (const auto& input : m_actionInputs)
    {
        // If one of the inputs in m_acionInputs is a mouse moved
        if (input.GetEventType() == ActionInputBinds::MouseMoved)
        {
            return true;
        }
    }

    // No state inputs

    // Range input
    for (const auto& input : m_rangeInputs)
    {
        // If one of the inputs in m_acionInputs is a mouse moved
        if (input.GetEventType() == RangeInputBinds::HorizontalMouseMovement || input.GetEventType() == RangeInputBinds::VertialMouseMovement)
        {
            return true;
        }
    }

    return false;
}

void InputContext::Update(const InputManager& inputManager)
{
    for (const auto& input : m_actionInputs)
    {
        input.CheckIsTriggered(inputManager);
    }

    for (const auto& input : m_stateInputs)
    {
        input.CheckIsTriggered(inputManager);
    }

    for (const auto& input : m_rangeInputs)
    {
        input.CheckIsTriggered(inputManager);
    }
}
