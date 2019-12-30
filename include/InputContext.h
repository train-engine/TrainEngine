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
    explicit InputContext(const InputManager& inputManager);
    ~InputContext();

    // Functions
    void update();
    void clear();

    // ActionInput

    template<typename F>
    void bindActionToKey(F callback, sf::Keyboard::Key key, EventType eventType);
    template<typename O, typename F>
    void bindActionToKey(O* object, F callback, sf::Keyboard::Key key, EventType eventType);

    template<typename F>
    void bindActionToMouseButton(F callback, sf::Mouse::Button mouseButton, EventType eventType);
    template<typename O, typename F>
    void bindActionToMouseButton(O* object, F callback, sf::Mouse::Button mouseButton, EventType eventType);

    template<typename F>
    void bindActionToMouseMoved(F callback);
    template<typename O, typename F>
    void bindActionToMouseMoved(O* object, F callback);

    template<typename F>
    void bindActionToMouseWheelScrolled(F callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    template<typename O, typename F>
    void bindActionToMouseWheelScrolled(O* object, F callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);

    template<typename F>
    void bindActionToJoystickButton(F callback, unsigned int joystick, unsigned int button, EventType eventType);
    template<typename O, typename F>
    void bindActionToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button, EventType eventType);

    // StateInput

    template<typename F>
    void bindStateToKey(F callback, sf::Keyboard::Key key);
    template<typename O, typename F>
    void bindStateToKey(O* object, F callback, sf::Keyboard::Key key);

    template<typename F>
    void bindStateToMouseButton(F callback, sf::Mouse::Button button);
    template<typename O, typename F>
    void bindStateToMouseButton(O* object, F callback, sf::Mouse::Button button);
    template<typename F>

    void bindStateToJoystickButton(F callback, unsigned int joystick, unsigned int button);
    template<typename O, typename F>
    void bindStateToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button);

    template<typename F>
    void bindStateToJoystickAxis(F callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold,
                                 JoystickAxisPosition axisPosition);
    template<typename O, typename F>
    void bindStateToJoystickAxis(O* object, F callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold,
                                 JoystickAxisPosition axisPosition);

    // RangeInput

    template<typename F>
    void bindRangeToKeyboard(F callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    template<typename O, typename F>
    void bindRangeToKeyboard(O* object, F callback, sf::Keyboard::Key negativeKey, sf::Keyboard::Key positiveKey);
    template<typename F>
    void bindRangeToKeyboard(F callback, sf::Keyboard::Key key);
    template<typename O, typename F>
    void bindRangeToKeyboard(O* object, F callback, sf::Keyboard::Key key);

    template<typename F>
    void bindRangeToMouseButtons(F callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton);
    template<typename O, typename F>
    void bindRangeToMouseButtons(O* object, F callback, sf::Mouse::Button negativeButton, sf::Mouse::Button positiveButton);
    template<typename F>
    void bindRangeToMouseButton(F callback, sf::Mouse::Button button);
    template<typename O, typename F>
    void bindRangeToMouseButton(O* object, F callback, sf::Mouse::Button button);

    template<typename F>
    void bindRangeToHorizontalMouseMovement(F callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void bindRangeToHorizontalMouseMovement(O* object, F callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename F>
    void bindRangeToVerticalMouseMovement(F callback, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void bindRangeToVerticalMouseMovement(O* object, F callback, RangeRestriction rangeRestriction = RangeRestriction::None);

    template<typename F>
    void bindRangeToMouseWheelScroll(F callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void bindRangeToMouseWheelScroll(O* object, F callback, sf::Mouse::Wheel wheelAxis,
                                     RangeRestriction rangeRestriction = RangeRestriction::None);

    template<typename F>
    void bindRangeToJoystickButtons(F callback, unsigned int joystick, unsigned int negativeButton, unsigned int positiveButton);
    template<typename O, typename F>
    void bindRangeToJoystickButtons(O* object, F callback, unsigned int joystick, unsigned int negativeButton, unsigned int positiveButton);
    template<typename F>
    void bindRangeToJoystickButton(F callback, unsigned int joystick, unsigned int button);
    template<typename O, typename F>
    void bindRangeToJoystickButton(O* object, F callback, unsigned int joystick, unsigned int button);

    template<typename F>
    void bindRangeToJoystickAxis(F callback, unsigned int joystick, sf::Joystick::Axis axis,
                                 RangeRestriction rangeRestriction = RangeRestriction::None);
    template<typename O, typename F>
    void bindRangeToJoystickAxis(O* object, F callback, unsigned int joystick, sf::Joystick::Axis axis,
                                 RangeRestriction rangeRestriction = RangeRestriction::None);

private:
    // Binding helper functions
    void bindActionToKeyHelper(Callable<>* callback, sf::Keyboard::Key key, EventType eventType);
    void bindActionToMouseButtonHelper(Callable<>* callback, sf::Mouse::Button mouseButton, EventType eventType);
    void bindActionToJoystickButtonHelper(Callable<>* callback, unsigned int joystick, unsigned int button, EventType eventType);
    void bindActionToMouseMovedHelper(Callable<>* callback);
    void bindActionToMouseWheelScrolledHelper(Callable<>* callback, sf::Mouse::Wheel mouseWheelAxis, EventType mouseWheelDirection);
    void bindStateToJoystickAxisHelper(Callable<bool>* callback, unsigned int joystick, sf::Joystick::Axis axis, float threshold,
                                       JoystickAxisPosition axisPosition);
    void bindRangeToMouseWheelScrollHelper(Callable<float>* callback, sf::Mouse::Wheel wheelAxis);
    void bindRangeToMouseWheelScrollHelper(Callable<float>* callback, sf::Mouse::Wheel wheelAxis, RangeRestriction rangeRestriction);

    const InputManager& m_inputManager;

    std::vector<ActionInput*> m_actionInputs;
    std::vector<StateInput*> m_stateInputs;
    std::vector<RangeInput*> m_rangeInputs;
};

#include "InputContext.inl"

#endif // INPUTCONTEXT_H
