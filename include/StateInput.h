#ifndef STATEINPUT_H
#define STATEINPUT_H

#include <SFML/Window.hpp>
#include "Misc/Callbacks.h"
#include "InputManager.h"

/// Class representing an input able to make a callback to a function with a bool as a parameter when triggered.
class StateInput
{
public:
    StateInput(const InputManager& inputManager, Callback<bool>* callback);
    StateInput(const StateInput&) = delete;
    StateInput(StateInput&&) = delete;
    virtual ~StateInput();
    StateInput& operator=(const StateInput&) = delete;
    StateInput& operator=(StateInput&&) = delete;
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
    KeyEventStateInput(const KeyEventStateInput&) = delete;
    KeyEventStateInput(KeyEventStateInput&&) = delete;
    KeyEventStateInput& operator=(const KeyEventStateInput&) = delete;
    KeyEventStateInput& operator=(KeyEventStateInput&&) = delete;
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
    MouseButtonEventStateInput(const MouseButtonEventStateInput&) = delete;
    MouseButtonEventStateInput(MouseButtonEventStateInput&&) = delete;
    MouseButtonEventStateInput& operator=(const MouseButtonEventStateInput&) = delete;
    MouseButtonEventStateInput& operator=(MouseButtonEventStateInput&&) = delete;
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
    JoystickButtonEventStateInput(const JoystickButtonEventStateInput&) = delete;
    JoystickButtonEventStateInput(JoystickButtonEventStateInput&&) = delete;
    JoystickButtonEventStateInput& operator=(const JoystickButtonEventStateInput&) = delete;
    JoystickButtonEventStateInput& operator=(JoystickButtonEventStateInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallAction() override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing a joystick axis input able to make a callback to a function with a bool as a parameter when the axis value exceeds the threshold.
class JoystickAxisAboveThresholdStateInput final : public StateInput
{
public:
    JoystickAxisAboveThresholdStateInput(const InputManager& inputManager, Callback<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold);
    JoystickAxisAboveThresholdStateInput(const JoystickAxisAboveThresholdStateInput&) = delete;
    JoystickAxisAboveThresholdStateInput(JoystickAxisAboveThresholdStateInput&&) = delete;
    JoystickAxisAboveThresholdStateInput& operator=(const JoystickAxisAboveThresholdStateInput&) = delete;
    JoystickAxisAboveThresholdStateInput& operator=(JoystickAxisAboveThresholdStateInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallAction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    float m_threshold;
    mutable float m_lastAxisValue; // Remember last axis value to remove useless calls to the callback
};

/// Class representing a joystick axis input able to make a callback to a function with a bool as a parameter when the axis value exceeds the threshold.
class JoystickAxisBelowThresholdStateInput final : public StateInput
{
public:
    JoystickAxisBelowThresholdStateInput(const InputManager& inputManager, Callback<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold);
    JoystickAxisBelowThresholdStateInput(const JoystickAxisBelowThresholdStateInput&) = delete;
    JoystickAxisBelowThresholdStateInput(JoystickAxisBelowThresholdStateInput&&) = delete;
    JoystickAxisBelowThresholdStateInput& operator=(const JoystickAxisBelowThresholdStateInput&) = delete;
    JoystickAxisBelowThresholdStateInput& operator=(JoystickAxisBelowThresholdStateInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallAction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    float m_threshold;
    mutable float m_lastAxisValue; // Remember last axis value to remove useless calls to the callback
};

#endif // STATEINPUT_H
