#ifndef ACTIONINPUT_H
#define ACTIONINPUT_H

#include <SFML/Window.hpp>
#include "Misc/Callbacks.h"
#include "InputManager.h"

/// Class representing an input able to make a callback to a function with no parameters when triggered.
class ActionInput
{
public:
    ActionInput(const InputManager& inputManager, Callback<>* callback);
    ActionInput(const ActionInput&) = delete;
    ActionInput(ActionInput&&) = delete;
    virtual ~ActionInput();
    ActionInput& operator=(const ActionInput&) = delete;
    ActionInput& operator=(ActionInput&&) = delete;
    virtual bool DetectedEvent() const = 0;
    virtual void CallFunction();

protected:
    const InputManager& m_inputManager;

private:
    Callback<>* m_callback;
};

/// Class representing key descending event able to make a callback to a function with no parameters when triggered.
class KeyDescendingActionInput final : public ActionInput
{
public:
    KeyDescendingActionInput(const InputManager& inputManager, Callback<>* callback, sf::Keyboard::Key key);
    KeyDescendingActionInput(const KeyDescendingActionInput&) = delete;
    KeyDescendingActionInput(KeyDescendingActionInput&&) = delete;
    KeyDescendingActionInput& operator=(const KeyDescendingActionInput&) = delete;
    KeyDescendingActionInput& operator=(KeyDescendingActionInput&&) = delete;
    virtual bool DetectedEvent() const override;

private:
    sf::Keyboard::Key m_key;
};

/// Class representing key ascending event able to make a callback to a function with no parameters when triggered.
class KeyAscendingActionInput final : public ActionInput
{
public:
    KeyAscendingActionInput(const InputManager& inputManager, Callback<>* callback, sf::Keyboard::Key key);
    KeyAscendingActionInput(const KeyAscendingActionInput&) = delete;
    KeyAscendingActionInput(KeyAscendingActionInput&&) = delete;
    KeyAscendingActionInput& operator=(const KeyAscendingActionInput&) = delete;
    KeyAscendingActionInput& operator=(KeyAscendingActionInput&&) = delete;
    virtual bool DetectedEvent() const override;

private:
    sf::Keyboard::Key m_key;
};

/// Class representing key mouse button descending event able to make a callback to a function with no parameters when triggered.
class MouseButtonDescendingActionInput final : public ActionInput
{
public:
    MouseButtonDescendingActionInput(const InputManager& inputManager, Callback<>* callback, sf::Mouse::Button button);
    MouseButtonDescendingActionInput(const MouseButtonDescendingActionInput&) = delete;
    MouseButtonDescendingActionInput(MouseButtonDescendingActionInput&&) = delete;
    MouseButtonDescendingActionInput& operator=(const MouseButtonDescendingActionInput&) = delete;
    MouseButtonDescendingActionInput& operator=(MouseButtonDescendingActionInput&&) = delete;
    virtual bool DetectedEvent() const override;

private:
    sf::Mouse::Button m_button;
};

/// Class representing key mouse button ascending event able to make a callback to a function with no parameters when triggered.
class MouseButtonAscendingActionInput final : public ActionInput
{
public:
    MouseButtonAscendingActionInput(const InputManager& inputManager, Callback<>* callback, sf::Mouse::Button button);
    MouseButtonAscendingActionInput(const MouseButtonAscendingActionInput&) = delete;
    MouseButtonAscendingActionInput(MouseButtonAscendingActionInput&&) = delete;
    MouseButtonAscendingActionInput& operator=(const MouseButtonAscendingActionInput&) = delete;
    MouseButtonAscendingActionInput& operator=(MouseButtonAscendingActionInput&&) = delete;
    virtual bool DetectedEvent() const override;

private:
    sf::Mouse::Button m_button;
};

