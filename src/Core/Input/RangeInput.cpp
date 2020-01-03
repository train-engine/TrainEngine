#include "Core/Input/RangeInput.h"
#include <algorithm>

// RangeInput

RangeInput::RangeInput(const InputManager& inputManager, Callable<float>* callback)
    : m_inputManager(inputManager)
    , m_callback(callback)
{
}

RangeInput::~RangeInput()
{
    delete m_callback;
}

// JoystickAxisBidirectionalRangeInput

JoystickAxisBidirectionalRangeInput::JoystickAxisBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                         unsigned int joystick, sf::Joystick::Axis axis)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_axis(axis)
    , m_previousAxisPosition(0.0)
{
}

bool JoystickAxisBidirectionalRangeInput::detectedEvent() const
{
    float currentAxisPosition = m_inputManager.getJoystickAxisPosition(m_joystick, m_axis);
    bool detectedEvent = currentAxisPosition != m_previousAxisPosition;
    m_previousAxisPosition = currentAxisPosition;
    return detectedEvent;
}

void JoystickAxisBidirectionalRangeInput::callFunction()
{
    (*m_callback)(m_inputManager.getJoystickAxisPosition(m_joystick, m_axis));
}

// JoystickAxisUnidirectionalRangeInput

JoystickAxisUnidirectionalRangeInput::JoystickAxisUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                           unsigned int joystick, sf::Joystick::Axis axis,
                                                                           bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_axis(axis)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_previousAxisPosition(0.0)
{
}

bool JoystickAxisUnidirectionalRangeInput::detectedEvent() const
{
    float currentAxisPosition = m_inputManager.getJoystickAxisPosition(m_joystick, m_axis);

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous axis position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentAxisPosition != m_previousAxisPosition && (currentAxisPosition > 0 || m_previousAxisPosition > 0);
    }
    else
    {
        detectedEvent = currentAxisPosition != m_previousAxisPosition && (currentAxisPosition < 0 || m_previousAxisPosition < 0);
    }
    m_previousAxisPosition = currentAxisPosition;
    return detectedEvent;
}

void JoystickAxisUnidirectionalRangeInput::callFunction()
{
    // The reason for this code:
    // If a certain tick, the joystick axis value is positive
    // and the very next tick, the joystick axis value is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0.0f, m_inputManager.getJoystickAxisPosition(m_joystick, m_axis));
    }
    else
    {
        valueToBeSent = std::min(0.0f, m_inputManager.getJoystickAxisPosition(m_joystick, m_axis));
    }

    (*m_callback)(valueToBeSent);
}

// VerticalMouseWheelBidirectionalRangeInput

VerticalMouseWheelBidirectionalRangeInput::VerticalMouseWheelBidirectionalRangeInput(const InputManager& inputManager,
                                                                                     Callable<float>* callback)
    : RangeInput(inputManager, callback)
    , m_previousVertScroll(0.0)
{
}

bool VerticalMouseWheelBidirectionalRangeInput::detectedEvent() const
{
    float currentVertScroll = m_inputManager.getMouseWheelDelta().y;
    bool detectedEvent = currentVertScroll != m_previousVertScroll;
    m_previousVertScroll = currentVertScroll;
    return detectedEvent;
}

void VerticalMouseWheelBidirectionalRangeInput::callFunction()
{
    (*m_callback)(m_inputManager.getMouseWheelDelta().y);
}

// VerticalMouseWheelUnidirectionalRangeInput

VerticalMouseWheelUnidirectionalRangeInput::VerticalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager,
                                                                                       Callable<float>* callback,
                                                                                       bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_previousVertScroll(0.0)
{
}

bool VerticalMouseWheelUnidirectionalRangeInput::detectedEvent() const
{
    float currentVertScroll = m_inputManager.getMouseWheelDelta().y;

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse wheel position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentVertScroll != m_previousVertScroll && (currentVertScroll > 0 || m_previousVertScroll > 0);
    }
    else
    {
        detectedEvent = currentVertScroll != m_previousVertScroll && (currentVertScroll < 0 || m_previousVertScroll < 0);
    }
    m_previousVertScroll = currentVertScroll;
    return detectedEvent;
}

