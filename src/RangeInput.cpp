#include "Core/Input/RangeInput.h"
#include <algorithm>

// RangeInput

RangeInput::RangeInput(const InputManager& inputManager, Callback<float>* callback)
    : m_inputManager(inputManager)
    , m_callback(callback)
{
}

RangeInput::~RangeInput()
{
    delete m_callback;
}

// JoystickAxisBidirectionalRangeInput

JoystickAxisBidirectionalRangeInput::JoystickAxisBidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                         unsigned int joystick, sf::Joystick::Axis axis)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_axis(axis)
    , m_lastAxisPosition(0.0)
{
}

bool JoystickAxisBidirectionalRangeInput::DetectedEvent() const
{
    float currentAxisPosition = m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis);
    bool detectedEvent = currentAxisPosition != m_lastAxisPosition;
    m_lastAxisPosition = currentAxisPosition;
    return detectedEvent;
}

void JoystickAxisBidirectionalRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis));
}

// JoystickAxisUnidirectionalRangeInput

JoystickAxisUnidirectionalRangeInput::JoystickAxisUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                           unsigned int joystick, sf::Joystick::Axis axis, bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_axis(axis)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_lastAxisPosition(0.0)
{
}

bool JoystickAxisUnidirectionalRangeInput::DetectedEvent() const
{
    float currentAxisPosition = m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis);

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous axis position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentAxisPosition != m_lastAxisPosition && (currentAxisPosition > 0 || m_lastAxisPosition > 0);
    }
    else
    {
        detectedEvent = currentAxisPosition != m_lastAxisPosition && (currentAxisPosition < 0 || m_lastAxisPosition < 0);        
    }
    m_lastAxisPosition = currentAxisPosition;
    return detectedEvent;
}

void JoystickAxisUnidirectionalRangeInput::CallFunction()
{
    // The reason of this code:
    // If a certain tick, the joystick axis value is positive
    // and the very next tick, the joystick axis value is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0.0f, m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis));
    }
    else
    {
        valueToBeSent = std::min(0.0f, m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis));        
    }
    
    (*m_callback)(valueToBeSent);
}

// VerticalMouseWheelBidirectionalRangeInput

VerticalMouseWheelBidirectionalRangeInput::VerticalMouseWheelBidirectionalRangeInput(const InputManager & inputManager, Callback<float>* callback)
    : RangeInput(inputManager, callback)
    , m_lastVertScroll(0.0)
{
}

bool VerticalMouseWheelBidirectionalRangeInput::DetectedEvent() const
{
    float currentVertScroll = m_inputManager.GetVerticalMouseWheelDelta();
    bool detectedEvent = currentVertScroll != m_lastVertScroll;
    m_lastVertScroll = currentVertScroll;
    return detectedEvent;
}

void VerticalMouseWheelBidirectionalRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetVerticalMouseWheelDelta());
}

// VerticalMouseWheelUnidirectionalRangeInput

VerticalMouseWheelUnidirectionalRangeInput::VerticalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                                       bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_lastVertScroll(0.0)
{
}

bool VerticalMouseWheelUnidirectionalRangeInput::DetectedEvent() const
{
    float currentVertScroll = m_inputManager.GetVerticalMouseWheelDelta();

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse whell position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentVertScroll != m_lastVertScroll && (currentVertScroll > 0 || m_lastVertScroll > 0);
    }
    else
    {
        detectedEvent = currentVertScroll != m_lastVertScroll && (currentVertScroll < 0 || m_lastVertScroll < 0);        
    }
    m_lastVertScroll = currentVertScroll;
    return detectedEvent;
}

void VerticalMouseWheelUnidirectionalRangeInput::CallFunction()
{
    // The reason of this code:
    // If a certain tick, the mouse wheel delta is positive
    // and the very next tick, the mouse wheel delta is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0.0f, m_inputManager.GetVerticalMouseWheelDelta());
    }
    else
    {
        valueToBeSent = std::min(0.0f, m_inputManager.GetVerticalMouseWheelDelta());        
    }
    
    (*m_callback)(valueToBeSent);
}

// HorizontalMouseWheelBidirectionalRangeInput

