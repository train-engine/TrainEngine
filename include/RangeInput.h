#ifndef RANGEINPUT_H
#define RANGEINPUT_H

#include <SFML/Graphics.hpp>
#include "Callbacks.h"
#include "InputManager.h"

/// Class representing an input able to make a callback to a function with a double as a parameter when triggered.
class RangeInput
{
    public:
    RangeInput(const InputManager& inputManager, Callback<double>* callback)
        : m_inputManager(inputManager)
        , m_callback(callback)
    {
    }

    virtual ~RangeInput() {delete m_callback;}

    virtual bool DetectedEvent() const = 0;
    virtual void CallAction() = 0;

protected:
    const InputManager& m_inputManager;
    Callback<double>* m_callback;
};

/// Class representing a joystick axis able to make a callback to a function with a double as a parameter when moved.
class JoystickAxisRangeInput final : public RangeInput
{
public:
    JoystickAxisRangeInput(const InputManager& inputManager, Callback<double>* callback, unsigned int joystick, sf::Joystick::Axis axis)
        : RangeInput(inputManager, callback)
        , m_joystick(joystick)
        , m_axis(axis)
        , m_lastAxisPosition(0)
    {
    }

    virtual bool DetectedEvent() const override
    {
        float currentAxisPosition = m_inputManager.DetectedJoystickMovedEvent();
        bool detectedEvent = currentAxisPosition != m_lastAxisPosition;
        m_lastAxisPosition = currentAxisPosition;
        return detectedEvent;
    }
    virtual void CallAction() override {(*m_callback)(m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis));}

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    mutable float m_lastAxisPosition;
};

/// Class representing a vertical mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class VerticalMouseWheelRangeInput final : public RangeInput
{
public:
    VerticalMouseWheelRangeInput(const InputManager & inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override
    {
        float currentVertScroll = m_inputManager.GetVerticalMouseWheelDelta();
        bool detectedEvent = currentVertScroll != m_lastVertScroll;
        m_lastVertScroll = currentVertScroll;
        return detectedEvent;
    }

    virtual void CallAction() override {(*m_callback)(m_inputManager.GetVerticalMouseWheelDelta());}

private:
    mutable float m_lastVertScroll;
};

/// Class representing a horizontal mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class HorizontalMouseWheelRangeInput final : public RangeInput
{
public:
    HorizontalMouseWheelRangeInput(const InputManager& inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
        , m_lastHorizScroll(0.0)
    {
    }

    virtual bool DetectedEvent() const override
    {
        float currentHorizScroll = m_inputManager.GetHorizontalMouseWheelDelta();
        bool detectedEvent = currentHorizScroll != m_lastHorizScroll;
        m_lastHorizScroll = currentHorizScroll;
        return detectedEvent;
    }
    
    virtual void CallAction() override {(*m_callback)(m_inputManager.GetHorizontalMouseWheelDelta());}

private:
    mutable float m_lastHorizScroll;
};

/// Class representing horizontal mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class VerticalMouseMovementRangeInput final : public RangeInput
{
public:
    VerticalMouseMovementRangeInput(const InputManager& inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override
    {
        float currentVertMouseMovement = m_inputManager.GetMousePositionDelta().y;
        bool detectedEvent = currentVertMouseMovement != m_lastVertMouseMovement;
        m_lastVertMouseMovement = currentVertMouseMovement;
        return detectedEvent;
    }

    virtual void CallAction() override {(*m_callback)(m_inputManager.GetMousePositionDelta().y);}

private:
    mutable float m_lastVertMouseMovement;
};

#include <iostream>

/// Class representing vertical mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class HorizontalMouseMovementRangeInput final : public RangeInput
{
public:
    HorizontalMouseMovementRangeInput(const InputManager& inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
        , m_lastHorizMouseMovement(0.0)
    {
    }

    virtual bool DetectedEvent() const override
    {
        float currentHorizMouseMovement = m_inputManager.GetMousePositionDelta().x;
        bool detectedEvent = currentHorizMouseMovement != m_lastHorizMouseMovement;
        m_lastHorizMouseMovement = currentHorizMouseMovement;
        return detectedEvent;
    }

    virtual void CallAction() override {(*m_callback)(m_inputManager.GetMousePositionDelta().x);}

private:
    mutable float m_lastHorizMouseMovement;
};

/// Class representing keyboard keys able to make a callback to a function with a double as a parameter when triggered.
class KeyboardRangeInput final : public RangeInput
{
public:
    /// Constructor
    /// \param inputManager     A const reference to the InputManager
    /// \param callback         The object containing the callback to call when input is triggered.
    /// \param negativeKey      The key wich will send -100 to the callback when pressed.
    /// \param positiveKey      The key wich will send +100 to the callback when pressed.
    KeyboardRangeInput(const InputManager& inputManager, Callback<double>* callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
        : RangeInput(inputManager, callback)
        , m_negativeKey(negativeKey)
        , m_positiveKey(positiveKey)
    {
    }

    virtual bool DetectedEvent() const override
    {
        return m_inputManager.IsKeyAscending(m_negativeKey) || m_inputManager.IsKeyAscending(m_positiveKey) ||
               m_inputManager.IsKeyDescending(m_negativeKey) || m_inputManager.IsKeyDescending(m_positiveKey);
    }

    virtual void CallAction() override
    {
        bool isNegativeKeyHeld = m_inputManager.IsKeyHeld(m_negativeKey);
        bool isPositiveKeyHeld = m_inputManager.IsKeyHeld(m_positiveKey);

        // If only negative key is held
        if (isNegativeKeyHeld == true && isPositiveKeyHeld == false)
        {
            (*m_callback)(-100.0f);
            return;
        }

        // If only positive key is held
        if (isPositiveKeyHeld == true && isNegativeKeyHeld == false)
        {
            (*m_callback)(100.0f);
            return;
        }

        (*m_callback)(0.0f);
    }

private:
    sf::Keyboard::Key m_negativeKey;
    sf::Keyboard::Key m_positiveKey;
};

#endif //RANGEINPUT_H