void VerticalMouseWheelUnidirectionalRangeInput::callFunction()
{
    // The reason for this code:
    // If a certain tick, the mouse wheel delta is positive
    // and the very next tick, the mouse wheel delta is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0.0f, m_inputManager.getMouseWheelDelta().y);
    }
    else
    {
        valueToBeSent = std::min(0.0f, m_inputManager.getMouseWheelDelta().y);
    }

    (*m_callback)(valueToBeSent);
}

// HorizontalMouseWheelBidirectionalRangeInput

HorizontalMouseWheelBidirectionalRangeInput::HorizontalMouseWheelBidirectionalRangeInput(const InputManager& inputManager,
                                                                                         Callable<float>* callback)
    : RangeInput(inputManager, callback)
    , m_previousHorizScroll(0.0)
{
}

bool HorizontalMouseWheelBidirectionalRangeInput::detectedEvent() const
{
    float currentHorizScroll = m_inputManager.getMouseWheelDelta().x;
    bool detectedEvent = currentHorizScroll != m_previousHorizScroll;
    m_previousHorizScroll = currentHorizScroll;
    return detectedEvent;
}

void HorizontalMouseWheelBidirectionalRangeInput::callFunction()
{
    (*m_callback)(m_inputManager.getMouseWheelDelta().x);
}

// HorizontalMouseWheelUnidirectionalRangeInput

HorizontalMouseWheelUnidirectionalRangeInput::HorizontalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager,
                                                                                           Callable<float>* callback,
                                                                                           bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_previousHorizScroll(0.0)
{
}

bool HorizontalMouseWheelUnidirectionalRangeInput::detectedEvent() const
{
    float currentHorizScroll = m_inputManager.getMouseWheelDelta().x;

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse wheel position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentHorizScroll != m_previousHorizScroll && (currentHorizScroll > 0 || m_previousHorizScroll > 0);
    }
    else
    {
        detectedEvent = currentHorizScroll != m_previousHorizScroll && (currentHorizScroll < 0 || m_previousHorizScroll < 0);
    }
    m_previousHorizScroll = currentHorizScroll;
    return detectedEvent;
}

void HorizontalMouseWheelUnidirectionalRangeInput::callFunction()
{
    // The reason for this code:
    // If a certain tick, the mouse wheel delta is positive
    // and the very next tick, the mouse wheel delta is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0.0f, m_inputManager.getMouseWheelDelta().x);
    }
    else
    {
        valueToBeSent = std::min(0.0f, m_inputManager.getMouseWheelDelta().x);
    }

    (*m_callback)(valueToBeSent);
}

// VerticalMouseMovementBidirectionalRangeInput

VerticalMouseMovementBidirectionalRangeInput::VerticalMouseMovementBidirectionalRangeInput(const InputManager& inputManager,
                                                                                           Callable<float>* callback)
    : RangeInput(inputManager, callback)
    , m_previousVertMouseMovement(0.0)
{
}

bool VerticalMouseMovementBidirectionalRangeInput::detectedEvent() const
{
    float currentVertMouseMovement = m_inputManager.getMousePositionDelta().y;
    bool detectedEvent = currentVertMouseMovement != m_previousVertMouseMovement;
    m_previousVertMouseMovement = currentVertMouseMovement;
    return detectedEvent;
}

void VerticalMouseMovementBidirectionalRangeInput::callFunction()
{
    (*m_callback)(m_inputManager.getMousePositionDelta().y);
}

// VerticalMouseMovementUnidirectionalRangeInput

VerticalMouseMovementUnidirectionalRangeInput::VerticalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager,
                                                                                             Callable<float>* callback,
                                                                                             bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_previousVertMouseMovement(0.0)
{
}

