#include "Core/Input/StateInput.h"
#include <cmath>

// StateInput

StateInput::StateInput(const InputManager& inputManager, Callable<bool>* callback)
    : m_inputManager(inputManager)
    , m_callback(callback)
{
}

StateInput::~StateInput()
{
    delete m_callback;
}

// KeyEventStateInput

KeyEventStateInput::KeyEventStateInput(const InputManager& inputManager, Callable<bool>* callback, sf::Keyboard::Key key)
    : StateInput(inputManager, callback)
    , m_key(key)
{
}

bool KeyEventStateInput::detectedEvent() const
{
    return m_inputManager.isKeyAscending(m_key) || m_inputManager.isKeyDescending(m_key);
}

void KeyEventStateInput::callAction()
{
    (*m_callback)(m_inputManager.isKeyHeld(m_key));
}

// MouseButtonEventStateInput

MouseButtonEventStateInput::MouseButtonEventStateInput(const InputManager& inputManager, Callable<bool>* callback, sf::Mouse::Button button)
    : StateInput(inputManager, callback)
    , m_button(button)
{
}

bool MouseButtonEventStateInput::detectedEvent() const
{
    return m_inputManager.isMouseButtonAscending(m_button) || m_inputManager.isMouseButtonDescending(m_button);
}

void MouseButtonEventStateInput::callAction()
{
    bool mouseButtonDescending = m_inputManager.isMouseButtonAscending(m_button);
    (*m_callback)(mouseButtonDescending);
}

// JoystickButtonEventStateInput

JoystickButtonEventStateInput::JoystickButtonEventStateInput(const InputManager& inputManager, Callable<bool>* callback, unsigned int joystick, unsigned int button)
    : StateInput(inputManager, callback)
    , m_joystick(joystick)
    , m_button(button)
{
}

bool JoystickButtonEventStateInput::detectedEvent() const
{
    return m_inputManager.isJoystickButtonAscending(m_joystick, m_button) || m_inputManager.isJoystickButtonDescending(m_joystick, m_button);
}

void JoystickButtonEventStateInput::callAction()
{
    bool joystickButtonDescending = m_inputManager.isJoystickButtonDescending(m_joystick, m_button);
    (*m_callback)(joystickButtonDescending);
}

// JoystickAxisAboveThresholdStateInput

/// Constructor
/// \param inputManager     A const reference to the InputManager.
/// \param callback         The object containing the callback to call when input is triggered.
/// \param joystick         The id of the joystick controller.
/// \param threshold        The threshold to be exceeded for the callback to be called.
///                         If the threshold is negative, the axis value must be lower for the input to be triggered.
///                         If the threshold is positive, the axis value must be higher for the input to be triggered.
JoystickAxisAboveThresholdStateInput::JoystickAxisAboveThresholdStateInput(const InputManager& inputManager, Callable<bool>* callback,
                                                                           unsigned int joystick, sf::Joystick::Axis axis, float threshold)
    : StateInput(inputManager, callback)
    , m_joystick(joystick)
    , m_axis(axis)
    , m_threshold(threshold)
    , m_lastAxisValue(0)
{
}

bool JoystickAxisAboveThresholdStateInput::detectedEvent() const
{
    float currentAxisValue = m_inputManager.getJoystickAxisPosition(m_joystick, m_axis);
    // Return if the current axis position just passed the threshold
    bool detectedEvent = (m_lastAxisValue <= m_threshold && currentAxisValue > m_threshold) ||
                         (m_lastAxisValue > m_threshold && currentAxisValue <= m_threshold);

    m_lastAxisValue = currentAxisValue;
    return detectedEvent;
}

void JoystickAxisAboveThresholdStateInput::callAction()
{
    float currentAxisValue = m_inputManager.getJoystickAxisPosition(m_joystick, m_axis);
    (*m_callback)(currentAxisValue > m_threshold);
}

// JoystickAxisAboveThresholdStateInput

/// Constructor
/// \param inputManager     A const reference to the InputManager.
/// \param callback         The object containing the callback to call when input is triggered.
/// \param joystick         The id of the joystick controller.
/// \param threshold        The threshold to be exceeded for the callback to be called.
///                         If the threshold is negative, the axis value must be lower for the input to be triggered.
///                         If the threshold is positive, the axis value must be higher for the input to be triggered.
JoystickAxisBelowThresholdStateInput::JoystickAxisBelowThresholdStateInput(const InputManager& inputManager, Callable<bool>* callback,
                                                                         unsigned int joystick, sf::Joystick::Axis axis, float threshold)
    : StateInput(inputManager, callback)
    , m_joystick(joystick)
    , m_axis(axis)
    , m_threshold(std::abs(threshold))
    , m_lastAxisValue(0)
{
}

bool JoystickAxisBelowThresholdStateInput::detectedEvent() const
{
    float currentAxisValue = m_inputManager.getJoystickAxisPosition(m_joystick, m_axis);
    // Return if the current axis position just passed the threshold
    bool detectedEvent = (m_lastAxisValue < m_threshold && currentAxisValue >= m_threshold) ||
                         (m_lastAxisValue >= m_threshold && currentAxisValue < m_threshold);

    m_lastAxisValue = currentAxisValue;
    return detectedEvent;
}

void JoystickAxisBelowThresholdStateInput::callAction()
{
    float currentAxisValue = m_inputManager.getJoystickAxisPosition(m_joystick, m_axis);
    (*m_callback)(currentAxisValue < m_threshold);
}
