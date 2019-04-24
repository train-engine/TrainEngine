#ifndef RANGEINPUT_H
#define RANGEINPUT_H

#include <SFML/Window.hpp>
#include "Misc/Callbacks.h"
#include "InputManager.h"

/// Class representing an input able to make a callback to a function with a double as a parameter when triggered.
class RangeInput
{
    public:
    RangeInput(const InputManager& inputManager, Callback<double>* callback);
    RangeInput(const RangeInput&) = delete;
    RangeInput(RangeInput&&) = delete;
    virtual ~RangeInput();
    RangeInput& operator=(const RangeInput&) = delete;
    RangeInput& operator=(RangeInput&&) = delete;

    virtual bool DetectedEvent() const = 0;
    virtual void CallFunction() = 0;

protected:
    const InputManager& m_inputManager;
    Callback<double>* m_callback;
};

/// Class representing a joystick axis able to make a callback to a function with a double as a parameter when moved.
class JoystickAxisBidirectionalRangeInput final : public RangeInput
{
public:
    JoystickAxisBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback,
                                        unsigned int joystick, sf::Joystick::Axis axis);
    JoystickAxisBidirectionalRangeInput(const JoystickAxisBidirectionalRangeInput&) = delete;
    JoystickAxisBidirectionalRangeInput(JoystickAxisBidirectionalRangeInput&&) = delete;
    JoystickAxisBidirectionalRangeInput& operator=(const JoystickAxisBidirectionalRangeInput&) = delete;
    JoystickAxisBidirectionalRangeInput& operator=(JoystickAxisBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    mutable float m_lastAxisPosition;
};

/// Class representing a joystick axis able to make a callback to a function with a double as a parameter when moved.
class JoystickAxisUnidirectionalRangeInput final : public RangeInput
{
public:
    JoystickAxisUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, unsigned int joystick,
                                         sf::Joystick::Axis axis, bool isRestrictedToPositives);
    JoystickAxisUnidirectionalRangeInput(const JoystickAxisUnidirectionalRangeInput&) = delete;
    JoystickAxisUnidirectionalRangeInput(JoystickAxisUnidirectionalRangeInput&&) = delete;
    JoystickAxisUnidirectionalRangeInput& operator=(const JoystickAxisUnidirectionalRangeInput&) = delete;
    JoystickAxisUnidirectionalRangeInput& operator=(JoystickAxisUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
    bool m_isRestrictedToPositives;
    mutable float m_lastAxisPosition;
};

/// Class representing a vertical mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class VerticalMouseWheelBidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseWheelBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback);
    VerticalMouseWheelBidirectionalRangeInput(const VerticalMouseWheelBidirectionalRangeInput&) = delete;
    VerticalMouseWheelBidirectionalRangeInput(VerticalMouseWheelBidirectionalRangeInput&&) = delete;
    VerticalMouseWheelBidirectionalRangeInput& operator=(const VerticalMouseWheelBidirectionalRangeInput&) = delete;
    VerticalMouseWheelBidirectionalRangeInput& operator=(VerticalMouseWheelBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastVertScroll;
};

/// Class representing a vertical mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class VerticalMouseWheelUnidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, bool isRestrictedToPositives);
    VerticalMouseWheelUnidirectionalRangeInput(const VerticalMouseWheelUnidirectionalRangeInput&) = delete;
    VerticalMouseWheelUnidirectionalRangeInput(VerticalMouseWheelUnidirectionalRangeInput&&) = delete;
    VerticalMouseWheelUnidirectionalRangeInput& operator=(const VerticalMouseWheelUnidirectionalRangeInput&) = delete;
    VerticalMouseWheelUnidirectionalRangeInput& operator=(VerticalMouseWheelUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_lastVertScroll;
};

/// Class representing a horizontal mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class HorizontalMouseWheelBidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseWheelBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback);
    HorizontalMouseWheelBidirectionalRangeInput(const HorizontalMouseWheelBidirectionalRangeInput&) = delete;
    HorizontalMouseWheelBidirectionalRangeInput(HorizontalMouseWheelBidirectionalRangeInput&&) = delete;
    HorizontalMouseWheelBidirectionalRangeInput& operator=(const HorizontalMouseWheelBidirectionalRangeInput&) = delete;
    HorizontalMouseWheelBidirectionalRangeInput& operator=(HorizontalMouseWheelBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastHorizScroll;
};