HorizontalMouseWheelBidirectionalRangeInput::HorizontalMouseWheelBidirectionalRangeInput(const InputManager & inputManager, Callback<float>* callback)
    : RangeInput(inputManager, callback)
    , m_lastHorizScroll(0.0)
{
}

bool HorizontalMouseWheelBidirectionalRangeInput::DetectedEvent() const
{
    float currentHorizScroll = m_inputManager.GetHorizontalMouseWheelDelta();
    bool detectedEvent = currentHorizScroll != m_lastHorizScroll;
    m_lastHorizScroll = currentHorizScroll;
    return detectedEvent;
}

void HorizontalMouseWheelBidirectionalRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetHorizontalMouseWheelDelta());
}

// HorizontalMouseWheelUnidirectionalRangeInput

HorizontalMouseWheelUnidirectionalRangeInput::HorizontalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                                           bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_lastHorizScroll(0.0)
{
}

bool HorizontalMouseWheelUnidirectionalRangeInput::DetectedEvent() const
{
    float currentHorizScroll = m_inputManager.GetHorizontalMouseWheelDelta();

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse whell position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentHorizScroll != m_lastHorizScroll && (currentHorizScroll > 0 || m_lastHorizScroll > 0);
    }
    else
    {
        detectedEvent = currentHorizScroll != m_lastHorizScroll && (currentHorizScroll < 0 || m_lastHorizScroll < 0);        
    }
    m_lastHorizScroll = currentHorizScroll;
    return detectedEvent;
}

void HorizontalMouseWheelUnidirectionalRangeInput::CallFunction()
{
    // The reason of this code:
    // If a certain tick, the mouse wheel delta is positive
    // and the very next tick, the mouse wheel delta is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0.0f, m_inputManager.GetHorizontalMouseWheelDelta());
    }
    else
    {
        valueToBeSent = std::min(0.0f, m_inputManager.GetHorizontalMouseWheelDelta());        
    }
    
    (*m_callback)(valueToBeSent);
}

// VerticalMouseMovementBidirectionalRangeInput

VerticalMouseMovementBidirectionalRangeInput::VerticalMouseMovementBidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback)
    : RangeInput(inputManager, callback)
    , m_lastVertMouseMovement(0.0)
{
}

bool VerticalMouseMovementBidirectionalRangeInput::DetectedEvent() const
{
    float currentVertMouseMovement = m_inputManager.GetMousePositionDelta().y;
    bool detectedEvent = currentVertMouseMovement != m_lastVertMouseMovement;
    m_lastVertMouseMovement = currentVertMouseMovement;
    return detectedEvent;
}

void VerticalMouseMovementBidirectionalRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetMousePositionDelta().y);
}

// VerticalMouseMovementUnidirectionalRangeInput

VerticalMouseMovementUnidirectionalRangeInput::VerticalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                                             bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_lastVertMouseMovement(0.0)
{
}

bool VerticalMouseMovementUnidirectionalRangeInput::DetectedEvent() const
{
    float currentVertMouseMovement = m_inputManager.GetMousePositionDelta().y;

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse whell position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentVertMouseMovement != m_lastVertMouseMovement && (currentVertMouseMovement > 0 || m_lastVertMouseMovement > 0);
    }
    else
    {
        detectedEvent = currentVertMouseMovement != m_lastVertMouseMovement && (currentVertMouseMovement < 0 || m_lastVertMouseMovement < 0);        
    }
    m_lastVertMouseMovement = currentVertMouseMovement;
    return detectedEvent;
}

void VerticalMouseMovementUnidirectionalRangeInput::CallFunction()
{
    // The reason of this code:
    // If a certain tick, the mouse movement is positive
    // and the very next tick, the mouse movement is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0, m_inputManager.GetMousePositionDelta().y);
    }
    else
    {
        valueToBeSent = std::min(0, m_inputManager.GetMousePositionDelta().y);        
    }
    
    (*m_callback)(valueToBeSent);
}

// HorizontalMouseMovementBidirectionalRangeInput

HorizontalMouseMovementBidirectionalRangeInput::HorizontalMouseMovementBidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback)
    : RangeInput(inputManager, callback)
    , m_lastHorizMouseMovement(0.0)
{
}

