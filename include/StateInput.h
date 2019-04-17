#ifndef STATEINPUT_H
#define STATEINPUT_H

#include <SFML/Graphics.hpp>
#include "Callbacks.h"
#include "InputManager.h"

/// Class representing an input able to make a callback to a function with a bool as a parameter when triggered.
class StateInput
{
public:
    StateInput(const InputManager& inputManager, Callback<bool>* callback)
        : m_inputManager(inputManager)
        , m_callback(callback)
    {
    }
    
    virtual ~StateInput() {delete m_callback;}

    virtual bool DetectedEvent() const = 0;
    virtual void CallAction() = 0;

protected:
    const InputManager& m_inputManager;
    Callback<bool>* m_callback;
};

/// Class representing a keyboard input able to make a callback to a function with a bool as a parameter when triggered.
class KeyEventStateInput final : public StateInput
{
public:
    KeyEventStateInput(const InputManager& inputManager, Callback<bool>* callback, sf::Keyboard::Key key)
        : StateInput(inputManager, callback)
        , m_key(key)
    {
    }

    virtual bool DetectedEvent() const override
    {
        return m_inputManager.IsKeyAscending(m_key) || m_inputManager.IsKeyDescending(m_key);
    }

    virtual void CallAction() override
    {
        bool keyDescending = m_inputManager.IsKeyDescending(m_key);
        (*m_callback)(keyDescending);
    }
    
private:
    sf::Keyboard::Key m_key;
};

/// Class representing a mouse button input able to make a callback to a function with a bool as a parameter when triggered.
class MouseButtonEventStateInput final : public StateInput
{
public:
    MouseButtonEventStateInput(const InputManager& inputManager, Callback<bool>* callback, sf::Mouse::Button button)
        : StateInput(inputManager, callback)
        , m_button(button)
    {
    }

    virtual bool DetectedEvent() const override
    {
        return m_inputManager.IsMouseButtonAscending(m_button) || m_inputManager.IsMouseButtonDescending(m_button);
    }

    virtual void CallAction() override
    {
        bool mouseButtonDescending = m_inputManager.IsMouseButtonAscending(m_button);
        (*m_callback)(mouseButtonDescending);
    }
private:
    sf::Mouse::Button m_button;
};

/// Class representing a joystick button input able to make a callback to a function with a bool as a parameter when triggered.
class JoystickButtonEventStateInput final : public StateInput
{
public:
    JoystickButtonEventStateInput(const InputManager& inputManager, Callback<bool>* callback, unsigned int joystick, unsigned int button)
        : StateInput(inputManager, callback)
        , m_joystick(joystick)
        , m_button(button)
    {
    }

    virtual bool DetectedEvent() const override
    {
        return m_inputManager.IsJoystickButtonAscending(m_joystick, m_button) || m_inputManager.IsJoystickButtonDescending(m_joystick, m_button);
    }

    virtual void CallAction() override
    {
        bool joystickButtonDescending = m_inputManager.IsJoystickButtonAscending(m_joystick, m_button);
        (*m_callback)(joystickButtonDescending);
    }
private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing a joystick axis input able to make a callback to a function with a bool as a parameter when the axis value exceeds the treshold.
class JoystickAxisStateInput final : public StateInput
{
public:
    /// Constructor
    /// \param inputManager     A const reference to the InputManager.
    /// \param callback         The object containing the callback to call when input is triggered.
    /// \param joystick         The id of the joystick controller.
    /// \param treshold         The treshold to be exceeded for the callback to be called.
    ///                         If the treshold is negative, the axis value must be lower for the input to be triggered.
    ///                         If the treshold is positive, the axis value must be higher for the input to be triggered.
    JoystickAxisStateInput(const InputManager& inputManager, Callback<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis, float treshold)
        : StateInput(inputManager, callback)
        , m_joystick(joystick)
        , m_axis(axis)
        , m_treshold(treshold)
        , m_lastAxisValue(0)
    {
    }

    virtual bool DetectedEvent() const override
    {
        float currentAxisValue = m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis);
        bool detectedEvent = false;
        if (m_treshold >= 0)
        {
            // Return if the current axis position just passed the treshold
            detectedEvent = (m_lastAxisValue <= m_treshold && currentAxisValue > m_treshold) || 
                            (m_lastAxisValue > m_treshold && currentAxisValue <= m_treshold);
        }
        else
        {
            // Return if the current axis position just passed the treshold
            detectedEvent = (m_lastAxisValue >= m_treshold && currentAxisValue < m_treshold) ||
                            (m_lastAxisValue < m_treshold && currentAxisValue >= m_treshold);
        }

        m_lastAxisValue = currentAxisValue;
        return detectedEvent;
    }

    virtual void CallAction() override
    {
        float currentAxisValue = m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis);
        if (m_treshold >= 0)
        {
            (*m_callback)(currentAxisValue > m_treshold);
        }
        else if (m_treshold < 0)
        {
            (*m_callback)(currentAxisValue < m_treshold);
        }
    }

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    float m_treshold;
    mutable float m_lastAxisValue; // Remember last axis value to remove useless calls to the callback
};

#endif // STATEINPUT_H