#include "RangeInput.h"

// RangeInput

RangeInput::RangeInput(const InputManager& inputManager, Callback<double>* callback)
    : m_inputManager(inputManager)
    , m_callback(callback)
{
}

RangeInput::~RangeInput()
{
    delete m_callback;
}

// JoystickAxisRangeInput

JoystickAxisRangeInput::JoystickAxisRangeInput(const InputManager& inputManager, Callback<double>* callback, unsigned int joystick, sf::Joystick::Axis axis)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_axis(axis)
    , m_lastAxisPosition(0)
{
}

bool JoystickAxisRangeInput::DetectedEvent() const
{
    return m_inputManager.DetectedJoystickMovedEvent();
}

void JoystickAxisRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis));
}

// VerticalMouseWheelRangeInput

VerticalMouseWheelRangeInput::VerticalMouseWheelRangeInput(const InputManager & inputManager, Callback<double>* callback)
    : RangeInput(inputManager, callback)
{
}

bool VerticalMouseWheelRangeInput::DetectedEvent() const
{
    float currentVertScroll = m_inputManager.GetVerticalMouseWheelDelta();
    bool detectedEvent = currentVertScroll != m_lastVertScroll;
    m_lastVertScroll = currentVertScroll;
    return detectedEvent;
}

void VerticalMouseWheelRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetVerticalMouseWheelDelta());
}

// HorizontalMouseWheelRangeInput
HorizontalMouseWheelRangeInput::HorizontalMouseWheelRangeInput(const InputManager & inputManager, Callback<double>* callback)
    : RangeInput(inputManager, callback)
{
}

bool HorizontalMouseWheelRangeInput::DetectedEvent() const
{
    float currentHorizScroll = m_inputManager.GetHorizontalMouseWheelDelta();
    bool detectedEvent = currentHorizScroll != m_lastHorizScroll;
    m_lastHorizScroll = currentHorizScroll;
    return detectedEvent;
}

void HorizontalMouseWheelRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetHorizontalMouseWheelDelta());
}

// VerticalMouseMovementRangeInput

VerticalMouseMovementRangeInput::VerticalMouseMovementRangeInput(const InputManager& inputManager, Callback<double>* callback)
    : RangeInput(inputManager, callback)
{
}

bool VerticalMouseMovementRangeInput::DetectedEvent() const
{
    float currentVertMouseMovement = m_inputManager.GetMousePositionDelta().y;
    bool detectedEvent = currentVertMouseMovement != m_lastVertMouseMovement;
    m_lastVertMouseMovement = currentVertMouseMovement;
    return detectedEvent;
}

void VerticalMouseMovementRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetMousePositionDelta().y);
}

// HorizontalMouseMovementRangeInput

HorizontalMouseMovementRangeInput::HorizontalMouseMovementRangeInput(const InputManager& inputManager, Callback<double>* callback)
    : RangeInput(inputManager, callback)
{
}

bool HorizontalMouseMovementRangeInput::DetectedEvent() const
{
    float currentHorizMouseMovement = m_inputManager.GetMousePositionDelta().y;
    bool detectedEvent = currentHorizMouseMovement != m_lastHorizMouseMovement;
    m_lastHorizMouseMovement = currentHorizMouseMovement;
    return detectedEvent;
}

void HorizontalMouseMovementRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetMousePositionDelta().x);
}

// KeyboardRangeInput

/// Constructor
/// \param inputManager     A const reference to the InputManager
/// \param callback         The object containing the callback to call when input is triggered.
/// \param negativeKey      The key wich will send -100 to the callback when pressed.
/// \param positiveKey      The key wich will send +100 to the callback when pressed.
KeyboardRangeInput::KeyboardRangeInput(const InputManager& inputManager, Callback<double>* callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
    : RangeInput(inputManager, callback)
    , m_negativeKey(negativeKey)
    , m_positiveKey(positiveKey)
{
}

bool KeyboardRangeInput::DetectedEvent() const
{
    return m_inputManager.IsKeyAscending(m_negativeKey) || m_inputManager.IsKeyAscending(m_positiveKey) ||
           m_inputManager.IsKeyDescending(m_negativeKey) || m_inputManager.IsKeyDescending(m_positiveKey);
}

void KeyboardRangeInput::CallFunction()
{
    bool isNegativeKeyHeld = m_inputManager.IsKeyHeld(m_negativeKey);
    bool isPositiveKeyHeld = m_inputManager.IsKeyHeld(m_positiveKey);

    // If only negative key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isNegativeKeyHeld == true && isPositiveKeyHeld == false)
    {
        (*m_callback)(-100.0f);
        return;
    }

    // If only positive key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isPositiveKeyHeld == true && isNegativeKeyHeld == false)
    {
        (*m_callback)(100.0f);
        return;
    }

    (*m_callback)(0.0f);
}