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

void ActionInput::callFunction()
{
    (*m_callback)();
}

// KeyDescendingActionInput

KeyDescendingActionInput::KeyDescendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Keyboard::Key key)
    : ActionInput(inputManager, callback)
    , m_key(key)
{
}

bool KeyDescendingActionInput::detectedEvent() const
{
    return m_inputManager.isKeyDescending(m_key);
}

// KeyAscendingActionInput
KeyAscendingActionInput::KeyAscendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Keyboard::Key key)
    : ActionInput(inputManager, callback)
    , m_key(key)
{
}

bool KeyAscendingActionInput::detectedEvent() const
{
    return m_inputManager.isKeyAscending(m_key);
}

// MouseButtonDescendingActionInput
MouseButtonDescendingActionInput::MouseButtonDescendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Mouse::Button button)
    : ActionInput(inputManager, callback)
    , m_button(button)
{
}

bool MouseButtonDescendingActionInput::detectedEvent() const
{
    return m_inputManager.isMouseButtonDescending(m_button);
}

// MouseButtonAscendingActionInput

MouseButtonAscendingActionInput::MouseButtonAscendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Mouse::Button button)
    : ActionInput(inputManager, callback)
    , m_button(button)
{
}

bool MouseButtonAscendingActionInput::detectedEvent() const
{
    return m_inputManager.isMouseButtonAscending(m_button);
}

// JoystickButtonDescendingActionInput

JoystickButtonDescendingActionInput::JoystickButtonDescendingActionInput(const InputManager& inputManager, Callable<>* callback, unsigned int joystick, unsigned int button)
    : ActionInput(inputManager, callback)
    , m_joystick(joystick)
    , m_button(button)
{
}

bool JoystickButtonDescendingActionInput::detectedEvent() const
{
    return m_inputManager.isJoystickButtonDescending(m_joystick, m_button);
}

// JoystickButtonAscendingActionInput

JoystickButtonAscendingActionInput::JoystickButtonAscendingActionInput(const InputManager& inputManager, Callable<>* callback, unsigned int joystick, unsigned int button)
    : ActionInput(inputManager, callback)
    , m_joystick(joystick)
    , m_button(button)
{
}

bool JoystickButtonAscendingActionInput::detectedEvent() const
{
    return m_inputManager.isJoystickButtonAscending(m_joystick, m_button);
}

// MouseMovedActionInput

MouseMovedActionInput::MouseMovedActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseMovedActionInput::detectedEvent() const
{
    return m_inputManager.detectedMouseMovedEvent();
}

// MouseWheelUpActionInput

MouseWheelUpActionInput::MouseWheelUpActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelUpActionInput::detectedEvent() const
{
    return m_inputManager.detectedMouseWheelScrolledEvent() && m_inputManager.getMouseWheelDelta().y > 0;
}

// MouseWheelDownActionInput

MouseWheelDownActionInput::MouseWheelDownActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelDownActionInput::detectedEvent() const
{
    return m_inputManager.detectedMouseWheelScrolledEvent() && m_inputManager.getMouseWheelDelta().y < 0;
}

// MouseWheelLeftActionInput

MouseWheelLeftActionInput::MouseWheelLeftActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelLeftActionInput::detectedEvent() const
{
    return m_inputManager.detectedMouseWheelScrolledEvent() && m_inputManager.getMouseWheelDelta().x < 0;
}

// MouseWheelRightActionInput

MouseWheelRightActionInput::MouseWheelRightActionInput(const InputManager& inputManager, Callable<>* callback)
    : ActionInput(inputManager, callback)
{
}

bool MouseWheelRightActionInput::detectedEvent() const
{
    return m_inputManager.detectedMouseWheelScrolledEvent() && m_inputManager.getMouseWheelDelta().x > 0;
}