bool VerticalMouseMovementUnidirectionalRangeInput::detectedEvent() const
{
    float currentVertMouseMovement = m_inputManager.getMousePositionDelta().y;

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse wheel position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent =
            currentVertMouseMovement != m_previousVertMouseMovement && (currentVertMouseMovement > 0 || m_previousVertMouseMovement > 0);
    }
    else
    {
        detectedEvent =
            currentVertMouseMovement != m_previousVertMouseMovement && (currentVertMouseMovement < 0 || m_previousVertMouseMovement < 0);
    }
    m_previousVertMouseMovement = currentVertMouseMovement;
    return detectedEvent;
}

void VerticalMouseMovementUnidirectionalRangeInput::callFunction()
{
    // The reason for this code:
    // If a certain tick, the mouse movement is positive
    // and the very next tick, the mouse movement is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0, m_inputManager.getMousePositionDelta().y);
    }
    else
    {
        valueToBeSent = std::min(0, m_inputManager.getMousePositionDelta().y);
    }

    (*m_callback)(valueToBeSent);
}

// HorizontalMouseMovementBidirectionalRangeInput

HorizontalMouseMovementBidirectionalRangeInput::HorizontalMouseMovementBidirectionalRangeInput(const InputManager& inputManager,
                                                                                               Callable<float>* callback)
    : RangeInput(inputManager, callback)
    , m_previousHorizMouseMovement(0.0)
{
}

bool HorizontalMouseMovementBidirectionalRangeInput::detectedEvent() const
{
    float currentHorizMouseMovement = m_inputManager.getMousePositionDelta().y;
    bool detectedEvent = currentHorizMouseMovement != m_previousHorizMouseMovement;
    m_previousHorizMouseMovement = currentHorizMouseMovement;
    return detectedEvent;
}

void HorizontalMouseMovementBidirectionalRangeInput::callFunction()
{
    (*m_callback)(m_inputManager.getMousePositionDelta().x);
}

// HorizontalMouseMovementUnidirectionalRangeInput

HorizontalMouseMovementUnidirectionalRangeInput::HorizontalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager,
                                                                                                 Callable<float>* callback,
                                                                                                 bool isRestrictedToPositives)
    : RangeInput(inputManager, callback)
    , m_isRestrictedToPositives(isRestrictedToPositives)
    , m_previousHorizMouseMovement(0.0)
{
}

bool HorizontalMouseMovementUnidirectionalRangeInput::detectedEvent() const
{
    float currentHorizMouseMovement = m_inputManager.getMousePositionDelta().x;

    // The logic for detected event depends on the direction
    // This prevents useless callbacks to be made if both current and previous mouse wheel position
    // are in the direction
    bool detectedEvent;
    if (m_isRestrictedToPositives)
    {
        detectedEvent = currentHorizMouseMovement != m_previousHorizMouseMovement &&
                        (currentHorizMouseMovement > 0 || m_previousHorizMouseMovement > 0);
    }
    else
    {
        detectedEvent = currentHorizMouseMovement != m_previousHorizMouseMovement &&
                        (currentHorizMouseMovement < 0 || m_previousHorizMouseMovement < 0);
    }
    m_previousHorizMouseMovement = currentHorizMouseMovement;
    return detectedEvent;
}

void HorizontalMouseMovementUnidirectionalRangeInput::callFunction()
{
    // The reason for this code:
    // If a certain tick, the mouse movement is positive
    // and the very next tick, the mouse movement is negative,
    // and the direction must be positive, we do not want the send the negative
    // value, but instead send 0.
    float valueToBeSent;
    if (m_isRestrictedToPositives)
    {
        valueToBeSent = std::max(0, m_inputManager.getMousePositionDelta().x);
    }
    else
    {
        valueToBeSent = std::min(0, m_inputManager.getMousePositionDelta().x);
    }

    (*m_callback)(valueToBeSent);
}

// KeyboardBidirectionalRangeInput