bool HorizontalMouseMovementBidirectionalRangeInput::DetectedEvent() const
{
    float currentHorizMouseMovement = m_inputManager.GetMousePositionDelta().y;
    bool detectedEvent = currentHorizMouseMovement != m_lastHorizMouseMovement;
    m_lastHorizMouseMovement = currentHorizMouseMovement;
    return detectedEvent;
}

void HorizontalMouseMovementBidirectionalRangeInput::CallFunction()
{
    (*m_callback)(m_inputManager.GetMousePositionDelta().x);
}

// HorizontalMouseMovementUnidirectionalRangeInput

HorizontalMouseMovementUnidirectionalRangeInput::HorizontalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                                                 bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_lastHorizMouseMovement(0.0)
{
}

bool HorizontalMouseMovementUnidirectionalRangeInput::DetectedEvent() const
{
    float currentHorizMouseMovement = m_inputManager.GetMousePositionDelta().x;

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse whell position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentHorizMouseMovement != m_lastHorizMouseMovement && (currentHorizMouseMovement > 0 || m_lastHorizMouseMovement > 0);
    }
    else
    {
        detectedEvent = currentHorizMouseMovement != m_lastHorizMouseMovement && (currentHorizMouseMovement < 0 || m_lastHorizMouseMovement < 0);        
    }
    m_lastHorizMouseMovement = currentHorizMouseMovement;
    return detectedEvent;
}

void HorizontalMouseMovementUnidirectionalRangeInput::CallFunction()
{
    // The reason of this code:
    // If a certain tick, the mouse movement is positive
    // and the very next tick, the mouse movement is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0, m_inputManager.GetMousePositionDelta().x);
    }
    else
    {
        valueToBeSent = std::min(0, m_inputManager.GetMousePositionDelta().x);        
    }
    
    (*m_callback)(valueToBeSent);
}

// KeyboardBidirectionalRangeInput

/// Constructor
/// \param inputManager     A const reference to the InputManager
/// \param callback         The object containing the callback to call when input is triggered.
/// \param negativeKey      The key wich will send -100 to the callback when pressed.
/// \param positiveKey      The key wich will send +100 to the callback when pressed.
KeyboardBidirectionalRangeInput::KeyboardBidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                 sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
    : RangeInput(inputManager, callback)
    , m_negativeKey(negativeKey)
    , m_positiveKey(positiveKey)
{
}

bool KeyboardBidirectionalRangeInput::DetectedEvent() const
{
    return m_inputManager.IsKeyAscending(m_negativeKey) || m_inputManager.IsKeyAscending(m_positiveKey) ||
           m_inputManager.IsKeyDescending(m_negativeKey) || m_inputManager.IsKeyDescending(m_positiveKey);
}

void KeyboardBidirectionalRangeInput::CallFunction()
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

/// Constructor
/// \param inputManager     A const reference to the InputManager
/// \param callback         The object containing the callback to call when input is triggered.
/// \param negativeKey      The key wich will send -100 to the callback when pressed.
/// \param positiveKey      The key wich will send +100 to the callback when pressed.
KeyboardUnidirectionalRangeInput::KeyboardUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                   sf::Keyboard::Key key)
    : RangeInput(inputManager, callback)
    , m_key(key)
{
}

bool KeyboardUnidirectionalRangeInput::DetectedEvent() const
{
    return m_inputManager.IsKeyDescending(m_key) || m_inputManager.IsKeyAscending(m_key);
}

void KeyboardUnidirectionalRangeInput::CallFunction()
{
    if (m_inputManager.IsKeyHeld(m_key))
    {
        (*m_callback)(100.0f);
    }
    else
    {
        (*m_callback)(0.0f);
    }
}

JoystickButtonBidirectionalRangeInput::JoystickButtonBidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                             unsigned int joystick, unsigned int negativeJoystickButton,
                                                                             unsigned int positiveJoystickButton)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_negativeJoystickButton(negativeJoystickButton)
    , m_positiveJoystickButton(positiveJoystickButton)
{
}

