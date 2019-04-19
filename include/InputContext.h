#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <SFML/Graphics.hpp>
#include "Callbacks.h"
#include "InputManager.h"
#include "ActionInput.h"
#include "StateInput.h"
#include "RangeInput.h"

enum class EventType
{
    Ascending,
    Descending,
    Any
};

class InputContext
{
public:
    InputContext(const InputManager& inputManager) : m_inputManager(inputManager) {}
    ~InputContext();

    void Update();

    // InputAction
    template<typename Callable>
    void BindActionToKey(Callable callback, sf::Keyboard::Key key, EventType eventType);
    template<typename Object, typename Callable>
    void BindActionToKey(Object* object, Callable callback, sf::Keyboard::Key key, EventType eventType);
    template<typename Callable>
    void BindActionToMouseButton(Callable callback, sf::Mouse::Button mouseButton, EventType eventType);
    template<typename Object, typename Callable>
    void BindActionToMouseButton(Object* object, Callable callback, sf::Mouse::Button mouseButton, EventType eventType);
    template<typename Callable>
    void BindActionToJoystickButton(Callable callback, unsigned int joystick, unsigned int button, EventType eventType);
    template<typename Object, typename Callable>
    void BindActionToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button, EventType eventType);
    template<typename Callable>
    void BindActionToMouseMoved(Callable callback);
    template<typename Object, typename Callable>
    void BindActionToMouseMoved(Object* object, Callable callback);
    template<typename Callable>
    void BindActionToMouseWheelScrolled(Callable callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    template<typename Object, typename Callable>
    void BindActionToMouseWheelScrolled(Object* object, Callable callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    
    // InputState
    template<typename Callable>
    void BindStateToKey(Callable callback, sf::Keyboard::Key key);
    template<typename Object, typename Callable>
    void BindStateToKey(Object* object, Callable callback, sf::Keyboard::Key key);
    template<typename Callable>
    void BindStateToMouseButton(Callable callback, sf::Mouse::Button button);
    template<typename Object, typename Callable>
    void BindStateToMouseButton(Object* object, Callable callback, sf::Mouse::Button button);
    template<typename Callable>
    void BindStateToJoyStickButton(Callable callback, unsigned int joystick, unsigned int button);
    template<typename Object, typename Callable>
    void BindStateToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button);
    template<typename Callable>
    void BindStateToJoystickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis, float treshold);
    template<typename Object, typename Callable>
    void BindStateToJoystickAxis(Object* object, Callable callback,  unsigned int joystick, sf::Joystick::Axis axis, float treshold);
    
    // InputRange
    template<typename Callable>
    void BindRangeToJoystickAxis(Callable, unsigned int joystick, sf::Joystick::Axis axis);
    template<typename Object, typename Callable>
    void BindRangeToJoystickAxis(Object* object, Callable callback, unsigned int joystick, sf::Joystick::Axis axis);
    template<typename Callable>
    void BindRangeToMouseScroll(Callable callback, sf::Mouse::Wheel wheelAxis);
    template<typename Object, typename Callable>
    void BindRangeToMouseScroll(Object* object, Callable callback, sf::Mouse::Wheel wheelAxis);
    template<typename Callable>
    void BindRangeToHorizontalMouseMovement(Callable callback);
    template<typename Object, typename Callable>
    void BindRangeToHorizontalMouseMovement(Object* object, Callable callback);
    template<typename Callable>
    void BindRangeToVerticalMouseMovement(Callable callback);
    template<typename Object, typename Callable>
    void BindRangeToVerticalMouseMovement(Object* object, Callable callback);
    template<typename Callable>
    void BindRangeToKeyboard(Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    template<typename Object, typename Callable>
    void BindRangeToKeyboard(Object* object, Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);

private:
    // Functions
    void BindActionToKeyHelper(Callback<>* callback, sf::Keyboard::Key key, EventType eventType);
    void BindActionToMouseButtonHelper(Callback<>* callback, sf::Mouse::Button mouseButton, EventType eventType);
    void BindActionToJoystickButtonHelper(Callback<>* callback, unsigned int joystick, unsigned int button, EventType eventType);
    void BindActionToMouseMovedHelper(Callback<>* callback);
    void BindActionToMouseWheelScrolledHelper(Callback<>* callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    void BindRangeToMouseScrollHelper(Callback<double>* callback, sf::Mouse::Wheel wheelAxis);

    // Attributes
    const InputManager& m_inputManager;
    std::vector<ActionInput*> m_actionInputs;
    std::vector<StateInput*> m_stateInputs;
    std::vector<RangeInput*> m_rangeInputs;
};

#include "InputContext.inl"

#endif // INPUTCONTEXT_H