/// Constructor
/// \param inputManager     A const reference to the InputManager
/// \param callback         The object containing the callback to call when input is triggered
/// \param negativeKey      The key which will send -1.0 to the callback when pressed
/// \param positiveKey      The key which will send +1.0 to the callback when pressed
KeyboardBidirectionalRangeInput::KeyboardBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                 sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
    : RangeInput(inputManager, callback)
    , m_negativeKey(negativeKey)
    , m_positiveKey(positiveKey)
{
}

bool KeyboardBidirectionalRangeInput::detectedEvent() const
{
    return m_inputManager.isKeyAscending(m_negativeKey) || m_inputManager.isKeyAscending(m_positiveKey) ||
           m_inputManager.isKeyDescending(m_negativeKey) || m_inputManager.isKeyDescending(m_positiveKey);
}

void KeyboardBidirectionalRangeInput::callFunction()
{
    bool isNegativeKeyHeld = m_inputManager.isKeyHeld(m_negativeKey);
    bool isPositiveKeyHeld = m_inputManager.isKeyHeld(m_positiveKey);

    // If only negative key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isNegativeKeyHeld == true && isPositiveKeyHeld == false)
    {
        (*m_callback)(-1.0f);
        return;
    }

    // If only positive key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isPositiveKeyHeld == true && isNegativeKeyHeld == false)
    {
        (*m_callback)(1.0f);
        return;
    }

    (*m_callback)(0.0f);
}

/// Constructor
/// \param inputManager     A const reference to the InputManager
/// \param callback         The object containing the callback to call when input is triggered
/// \param key              The key which will send +1.0 to the callback when pressed
KeyboardUnidirectionalRangeInput::KeyboardUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                   sf::Keyboard::Key key)
    : RangeInput(inputManager, callback)
    , m_key(key)
{
}

bool KeyboardUnidirectionalRangeInput::detectedEvent() const
{
    return m_inputManager.isKeyDescending(m_key) || m_inputManager.isKeyAscending(m_key);
}

void KeyboardUnidirectionalRangeInput::callFunction()
{
    if (m_inputManager.isKeyHeld(m_key))
    {
        (*m_callback)(1.0f);
    }
    else
    {
        (*m_callback)(0.0f);
    }
}

/// Constructor
/// \param inputManager             A const reference to the InputManager
/// \param callback                 The object containing the callback to call when input is triggered
/// \param joystick                 The joystick id
/// \param negativeJoystickButton   The button which will send -1.0 to the callback when pressed
/// \param positiveJoystickButton   The button which will send +1.0 to the callback when pressed
JoystickButtonBidirectionalRangeInput::JoystickButtonBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                             unsigned int joystick, unsigned int negativeJoystickButton,
                                                                             unsigned int positiveJoystickButton)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_negativeJoystickButton(negativeJoystickButton)
    , m_positiveJoystickButton(positiveJoystickButton)
{
}

bool JoystickButtonBidirectionalRangeInput::detectedEvent() const
{
    return m_inputManager.isJoystickButtonDescending(m_joystick, m_negativeJoystickButton) ||
           m_inputManager.isJoystickButtonDescending(m_joystick, m_positiveJoystickButton) ||
           m_inputManager.isJoystickButtonAscending(m_joystick, m_negativeJoystickButton) ||
           m_inputManager.isJoystickButtonAscending(m_joystick, m_positiveJoystickButton);
}

void JoystickButtonBidirectionalRangeInput::callFunction()
{
    bool isNegativeButtonHeld = m_inputManager.isJoystickButtonHeld(m_joystick, m_negativeJoystickButton);
    bool isPositiveButtonHeld = m_inputManager.isJoystickButtonHeld(m_joystick, m_positiveJoystickButton);

    // If only negative key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isNegativeButtonHeld == true && isPositiveButtonHeld == false)
    {
        (*m_callback)(-1.0f);
        return;
    }

    // If only positive key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isPositiveButtonHeld == true && isNegativeButtonHeld == false)
    {
        (*m_callback)(1.0f);
        return;
    }

    (*m_callback)(0.0f);
}

