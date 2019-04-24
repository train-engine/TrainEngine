#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <SFML/Window.hpp>
#include "Misc/Callbacks.h"
#include "Core/Input/InputManager.h"
#include "Core/Input/ActionInput.h"
#include "Core/Input/StateInput.h"
#include "Core/Input/RangeInput.h"

enum class EventType
{
    Ascending,
    Descending,
    Any
};

enum class JoystickAxisPosition
{
    AboveTreshold,
    BelowTreshold,
    Any
};

enum class RangeRestriction
{
    PositiveOnly,
    NegativeOnly,
    None
};

class InputContext
{
public:
    InputContext(const InputManager& inputManager) : m_inputManager(inputManager) {}
    ~InputContext();

    void Update();

    void Clear();

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
    void BindStateToJoystickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis, float treshold, JoystickAxisPosition axisPosition);
    template<typename Object, typename Callable>
    void BindStateToJoystickAxis(Object* object, Callable callback,  unsigned int joystick, sf::Joystick::Axis axis, float treshold,
                                 JoystickAxisPosition axisPosition);
    
    // InputRange
    template<typename Callable>
    void BindRangeToJoystickAxis(Callable callback, unsigned int joystick, sf::Joystick::Axis axis,
                                 RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Object, typename Callable>
    void BindRangeToJoystickAxis(Object* object, Callable callback, unsigned int joystick, sf::Joystick::Axis axis,
                                 RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Callable>
    void BindRangeToMouseScroll(Callable callback, sf::Mouse::Wheel wheelAxis,
                                RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Object, typename Callable>
    void BindRangeToMouseScroll(Object* object, Callable callback, sf::Mouse::Wheel wheelAxis,
                                RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Callable>
    void BindRangeToHorizontalMouseMovement(Callable callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Object, typename Callable>
    void BindRangeToHorizontalMouseMovement(Object* object, Callable callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Callable>
    void BindRangeToVerticalMouseMovement(Callable callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Object, typename Callable>
    void BindRangeToVerticalMouseMovement(Object* object, Callable callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename Callable>
    void BindRangeToKeyboard(Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    template<typename Object, typename Callable>
    void BindRangeToKeyboard(Object* object, Callable callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    template<typename Callable>
    void BindRangeToKeyboard(Callable callback, sf::Keyboard::Key key);
    template<typename Object, typename Callable>
    void BindRangeToKeyboard(Object* object, Callable callback, sf::Keyboard::Key key);
    template<typename Callable>
    void BindRangeToJoystickButtons(Callable callback, unsigned int joystick, unsigned int negativeButton, unsigned int positiveButton);
    template<typename Object, typename Callable>
    void BindRangeToJoystickButtons(Object* object, Callable callback, unsigned int joystick, unsigned int negativeButton, unsigned int positiveButton);
    template<typename Callable>
    void BindRangeToJoystickButton(Callable callback, unsigned int joystick, unsigned int button);
    template<typename Object, typename Callable>
    void BindRangeToJoystickButton(Object* object, Callable callback, unsigned int joystick, unsigned int button);

private:
    // Functions
    void BindActionToKeyHelper(Callback<>* callback, sf::Keyboard::Key key, EventType eventType);
    void BindActionToMouseButtonHelper(Callback<>* callback, sf::Mouse::Button mouseButton, EventType eventType);
    void BindActionToJoystickButtonHelper(Callback<>* callback, unsigned int joystick, unsigned int button, EventType eventType);
    void BindActionToMouseMovedHelper(Callback<>* callback);
    void BindActionToMouseWheelScrolledHelper(Callback<>* callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    void BindStateToJoystickAxisHelper(Callback<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis, float treshold, JoystickAxisPosition axisPosition);
    void BindRangeToMouseScrollHelper(Callback<double>* callback, sf::Mouse::Wheel wheelAxis);
    void BindRangeToMouseScrollHelper(Callback<double>* callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction);

    // Attributes
    const InputManager& m_inputManager;
    std::vector<ActionInput*> m_actionInputs;
    std::vector<StateInput*> m_stateInputs;
    std::vector<RangeInput*> m_rangeInputs;
};

#include "InputContext.inl"

#endif // INPUTCONTEXT_H