/// Class representing a horizontal mouse wheel able to make a callback to a function with a double as a parameter when scrolled.
class HorizontalMouseWheelUnidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseWheelUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, bool isRestrictedToPositives);
    HorizontalMouseWheelUnidirectionalRangeInput(const HorizontalMouseWheelUnidirectionalRangeInput&) = delete;
    HorizontalMouseWheelUnidirectionalRangeInput(HorizontalMouseWheelUnidirectionalRangeInput&&) = delete;
    HorizontalMouseWheelUnidirectionalRangeInput& operator=(const HorizontalMouseWheelUnidirectionalRangeInput&) = delete;
    HorizontalMouseWheelUnidirectionalRangeInput& operator=(HorizontalMouseWheelUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_lastHorizScroll;
};

/// Class representing horizontal mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class VerticalMouseMovementBidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseMovementBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback);
    VerticalMouseMovementBidirectionalRangeInput(const VerticalMouseMovementBidirectionalRangeInput&) = delete;
    VerticalMouseMovementBidirectionalRangeInput(VerticalMouseMovementBidirectionalRangeInput&&) = delete;
    VerticalMouseMovementBidirectionalRangeInput& operator=(const VerticalMouseMovementBidirectionalRangeInput&) = delete;
    VerticalMouseMovementBidirectionalRangeInput& operator=(VerticalMouseMovementBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastVertMouseMovement;
};

/// Class representing horizontal mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class VerticalMouseMovementUnidirectionalRangeInput final : public RangeInput
{
public:
    VerticalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, bool isRestrictedToPositives);
    VerticalMouseMovementUnidirectionalRangeInput(const VerticalMouseMovementUnidirectionalRangeInput&) = delete;
    VerticalMouseMovementUnidirectionalRangeInput(VerticalMouseMovementUnidirectionalRangeInput&&) = delete;
    VerticalMouseMovementUnidirectionalRangeInput& operator=(const VerticalMouseMovementUnidirectionalRangeInput&) = delete;
    VerticalMouseMovementUnidirectionalRangeInput& operator=(VerticalMouseMovementUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_lastVertMouseMovement;
};

/// Class representing vertical mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class HorizontalMouseMovementBidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseMovementBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback);
    HorizontalMouseMovementBidirectionalRangeInput(const HorizontalMouseMovementBidirectionalRangeInput&) = delete;
    HorizontalMouseMovementBidirectionalRangeInput(HorizontalMouseMovementBidirectionalRangeInput&&) = delete;
    HorizontalMouseMovementBidirectionalRangeInput& operator=(const HorizontalMouseMovementBidirectionalRangeInput&) = delete;
    HorizontalMouseMovementBidirectionalRangeInput& operator=(HorizontalMouseMovementBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    mutable float m_lastHorizMouseMovement;
};

/// Class representing vertical mouse movements able to make a callback to a function with a double as a parameter when the mouse is moved.
class HorizontalMouseMovementUnidirectionalRangeInput final : public RangeInput
{
public:
    HorizontalMouseMovementUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, bool isRestrictedToPositives);
    HorizontalMouseMovementUnidirectionalRangeInput(const HorizontalMouseMovementUnidirectionalRangeInput&) = delete;
    HorizontalMouseMovementUnidirectionalRangeInput(HorizontalMouseMovementUnidirectionalRangeInput&&) = delete;
    HorizontalMouseMovementUnidirectionalRangeInput& operator=(const HorizontalMouseMovementUnidirectionalRangeInput&) = delete;
    HorizontalMouseMovementUnidirectionalRangeInput& operator=(HorizontalMouseMovementUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    bool m_isRestrictedToPositives;
    mutable float m_lastHorizMouseMovement;
};

