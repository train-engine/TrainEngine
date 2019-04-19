#ifndef RANGEINPUT_H
#define RANGEINPUT_H

#include <SFML/Graphics.hpp>
#include "Callbacks.h"
#include "InputManager.h"
/// Class representing an input able to make a callback to a function with a double as a parameter when triggered.
class RangeInput
{
    public:
    RangeInput(const InputManager& inputManager, Callback<double>* callback);
    virtual ~RangeInput();
    virtual bool DetectedEvent() const = 0;
    virtual void CallFunction() = 0;

protected:
    const InputManager& m_inputManager;
    Callback<double>* m_callback;
};

/// Class representing a joystick axis able to make a callback to a function with a double as a parameter when moved.
class JoystickAxisRangeInput final : public RangeInput
{
public:
    JoystickAxisRangeInput(const InputManager& inputManager, Callback<double>* callback, unsigned int joystick, sf::Joystick::Axis axis);
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    mutable float m_lastAxisPosition;
};

/// Class representing a vertical mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class VerticalMouseWheelRangeInput final : public RangeInput
{
public:
    VerticalMouseWheelRangeInput(const InputManager & inputManager, Callback<double>* callback);
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastVertScroll;
};

/// Class representing a horizontal mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class HorizontalMouseWheelRangeInput final : public RangeInput
{
public:
    HorizontalMouseWheelRangeInput(const InputManager& inputManager, Callback<double>* callback);
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastHorizScroll;
};

/// Class representing horizontal mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class VerticalMouseMovementRangeInput final : public RangeInput
{
public:
    VerticalMouseMovementRangeInput(const InputManager& inputManager, Callback<double>* callback);
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastVertMouseMovement;
};

/// Class representing vertical mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class HorizontalMouseMovementRangeInput final : public RangeInput
{
public:
    HorizontalMouseMovementRangeInput(const InputManager& inputManager, Callback<double>* callback);
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastHorizMouseMovement;
};

/// Class representing keyboard keys able to make a callback to a function with a double as a parameter when triggered.
class KeyboardRangeInput final : public RangeInput
{
public:
    KeyboardRangeInput(const InputManager& inputManager, Callback<double>* callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    sf::Keyboard::Key m_negativeKey;
    sf::Keyboard::Key m_positiveKey;
};

#endif //RANGEINPUT_H