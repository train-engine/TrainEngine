#ifndef STATEINPUT_H
#define STATEINPUT_H

#include <SFML/Window.hpp>
#include "Core/Input/InputManager.h"
#include "Misc/Callables.h"

/// Class representing an input able to make a callback to a function with a bool as a parameter when triggered.
class StateInput
{
public:
    StateInput(const InputManager& inputManager, Callable<bool>* callback);
    StateInput(const StateInput&) = delete;
    StateInput(StateInput&&) = delete;
    virtual ~StateInput();
    StateInput& operator=(const StateInput&) = delete;
    StateInput& operator=(StateInput&&) = delete;
    virtual bool detectedEvent() const = 0;
    virtual void callAction() = 0;

protected:
    const InputManager& m_inputManager;
    Callable<bool>* m_callback;
};

/// Class representing a keyboard input able to make a callback to a function with a bool as a parameter when triggered.
class KeyEventStateInput final : public StateInput
{
public:
    KeyEventStateInput(const InputManager& inputManager, Callable<bool>* callback, sf::Keyboard::Key key);
    virtual bool detectedEvent() const override;
    virtual void callAction() override;

private:
    sf::Keyboard::Key m_key;
};

/// Class representing a mouse button input able to make a callback to a function with a bool as a parameter when triggered.
class MouseButtonEventStateInput final : public StateInput
{
public:
    MouseButtonEventStateInput(const InputManager& inputManager, Callable<bool>* callback, sf::Mouse::Button button);
    virtual bool detectedEvent() const override;
    virtual void callAction() override;

private:
    sf::Mouse::Button m_button;
};

/// Class representing a joystick button input able to make a callback to a function with a bool as a parameter when triggered.
class JoystickButtonEventStateInput final : public StateInput
{
public:
    JoystickButtonEventStateInput(const InputManager& inputManager, Callable<bool>* callback, unsigned int joystick, unsigned int button);
    virtual bool detectedEvent() const override;
    virtual void callAction() override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing a joystick axis input able to make a callback to a function with a bool as
/// a parameter when the axis value exceeds the threshold.
class JoystickAxisAboveThresholdStateInput final : public StateInput
{
public:
    JoystickAxisAboveThresholdStateInput(const InputManager& inputManager, Callable<bool>* callback, unsigned int joystick,
                                         sf::Joystick::Axis axis, float threshold);
    virtual bool detectedEvent() const override;
    virtual void callAction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    float m_threshold;
    mutable float m_previousAxisValue; // Remember previous axis value to remove useless calls to the callback
};

/// Class representing a joystick axis input able to make a callback to a function with a bool as
/// a parameter when the axis value exceeds the threshold.
class JoystickAxisBelowThresholdStateInput final : public StateInput
{
public:
    JoystickAxisBelowThresholdStateInput(const InputManager& inputManager, Callable<bool>* callback, unsigned int joystick,
                                         sf::Joystick::Axis axis, float threshold);
    virtual bool detectedEvent() const override;
    virtual void callAction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    float m_threshold;
    mutable float m_previousAxisValue; // Remember previous axis value to remove useless calls to the callback
};

#endif // STATEINPUT_H