/// Class representing keyboard keys able to make a callback to a function with a double as a parameter when triggered.
class KeyboardBidirectionalRangeInput final : public RangeInput
{
public:
    KeyboardBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    KeyboardBidirectionalRangeInput(const KeyboardBidirectionalRangeInput&) = delete;
    KeyboardBidirectionalRangeInput(KeyboardBidirectionalRangeInput&&) = delete;
    KeyboardBidirectionalRangeInput& operator=(const KeyboardBidirectionalRangeInput&) = delete;
    KeyboardBidirectionalRangeInput& operator=(KeyboardBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    sf::Keyboard::Key m_negativeKey;
    sf::Keyboard::Key m_positiveKey;
};

/// Class representing keyboard keys able to make a callback to a function with a double as a parameter when triggered.
class KeyboardUnidirectionalRangeInput final : public RangeInput
{
public:
    KeyboardUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, sf::Keyboard::Key key);
    KeyboardUnidirectionalRangeInput(const KeyboardUnidirectionalRangeInput&) = delete;
    KeyboardUnidirectionalRangeInput(KeyboardUnidirectionalRangeInput&&) = delete;
    KeyboardUnidirectionalRangeInput& operator=(const KeyboardUnidirectionalRangeInput&) = delete;
    KeyboardUnidirectionalRangeInput& operator=(KeyboardUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    sf::Keyboard::Key m_key;
};

/// Class representing joystick buttons able to make a callback to a function with a double as a parameter when triggered.
class JoystickButtonBidirectionalRangeInput final : public RangeInput
{
public:
    JoystickButtonBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, unsigned int joystick,
                                          unsigned int negativeJoystickButton, unsigned int positiveJoystickButton);
    JoystickButtonBidirectionalRangeInput(const JoystickButtonBidirectionalRangeInput&) = delete;
    JoystickButtonBidirectionalRangeInput(JoystickButtonBidirectionalRangeInput&&) = delete;
    JoystickButtonBidirectionalRangeInput& operator=(const JoystickButtonBidirectionalRangeInput&) = delete;
    JoystickButtonBidirectionalRangeInput& operator=(JoystickButtonBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    unsigned int m_joystick;
    unsigned int m_negativeJoystickButton;
    unsigned int m_positiveJoystickButton;
};

/// Class representing a joystick button able to make a callback to a function with a double as a parameter when triggered.
class JoystickButtonUnidirectionalRangeInput final : public RangeInput
{
public:
    JoystickButtonUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback, unsigned int joystick,
                                           unsigned int button);
    JoystickButtonUnidirectionalRangeInput(const JoystickButtonUnidirectionalRangeInput&) = delete;
    JoystickButtonUnidirectionalRangeInput(JoystickButtonUnidirectionalRangeInput&&) = delete;
    JoystickButtonUnidirectionalRangeInput& operator=(const JoystickButtonUnidirectionalRangeInput&) = delete;
    JoystickButtonUnidirectionalRangeInput& operator=(JoystickButtonUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing joystick buttons able to make a callback to a function with a double as a parameter when triggered.
class MouseButtonBidirectionalRangeInput final : public RangeInput
{
public:
    MouseButtonBidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback,
                                       sf::Mouse::Button negativeMouseButton, sf::Mouse::Button positiveMouseButton);
    MouseButtonBidirectionalRangeInput(const MouseButtonBidirectionalRangeInput&) = delete;
    MouseButtonBidirectionalRangeInput(MouseButtonBidirectionalRangeInput&&) = delete;
    MouseButtonBidirectionalRangeInput& operator=(const MouseButtonBidirectionalRangeInput&) = delete;
    MouseButtonBidirectionalRangeInput& operator=(MouseButtonBidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    sf::Mouse::Button m_negativeMouseButton;
    sf::Mouse::Button m_positiveMouseButton;
};

/// Class representing a joystick button able to make a callback to a function with a double as a parameter when triggered.
class MouseButtonUnidirectionalRangeInput final : public RangeInput
{
public:
    MouseButtonUnidirectionalRangeInput(const InputManager& inputManager, Callback<double>* callback,
                                        sf::Mouse::Button mouseButton);
    MouseButtonUnidirectionalRangeInput(const MouseButtonUnidirectionalRangeInput&) = delete;
    MouseButtonUnidirectionalRangeInput(MouseButtonUnidirectionalRangeInput&&) = delete;
    MouseButtonUnidirectionalRangeInput& operator=(const MouseButtonUnidirectionalRangeInput&) = delete;
    MouseButtonUnidirectionalRangeInput& operator=(MouseButtonUnidirectionalRangeInput&&) = delete;
    virtual bool DetectedEvent() const override;
    virtual void CallFunction() override;

private:
    sf::Mouse::Button m_mouseButton;
};

#endif //RANGEINPUT_H