/// Constructor
/// \param inputManager A const reference to the InputManager
/// \param callback     The object containing the callback to call when input is triggered
/// \param joystick     The joystick id
/// \param button       The button which will send +1.0 to the callback when pressed
JoystickButtonUnidirectionalRangeInput::JoystickButtonUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                               unsigned int joystick, unsigned int button)
    : RangeInput(inputManager, callback)
    , m_joystick(joystick)
    , m_button(button)
{
}

bool JoystickButtonUnidirectionalRangeInput::detectedEvent() const
{
    return m_inputManager.isJoystickButtonDescending(m_joystick, m_button) ||
           m_inputManager.isJoystickButtonAscending(m_joystick, m_button);
}

void JoystickButtonUnidirectionalRangeInput::callFunction()
{
    if (m_inputManager.isJoystickButtonHeld(m_joystick, m_button))
    {
        (*m_callback)(1.0f);
    }
    else
    {
        (*m_callback)(0.0f);
    }
}

// MouseButtonBidirectionalRangeInput

/// Constructor
/// \param inputManager         A const reference to the InputManager
/// \param callback             The object containing the callback to call when input is triggered
/// \param negativeMouseButton  The button which will send -1.0 to the callback when pressed
/// \param positiveMouseButton  The button which will send +1.0 to the callback when pressed
MouseButtonBidirectionalRangeInput::MouseButtonBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                       sf::Mouse::Button negativeMouseButton,
                                                                       sf::Mouse::Button positiveMouseButton)
    : RangeInput(inputManager, callback)
    , m_negativeMouseButton(negativeMouseButton)
    , m_positiveMouseButton(positiveMouseButton)
{
}

bool MouseButtonBidirectionalRangeInput::detectedEvent() const
{
    return m_inputManager.isMouseButtonAscending(m_negativeMouseButton) || m_inputManager.isMouseButtonAscending(m_positiveMouseButton) ||
           m_inputManager.isMouseButtonDescending(m_negativeMouseButton) || m_inputManager.isMouseButtonDescending(m_positiveMouseButton);
}

void MouseButtonBidirectionalRangeInput::callFunction()
{
    bool isNegativeButtonHeld = m_inputManager.isMouseButtonHeld(m_negativeMouseButton);
    bool isPositiveButtonHeld = m_inputManager.isMouseButtonHeld(m_positiveMouseButton);

    // If only negative key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isNegativeButtonHeld == true && isPositiveButtonHeld == false)
    {
        (*m_callback)(-1.0f);
        return;
    }

    // If only positive key is held, and the key is not ascending
    // The check for the ascending key is to account for the possibility of a key ascending and descending in the same tick
    if (isPositiveButtonHeld == true && isNegativeButtonHeld == false)
    {
        (*m_callback)(1.0f);
        return;
    }

    (*m_callback)(0.0f);
}

// MouseButtonUnidirectionalRangeInput

/// Constructor
/// \param inputManager A const reference to the InputManager
/// \param callback     The object containing the callback to call when input is triggered
/// \param mouseButton  The button which will send +1.0 to the callback when pressed
MouseButtonUnidirectionalRangeInput::MouseButtonUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                                         sf::Mouse::Button mouseButton)
    : RangeInput(inputManager, callback)
    , m_mouseButton(mouseButton)
{
}

bool MouseButtonUnidirectionalRangeInput::detectedEvent() const
{
    return m_inputManager.isMouseButtonAscending(m_mouseButton) || m_inputManager.isMouseButtonDescending(m_mouseButton);
}

void MouseButtonUnidirectionalRangeInput::callFunction()
{
    if (m_inputManager.isMouseButtonHeld(m_mouseButton))
    {
        (*m_callback)(1.0f);
    }
    else
    {
        (*m_callback)(0.0f);
    }
}
