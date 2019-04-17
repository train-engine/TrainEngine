#ifndef CALLBACKS_H
#define CALLBACKS_H

template<typename... Args>
class Callback
{
public:
    virtual ~Callback() {}

    virtual void operator()(const Args&... args) = 0;
};

template<typename Callable, typename... Args>
class CallbackFunctor : public Callback<Args...>
{
public:
    CallbackFunctor(Callable function)
        : m_function(function)
    {
    }

    virtual void operator()(const Args&... args) override { m_function(args...); }

private:
    Callable m_function;
};

template< typename Object, typename Callable, typename... Args>
class CallbackMember : public Callback<Args...>
{
public:
    CallbackMember(Object* object, Callable method)
        : m_method(method)
        , m_object(object)
    {
    }
    
    virtual void operator()(const Args&... args) override { (m_object->*m_method)(args...); }

private:
    Callable m_method;
    Object* m_object;
};

#endif //CALLBACKS_H