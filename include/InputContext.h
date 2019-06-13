#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <SFML/Window.hpp>
#include "Core/Input/ActionInput.h"
#include "Core/Input/InputManager.h"
#include "Core/Input/RangeInput.h"
#include "Core/Input/StateInput.h"
#include "Misc/Callables.h"

enum class EventType
{
    Ascending,
    Descending,
    Any
};

enum class JoystickAxisPosition
{
    AboveThreshold,
    BelowThreshold,
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
    // Constructor and destructor
    InputContext(const InputManager& inputManager);
    ~InputContext();

    // Functions
    void Update();
    void Clear();

    // ActionInput

    template<typename F>
    void BindActionToKey(F callback, sf::Keyboard::Key key, EventType eventType);
    template<typename O, typename F>
    void BindActionToKey(O* object, F callback, sf::Keyboard::Key key, EventType eventType);

    template<typename F>
    void BindActionToMouseButton(F callback, sf::Mouse::Button mouseButton, EventType eventType);
    template<typename O, typename F>
    void BindActionToMouseButton(O* object, F callback, sf::Mouse::Button mouseButton, EventType eventType);

    template<typename F>
    void BindActionToMouseMoved(F callback);
    template<typename O, typename F>
    void BindActionToMouseMoved(O* object, F callback);

    template<typename F>
    void BindActionToMouseWheelScrolled(F callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    template<typename O, typename F>
    void BindActionToMouseWheelScrolled(O* object, F callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);

     template<typename F>
    void BindActionToJoystickButton(F callback, unsigned int joystick, unsigned int button, EventType eventType);
    template<typename O, typename F>
    void BindActionToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button, EventType eventType);

    // StateInput

    template<typename F>
    void BindStateToKey(F callback, sf::Keyboard::Key key);
    template<typename O, typename F>
    void BindStateToKey(O* object, F callback, sf::Keyboard::Key key);

    template<typename F>
    void BindStateToMouseButton(F callback, sf::Mouse::Button button);
    template<typename O, typename F>
    void BindStateToMouseButton(O* object, F callback, sf::Mouse::Button button);
    template<typename F>

    void BindStateToJoystickButton(F callback, unsigned int joystick, unsigned int button);
    template<typename O, typename F>
    void BindStateToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button);

    template<typename F>
    void BindStateToJoystickAxis(F callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold,
                                 JoystickAxisPosition axisPosition);
    template<typename O, typename F>
    void BindStateToJoystickAxis(O* object, F callback,  unsigned int joystick, sf::Joystick::Axis axis, float threshold,
                                 JoystickAxisPosition axisPosition);

    // RangeInput

    template<typename F>
    void BindRangeToKeyboard(F callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    template<typename O, typename F>
    void BindRangeToKeyboard(O* object, F callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    template<typename F>
    void BindRangeToKeyboard(F callback, sf::Keyboard::Key key);
    template<typename O, typename F>
    void BindRangeToKeyboard(O* object, F callback, sf::Keyboard::Key key);

    template<typename F>
    void BindRangeToMouseButtons(F callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton);
    template<typename O, typename F>
    void BindRangeToMouseButtons(O* object, F callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton);
    template<typename F>
    void BindRangeToMouseButton(F callback, sf::Mouse::Button button);
    template<typename O, typename F>
    void BindRangeToMouseButton(O* object, F callback, sf::Mouse::Button button);

    template<typename F>
    void BindRangeToHorizontalMouseMovement(F callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void BindRangeToHorizontalMouseMovement(O* object, F callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename F>
    void BindRangeToVerticalMouseMovement(F callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void BindRangeToVerticalMouseMovement(O* object, F callback, RangeRestriction rangeRestriction = RangeRestriction::None);

    template<typename F>
    void BindRangeToMouseWheelScroll(F callback, sf::Mouse::Wheel wheelAxis,
                                RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void BindRangeToMouseWheelScroll(O* object, F callback, sf::Mouse::Wheel wheelAxis,
                                RangeRestriction rangeRestriction = RangeRestriction::None);

    template<typename F>
    void BindRangeToJoystickButtons(F callback, unsigned int joystick, unsigned int negativeButton,
                                    unsigned int positiveButton);
    template<typename O, typename F>
    void BindRangeToJoystickButtons(O* object, F callback, unsigned int joystick, unsigned int negativeButton,
                                    unsigned int positiveButton);
    template<typename F>
    void BindRangeToJoystickButton(F callback, unsigned int joystick, unsigned int button);
    template<typename O, typename F>
    void BindRangeToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button);

    template<typename F>
    void BindRangeToJoystickAxis(F callback, unsigned int joystick, sf::Joystick::Axis axis,
                                 RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void BindRangeToJoystickAxis(O* object, F callback, unsigned int joystick, sf::Joystick::Axis axis,
                                 RangeRestriction rangeRestriction = RangeRestriction::None);

private:
    // Binding helper functions
    void BindActionToKeyHelper(Callable<>* callback, sf::Keyboard::Key key, EventType eventType);
    void BindActionToMouseButtonHelper(Callable<>* callback, sf::Mouse::Button mouseButton, EventType eventType);
    void BindActionToJoystickButtonHelper(Callable<>* callback, unsigned int joystick, unsigned int button, EventType eventType);
    void BindActionToMouseMovedHelper(Callable<>* callback);
    void BindActionToMouseWheelScrolledHelper(Callable<>* callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    void BindStateToJoystickAxisHelper(Callable<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold, JoystickAxisPosition axisPosition);
    void BindRangeToMouseWheelScrollHelper(Callable<float>* callback, sf::Mouse::Wheel wheelAxis);
    void BindRangeToMouseWheelScrollHelper(Callable<float>* callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction);

    const InputManager& m_inputManager;

    std::vector<ActionInput*> m_actionInputs;
    std::vector<StateInput*> m_stateInputs;
    std::vector<RangeInput*> m_rangeInputs;
};

#include "InputContext.inl"

#endif // INPUTCONTEXT_H
