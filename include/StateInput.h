#ifndef STATEINPUT_H
#define STATEINPUT_H

#include <SFML/Graphics.hpp>
#include "Callbacks.h"
#include "InputManager.h"

/// Class representing an input able to make a callback to a function with a bool as a parameter when triggered.
class StateInput
{
public:
    StateInput(const InputManager& inputManager, Callback<bool>* callback);
    virtual ~StateInput();
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
    KeyEventStateInput(const InputManager& inputManager, Callback<bool>* callback, sf::Keyboard::Key key);
    virtual bool DetectedEvent() const override;
    virtual void CallAction() override;
    
private:
    sf::Keyboard::Key m_key;
};

/// Class representing a mouse button input able to make a callback to a function with a bool as a parameter when triggered.
class MouseButtonEventStateInput final : public StateInput
{
public:
    MouseButtonEventStateInput(const InputManager& inputManager, Callback<bool>* callback, sf::Mouse::Button button);
    virtual bool DetectedEvent() const override;
    virtual void CallAction() override;

private:
    sf::Mouse::Button m_button;
};

/// Class representing a joystick button input able to make a callback to a function with a bool as a parameter when triggered.
class JoystickButtonEventStateInput final : public StateInput
{
public:
    JoystickButtonEventStateInput(const InputManager& inputManager, Callback<bool>* callback, unsigned int joystick, unsigned int button);
    virtual bool DetectedEvent() const override;
    virtual void CallAction() override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing a joystick axis input able to make a callback to a function with a bool as a parameter when the axis value exceeds the treshold.
class JoystickAxisStateInput final : public StateInput
{
public:
    JoystickAxisStateInput(const InputManager& inputManager, Callback<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis, float treshold);
    virtual bool DetectedEvent() const override;
    virtual void CallAction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    float m_treshold;
    mutable float m_lastAxisValue; // Remember last axis value to remove useless calls to the callback
};

#endif // STATEINPUT_H