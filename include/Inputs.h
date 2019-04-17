#ifndef INPUTS_H
#define INPUTS_H

#include <SFML/Graphics.hpp>
#include "Callbacks.h"
#include "InputManager.h"

class ActionInput
{
public:
    ActionInput(const InputManager& inputManager, Callback<>* callback)
        : m_inputManager(inputManager)
        , m_callback(callback)
    {}
    virtual ~ActionInput() {delete m_callback;}

    virtual bool DetectedEvent() const = 0;
    virtual void CallAction() {(*m_callback)();}

protected:
    const InputManager& m_inputManager;

private:
    Callback<>* m_callback;
};

class KeyDescendingAction final : public ActionInput
{
public:
    KeyDescendingAction(const InputManager& inputManager, Callback<>* callback, sf::Keyboard::Key key)
        : ActionInput(inputManager, callback)
        , m_key(key)
    {
    }
     
    virtual bool DetectedEvent() const override {return m_inputManager.IsKeyDescending(m_key);}

private:
    sf::Keyboard::Key m_key;
};

class KeyAscendingAction final : public ActionInput
{
public:
    KeyAscendingAction(const InputManager& inputManager, Callback<>* callback, sf::Keyboard::Key key)
        : ActionInput(inputManager, callback)
        , m_key(key)
    {
    } 
    virtual bool DetectedEvent() const override {return m_inputManager.IsKeyAscending(m_key);}

private:
    sf::Keyboard::Key m_key;
};

class MouseButtonDescendingAction final : public ActionInput
{
public:
    MouseButtonDescendingAction(const InputManager& inputManager, Callback<>* callback, sf::Mouse::Button button)
        : ActionInput(inputManager, callback)
        , m_button(button)
    {
    }
    
    virtual bool DetectedEvent() const override {return m_inputManager.IsMouseButtonDescending(m_button);}

private:
    sf::Mouse::Button m_button;
};

class MouseButtonAscendingAction final : public ActionInput
{
public:
    MouseButtonAscendingAction(const InputManager& inputManager, Callback<>* callback, sf::Mouse::Button button)
        : ActionInput(inputManager, callback)
        , m_button(button)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.IsMouseButtonAscending(m_button);}

private:
    sf::Mouse::Button m_button;
};

class JoystickButtonDescendingAction final : public ActionInput
{
public:
    JoystickButtonDescendingAction(const InputManager& inputManager, Callback<>* callback, unsigned int joystick, unsigned int button)
        : ActionInput(inputManager, callback)
        , m_joystick(joystick)
        , m_button(button)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.IsJoystickButtonDescending(m_joystick, m_button);}

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

class JoystickButtonAscendingAction final : public ActionInput
{
public:
    JoystickButtonAscendingAction(const InputManager& inputManager, Callback<>* callback, unsigned int joystick, unsigned int button)
        : ActionInput(inputManager, callback)
        , m_joystick(joystick)
        , m_button(button)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.IsJoystickButtonAscending(m_joystick, m_button);}

private:
    unsigned int m_joystick;
    unsigned int m_button;
};

class MouseMovedAction final : public ActionInput
{
public:
    MouseMovedAction(const InputManager& inputManager, Callback<>* callback)
        : ActionInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.DetectedMouseMovedEvent();}
};

class MouseWheelUpAction final : public ActionInput
{
public:
    MouseWheelUpAction(const InputManager& inputManager, Callback<>* callback)
        : ActionInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override
    {
        return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetVerticalMouseWheelDelta() > 0;
    }
};

class MouseWheelDownAction final : public ActionInput
{
public:
    MouseWheelDownAction(const InputManager& inputManager, Callback<>* callback)
        : ActionInput(inputManager, callback)
    {
    }
     
    virtual bool DetectedEvent() const override
    {
        return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetVerticalMouseWheelDelta() < 0;
    }
};

class MouseWheelLeftAction final : public ActionInput
{
public:
    MouseWheelLeftAction(const InputManager& inputManager, Callback<>* callback)
        : ActionInput(inputManager, callback)
    {
    }
     
    virtual bool DetectedEvent() const override
    {
        return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetHorizontalMouseWheelDelta() < 0;
    }
};

class MouseWheelRightAction final : public ActionInput
{
public:
    MouseWheelRightAction(const InputManager& inputManager, Callback<>* callback)
        : ActionInput(inputManager, callback)
    {
    }
    
    virtual bool DetectedEvent() const override
    {
        return m_inputManager.DetectedMouseWheelScrolledEvent() && m_inputManager.GetHorizontalMouseWheelDelta() > 0;
    }
};

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

class KeyEventState final : public StateInput
{
public:
    KeyEventState(const InputManager& inputManager, Callback<bool>* callback, sf::Keyboard::Key key)
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

class MouseButtonEventState final : public StateInput
{
public:
    MouseButtonEventState(const InputManager& inputManager, Callback<bool>* callback, sf::Mouse::Button button)
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

class JoystickButtonEventState final : public StateInput
{
public:
    JoystickButtonEventState(const InputManager& inputManager, Callback<bool>* callback, unsigned int joystick, unsigned int button)
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

class JoystickAxisRange final : public RangeInput
{
public:
    JoystickAxisRange(const InputManager& inputManager, Callback<double>* callback, unsigned int joystick, sf::Joystick::Axis axis)
        : RangeInput(inputManager, callback)
        , m_joystick(joystick)
        , m_axis(axis)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.DetectedJoystickMovedEvent();}
    virtual void CallAction() override {(*m_callback)(m_inputManager.GetJoystickAxisPosition(m_joystick, m_axis));}

private:
    unsigned int m_joystick;
    sf::Joystick::Axis m_axis;
};

class VerticalMouseWheelRange final : public RangeInput
{
public:
    VerticalMouseWheelRange(const InputManager & inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.DetectedMouseWheelScrolledEvent();}
    virtual void CallAction() override {(*m_callback)(m_inputManager.GetVerticalMouseWheelDelta());}
};

class HorizontalMouseWheelRange final : public RangeInput
{
public:
    HorizontalMouseWheelRange(const InputManager& inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.DetectedMouseWheelScrolledEvent();}
    virtual void CallAction() override {(*m_callback)(m_inputManager.GetHorizontalMouseWheelDelta());}
};

class VerticalMouseMovementRange final : public RangeInput
{
public:
    VerticalMouseMovementRange(const InputManager& inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.DetectedMouseMovedEvent();}
    virtual void CallAction() override {(*m_callback)(m_inputManager.GetMousePositionDelta().y);}
};

class HorizontalMouseMovementRange final : public RangeInput
{
public:
    HorizontalMouseMovementRange(const InputManager& inputManager, Callback<double>* callback)
        : RangeInput(inputManager, callback)
    {
    }

    virtual bool DetectedEvent() const override {return m_inputManager.DetectedMouseMovedEvent();}
    virtual void CallAction() override {(*m_callback)(m_inputManager.GetMousePositionDelta().x);}
};

class KeyboardRange final : public RangeInput
{
public:
    KeyboardRange(const InputManager& inputManager, Callback<double>* callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey)
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
            (*m_callback)(-1.0f);
            return;
        }

        // If only positive key is held
        if (isPositiveKeyHeld == true && isNegativeKeyHeld == false)
        {
            (*m_callback)(1.0f);
            return;
        }

        (*m_callback)(0.0f);
    }

private:
    sf::Keyboard::Key m_negativeKey;
    sf::Keyboard::Key m_positiveKey;
};

#endif // INPUTS_H