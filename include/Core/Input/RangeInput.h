#ifndef RANGEINPUT_H
#define RANGEINPUT_H

#include <SFML/Window.hpp>
#include "Core/Input/InputManager.h"
#include "Misc/Callables.h"

/// Class representing an input able to make a callback to a function with a float as a parameter when triggered.
class RangeInput
{
public:
    RangeInput(const InputManager& inputManager, Callable<float>* callback);
    RangeInput(const RangeInput&) = delete;
    RangeInput(RangeInput&&) = delete;
    virtual ~RangeInput();
    RangeInput& operator=(const RangeInput&) = delete;
    RangeInput& operator=(RangeInput&&) = delete;

    virtual bool detectedEvent() const = 0;
    virtual void callFunction() = 0;

protected:
    const InputManager& m_inputManager;
    Callable<float>* m_callback;
};

/// Class representing a joystick axis able to make a callback to a function with a float as a parameter when moved.
class JoystickAxisBidirectionalRangeInput final : public RangeInput
{
public:
    JoystickAxisBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, unsigned int joystick,
                                        sf::Joystick::Axis axis);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    mutable float m_previousAxisPosition;
};

/// Class representing a joystick axis able to make a callback to a function with a float as a parameter when moved.
class JoystickAxisUnidirectionalRangeInput final : public RangeInput
{
public:
    JoystickAxisUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, unsigned int joystick,
                                         sf::Joystick::Axis axis, bool isRestrictedToPositives);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    bool m_isRestrictedToPositives;
    mutable float m_previousAxisPosition;
};

/// Class representing a vertical mouse wheel able to make a callback to a function with a float as a parameter when scrolled.
class VerticalMouseWheelBidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseWheelBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    mutable float m_previousVertScroll;
};

/// Class representing a vertical mouse wheel able to make a callback to a function with a float as a parameter when scrolled.
class VerticalMouseWheelUnidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, bool isRestrictedToPositives);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_previousVertScroll;
};

/// Class representing a horizontal mouse wheel able to make a callback to a function with a float as a parameter when scrolled.
class HorizontalMouseWheelBidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseWheelBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    mutable float m_previousHorizScroll;
};

/// Class representing a horizontal mouse wheel able to make a callback to a function with a float as a parameter when scrolled.
class HorizontalMouseWheelUnidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, bool isRestrictedToPositives);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_previousHorizScroll;
};

/// Class representing horizontal mouse movements able to make a callback to a function with a float as a parameter when the mouse is moved.
class VerticalMouseMovementBidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseMovementBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    mutable float m_previousVertMouseMovement;
};

/// Class representing horizontal mouse movements able to make a callback to a function with a float as a parameter when the mouse is moved.
class VerticalMouseMovementUnidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                  bool isRestrictedToPositives);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_previousVertMouseMovement;
};

/// Class representing vertical mouse movements able to make a callback to a function with a float as a parameter when the mouse is moved.
class HorizontalMouseMovementBidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseMovementBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    mutable float m_previousHorizMouseMovement;
};

/// Class representing vertical mouse movements able to make a callback to a function with a float as a parameter when the mouse is moved.
class HorizontalMouseMovementUnidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback,
                                                    bool isRestrictedToPositives);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_previousHorizMouseMovement;
};

/// Class representing keyboard keys able to make a callback to a function with a float as a parameter when triggered.
class KeyboardBidirectionalRangeInput final : public RangeInput
{
public:
    KeyboardBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, sf::Keyboard::Key negativeKey,
                                    sf::Keyboard::Key positiveKey);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    sf::Keyboard::Key m_negativeKey;
    sf::Keyboard::Key m_positiveKey;
};

/// Class representing keyboard keys able to make a callback to a function with a float as a parameter when triggered.
class KeyboardUnidirectionalRangeInput final : public RangeInput
{
public:
    KeyboardUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, sf::Keyboard::Key key);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    sf::Keyboard::Key m_key;
};

/// Class representing joystick buttons able to make a callback to a function with a float as a parameter when triggered.
class JoystickButtonBidirectionalRangeInput final : public RangeInput
{
public:
    JoystickButtonBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, unsigned int joystick,
                                          unsigned int negativeJoystickButton, unsigned int positiveJoystickButton);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    unsigned int m_joystick;
    unsigned int m_negativeJoystickButton;
    unsigned int m_positiveJoystickButton;
};

/// Class representing a joystick button able to make a callback to a function with a float as a parameter when triggered.
class JoystickButtonUnidirectionalRangeInput final : public RangeInput
{
public:
    JoystickButtonUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, unsigned int joystick,
                                           unsigned int button);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing joystick buttons able to make a callback to a function with a float as a parameter when triggered.
class MouseButtonBidirectionalRangeInput final : public RangeInput
{
public:
    MouseButtonBidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, sf::Mouse::Button negativeMouseButton,
                                       sf::Mouse::Button positiveMouseButton);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    sf::Mouse::Button m_negativeMouseButton;
    sf::Mouse::Button m_positiveMouseButton;
};

/// Class representing a joystick button able to make a callback to a function with a float as a parameter when triggered.
class MouseButtonUnidirectionalRangeInput final : public RangeInput
{
public:
    MouseButtonUnidirectionalRangeInput(const InputManager& inputManager, Callable<float>* callback, sf::Mouse::Button mouseButton);
    virtual bool detectedEvent() const override;
    virtual void callFunction() override;

private:
    sf::Mouse::Button m_mouseButton;
};

#endif // RANGEINPUT_H