bool JoystickButtonBidirectionalRangeInput::DetectedEvent() const
{
    return m_inputManager.IsJoystickButtonDescending(m_joystick, m_negativeJoystickButton) ||
           m_inputManager.IsJoystickButtonDescending(m_joystick, m_positiveJoystickButton) ||
           m_inputManager.IsJoystickButtonAscending(m_joystick, m_negativeJoystickButton) ||
           m_inputManager.IsJoystickButtonAscending(m_joystick, m_positiveJoystickButton);
}

void JoystickButtonBidirectionalRangeInput::CallFunction()
{
    bool isNegativeButtonHeld = m_inputManager.IsJoystickButtonHeld(m_joystick, m_negativeJoystickButton);
    bool isPositiveButtonHeld = m_inputManager.IsJoystickButtonHeld(m_joystick, m_positiveJoystickButton);

    // If only negative key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isNegativeButtonHeld == true && isPositiveButtonHeld == false)
    {
        (*m_callback)(-100.0f);
        return;
    }

    // If only positive key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isPositiveButtonHeld == true && isNegativeButtonHeld == false)
    {
        (*m_callback)(100.0f);
        return;
    }

    (*m_callback)(0.0f);
}

JoystickButtonUnidirectionalRangeInput::JoystickButtonUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                               unsigned int joystick, unsigned int button)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_button(button)
{
}

bool JoystickButtonUnidirectionalRangeInput::DetectedEvent() const
{
    return m_inputManager.IsJoystickButtonDescending(m_joystick, m_button) ||
           m_inputManager.IsJoystickButtonAscending(m_joystick, m_button);
}

void JoystickButtonUnidirectionalRangeInput::CallFunction()
{
    if (m_inputManager.IsJoystickButtonHeld(m_joystick, m_button))
    {
        (*m_callback)(100.0f);
    }
    else
    {
        (*m_callback)(0.0f);
    }
}

// MouseButtonBidirectionalRangeInput

MouseButtonBidirectionalRangeInput::MouseButtonBidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                       sf::Mouse::Button negativeMouseButton, sf::Mouse::Button positiveMouseButton)
    : RangeInput(inputManager, callback)
    , m_negativeMouseButton(negativeMouseButton)
    , m_positiveMouseButton(positiveMouseButton)
{
}

bool MouseButtonBidirectionalRangeInput::DetectedEvent() const
{
    return m_inputManager.IsMouseButtonAscending(m_negativeMouseButton) || m_inputManager.IsMouseButtonAscending(m_positiveMouseButton) ||
           m_inputManager.IsMouseButtonDescending(m_negativeMouseButton) || m_inputManager.IsMouseButtonDescending(m_positiveMouseButton);
}

void MouseButtonBidirectionalRangeInput::CallFunction()
{
    bool isNegativeButtonHeld = m_inputManager.IsMouseButtonHeld(m_negativeMouseButton);
    bool isPositiveButtonHeld = m_inputManager.IsMouseButtonHeld(m_positiveMouseButton);

    // If only negative key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isNegativeButtonHeld == true && isPositiveButtonHeld == false)
    {
        (*m_callback)(-100.0f);
        return;
    }

    // If only positive key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isPositiveButtonHeld == true && isNegativeButtonHeld == false)
    {
        (*m_callback)(100.0f);
        return;
    }

    (*m_callback)(0.0f);
}

// MouseButtonUnidirectionalRangeInput

MouseButtonUnidirectionalRangeInput::MouseButtonUnidirectionalRangeInput(const InputManager& inputManager, Callback<float>* callback,
                                                                         sf::Mouse::Button mouseButton)
    : RangeInput(inputManager, callback)
    , m_mouseButton(mouseButton)
{
}

bool MouseButtonUnidirectionalRangeInput::DetectedEvent() const
{
    return m_inputManager.IsMouseButtonAscending(m_mouseButton) || m_inputManager.IsMouseButtonDescending(m_mouseButton);
}

void MouseButtonUnidirectionalRangeInput::CallFunction()
{
    if (m_inputManager.IsMouseButtonHeld(m_mouseButton))
    {
        (*m_callback)(100.0f);
    }
    else
    {
        (*m_callback)(0.0f);
    }
}