/// Class representing joystick button descending event able to make a callback to a function with no parameters when triggered.
class JoystickButtonDescendingActionInput final : public ActionInput
{
public:
    JoystickButtonDescendingActionInput(const InputManager& inputManager, Callback<>* callback, unsigned int joystick, unsigned int button);
    JoystickButtonDescendingActionInput(const JoystickButtonDescendingActionInput&) = delete;
    JoystickButtonDescendingActionInput(JoystickButtonDescendingActionInput&&) = delete;
    JoystickButtonDescendingActionInput& operator=(const JoystickButtonDescendingActionInput&) = delete;
    JoystickButtonDescendingActionInput& operator=(JoystickButtonDescendingActionInput&&) = delete;
    virtual bool DetectedEvent() const override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing joystick button ascending event able to make a callback to a function with no parameters when triggered.
class JoystickButtonAscendingActionInput final : public ActionInput
{
public:
    JoystickButtonAscendingActionInput(const InputManager& inputManager, Callback<>* callback, unsigned int joystick, unsigned int button);
    JoystickButtonAscendingActionInput(const JoystickButtonAscendingActionInput&) = delete;
    JoystickButtonAscendingActionInput(JoystickButtonAscendingActionInput&&) = delete;
    JoystickButtonAscendingActionInput& operator=(const JoystickButtonAscendingActionInput&) = delete;
    JoystickButtonAscendingActionInput& operator=(JoystickButtonAscendingActionInput&&) = delete;
    virtual bool DetectedEvent() const override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing mouse moved event able to make a callback to a function with no parameters when triggered.
class MouseMovedActionInput final : public ActionInput
{
public:
    MouseMovedActionInput(const InputManager& inputManager, Callback<>* callback);
    MouseMovedActionInput(const MouseMovedActionInput&) = delete;
    MouseMovedActionInput(MouseMovedActionInput&&) = delete;
    MouseMovedActionInput& operator=(const MouseMovedActionInput&) = delete;
    MouseMovedActionInput& operator=(MouseMovedActionInput&&) = delete;
    virtual bool DetectedEvent() const override;
};

/// Class representing mouse wheel up event able to make a callback to a function with no parameters when triggered.
class MouseWheelUpActionInput final : public ActionInput
{
public:
    MouseWheelUpActionInput(const InputManager& inputManager, Callback<>* callback);
    MouseWheelUpActionInput(const MouseWheelUpActionInput&) = delete;
    MouseWheelUpActionInput(MouseWheelUpActionInput&&) = delete;
    MouseWheelUpActionInput& operator=(const MouseWheelUpActionInput&) = delete;
    MouseWheelUpActionInput& operator=(MouseWheelUpActionInput&&) = delete;
    virtual bool DetectedEvent() const override;
};

/// Class representing mouse wheel down event able to make a callback to a function with no parameters when triggered.
class MouseWheelDownActionInput final : public ActionInput
{
public:
    MouseWheelDownActionInput(const InputManager& inputManager, Callback<>* callback);
    MouseWheelDownActionInput(const MouseWheelDownActionInput&) = delete;
    MouseWheelDownActionInput(MouseWheelDownActionInput&&) = delete;
    MouseWheelDownActionInput& operator=(const MouseWheelDownActionInput&) = delete;
    MouseWheelDownActionInput& operator=(MouseWheelDownActionInput&&) = delete;
    virtual bool DetectedEvent() const override;
};

/// Class representing mouse wheel left event able to make a callback to a function with no parameters when triggered.
class MouseWheelLeftActionInput final : public ActionInput
{
public:
    MouseWheelLeftActionInput(const InputManager& inputManager, Callback<>* callback);
    MouseWheelLeftActionInput(const MouseWheelLeftActionInput&) = delete;
    MouseWheelLeftActionInput(MouseWheelLeftActionInput&&) = delete;
    MouseWheelLeftActionInput& operator=(const MouseWheelLeftActionInput&) = delete;
    MouseWheelLeftActionInput& operator=(MouseWheelLeftActionInput&&) = delete;
    virtual bool DetectedEvent() const override;
};

/// Class representing mouse wheel right event able to make a callback to a function with no parameters when triggered.
class MouseWheelRightActionInput final : public ActionInput
{
public:
    MouseWheelRightActionInput(const InputManager& inputManager, Callback<>* callback);
    MouseWheelRightActionInput(const MouseWheelRightActionInput&) = delete;
    MouseWheelRightActionInput(MouseWheelRightActionInput&&) = delete;
    MouseWheelRightActionInput& operator=(const MouseWheelRightActionInput&) = delete;
    MouseWheelRightActionInput& operator=(MouseWheelRightActionInput&&) = delete;
    virtual bool DetectedEvent() const override;
};


#endif //ACTIONINPUT_H
