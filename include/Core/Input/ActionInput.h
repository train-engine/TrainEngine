#ifndef ACTIONINPUT_H
#define ACTIONINPUT_H

#include <SFML/Window.hpp>
#include "Core/Input/InputManager.h"
#include "Misc/Callables.h"

/// Class representing an input able to make a callback to a function with no parameters when triggered.
class ActionInput
{
public:
    ActionInput(const InputManager& inputManager, Callable<>* callback);
    ActionInput(const ActionInput&) = delete;
    ActionInput(ActionInput&&) = delete;
    virtual ~ActionInput();
    ActionInput& operator=(const ActionInput&) = delete;
    ActionInput& operator=(ActionInput&&) = delete;
    virtual bool detectedEvent() const = 0;
    virtual void callFunction();

protected:
    const InputManager& m_inputManager;

private:
    Callable<>* m_callback;
};

/// Class representing key descending event able to make a callback to a function with no parameters when triggered.
class KeyDescendingActionInput final : public ActionInput
{
public:
    KeyDescendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Keyboard::Key key);
    virtual bool detectedEvent() const override;

private:
    sf::Keyboard::Key m_key;
};

/// Class representing key ascending event able to make a callback to a function with no parameters when triggered.
class KeyAscendingActionInput final : public ActionInput
{
public:
    KeyAscendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Keyboard::Key key);
    virtual bool detectedEvent() const override;

private:
    sf::Keyboard::Key m_key;
};

/// Class representing key mouse button descending event able to make a callback to a function with no parameters when triggered.
class MouseButtonDescendingActionInput final : public ActionInput
{
public:
    MouseButtonDescendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Mouse::Button button);
    virtual bool detectedEvent() const override;

private:
    sf::Mouse::Button m_button;
};

/// Class representing key mouse button ascending event able to make a callback to a function with no parameters when triggered.
class MouseButtonAscendingActionInput final : public ActionInput
{
public:
    MouseButtonAscendingActionInput(const InputManager& inputManager, Callable<>* callback, sf::Mouse::Button button);
    virtual bool detectedEvent() const override;

private:
    sf::Mouse::Button m_button;
};

/// Class representing joystick button descending event able to make a callback to a function with no parameters when triggered.
class JoystickButtonDescendingActionInput final : public ActionInput
{
public:
    JoystickButtonDescendingActionInput(const InputManager& inputManager, Callable<>* callback, unsigned int joystick, unsigned int button);
    virtual bool detectedEvent() const override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing joystick button ascending event able to make a callback to a function with no parameters when triggered.
class JoystickButtonAscendingActionInput final : public ActionInput
{
public:
    JoystickButtonAscendingActionInput(const InputManager& inputManager, Callable<>* callback, unsigned int joystick, unsigned int button);
    virtual bool detectedEvent() const override;

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

/// Class representing mouse moved event able to make a callback to a function with no parameters when triggered.
class MouseMovedActionInput final : public ActionInput
{
public:
    MouseMovedActionInput(const InputManager& inputManager, Callable<>* callback);
    virtual bool detectedEvent() const override;
};

/// Class representing mouse wheel up event able to make a callback to a function with no parameters when triggered.
class MouseWheelUpActionInput final : public ActionInput
{
public:
    MouseWheelUpActionInput(const InputManager& inputManager, Callable<>* callback);
    virtual bool detectedEvent() const override;
};

/// Class representing mouse wheel down event able to make a callback to a function with no parameters when triggered.
class MouseWheelDownActionInput final : public ActionInput
{
public:
    MouseWheelDownActionInput(const InputManager& inputManager, Callable<>* callback);
    virtual bool detectedEvent() const override;
};

/// Class representing mouse wheel left event able to make a callback to a function with no parameters when triggered.
class MouseWheelLeftActionInput final : public ActionInput
{
public:
    MouseWheelLeftActionInput(const InputManager& inputManager, Callable<>* callback);
    virtual bool detectedEvent() const override;
};

/// Class representing mouse wheel right event able to make a callback to a function with no parameters when triggered.
class MouseWheelRightActionInput final : public ActionInput
{
public:
    MouseWheelRightActionInput(const InputManager& inputManager, Callable<>* callback);
    virtual bool detectedEvent() const override;
};

#endif // ACTIONINPUT_H
