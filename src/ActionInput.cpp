#include "ActionInput.h"

// ActionInput

ActionInput::ActionInput(const InputManager& inputManager, Callable<>* callback)
    : m_inputManager(inputManager)
    , m_callback(callback)
{
}

ActionInput::~ActionInput()
{
    delete m_callback;
}

void ActionInput::CallFunction()
{
    (*m_callback)();
}

// KeyDescendingActionInput

KeyDescendingActionInput::KeyDescendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Keyboard::Key key)
    : ActionInput(inputManager, callback)
    , m_key(key)
{
}

bool KeyDescendingActionInput::DetectedEvent() const
{
    return m_inputManager.IsKeyDescending(m_key);
}

// KeyAscendingActionInput
KeyAscendingActionInput::KeyAscendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Keyboard::Key key)
    : ActionInput(inputManager, callback)
    , m_key(key)
{
}

bool KeyAscendingActionInput::DetectedEvent() const
{
    return m_inputManager.IsKeyAscending(m_key);
}

// MouseButtonDescendingActionInput
MouseButtonDescendingActionInput::MouseButtonDescendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Mouse::Button button)
    : ActionInput(inputManager, callback)
    , m_button(button)
{
}

bool MouseButtonDescendingActionInput::DetectedEvent() const
{
    return m_inputManager.IsMouseButtonDescending(m_button);
}

// MouseButtonAscendingActionInput

MouseButtonAscendingActionInput::MouseButtonAscendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Mouse::Button button)
    : ActionInput(inputManager, callback)
    , m_button(button)
{
}

bool MouseButtonAscendingActionInput::DetectedEvent() const
{
    return m_inputManager.IsMouseButtonAscending(m_button);
}

// JoystickButtonDescendingActionInput

JoystickButtonDescendingActionInput::JoystickButtonDescendingActionInput(const InputManager& inputManager, Callable<>* callback, unsigned int joystick, unsigned int button)
    : ActionInput(inputManager, callback)
    , m_joystick(joystick)
    , m_button(button)
{
}

bool JoystickButtonDescendingActionInput::DetectedEvent() const
{
    return m_inputManager.IsJoystickButtonDescending(m_joystick, m_button);
}

// JoystickButtonAscendingActionInput

JoystickButtonAscendingActionInput::JoystickButtonAscendingActionInput(const InputManager& inputManager, Callable<>* callback, unsigned int joystick, unsigned int button)
    : ActionInput(inputManager, callback)
    , m_joystick(joystick)
    , m_button(button)
{
}

bool JoystickButtonAscendingActionInput::DetectedEvent() const
{
    return m_inputManager.IsJoystickButtonAscending(m_joystick, m_button);
}

// MouseMovedActionInput

MouseMovedActionInput::MouseMovedActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseMovedActionInput::DetectedEvent() const
{
    return m_inputManager.DetectedMouseMovedEvent();
}

// MouseWheelUpActionInput

MouseWheelUpActionInput::MouseWheelUpActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelUpActionInput::DetectedEvent() const
{
    return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetMouseWheelDelta().y > 0;
}

// MouseWheelDownActionInput

MouseWheelDownActionInput::MouseWheelDownActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelDownActionInput::DetectedEvent() const
{
    return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetMouseWheelDelta().y < 0;
}

// MouseWheelLeftActionInput

MouseWheelLeftActionInput::MouseWheelLeftActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelLeftActionInput::DetectedEvent() const
{
    return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetMouseWheelDelta().x < 0;
}

// MouseWheelRightActionInput

MouseWheelRightActionInput::MouseWheelRightActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelRightActionInput::DetectedEvent() const
{
    return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetMouseWheelDelta().x > 0;
}
