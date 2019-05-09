#ifndef CALLABLES_H
#define CALLABLES_H

template<typename... Args>
class Callable
{
public:
    virtual ~Callable() = default;

    virtual void operator()(const Args&... args) = 0;
};

template<typename F, typename... Args>
class Functor : public Callable<Args...>
{
public:
    Functor(F function)
        : m_function(function)
    {
    }

    virtual void operator()(const Args&... args) override { m_function(args...); }

private:
    F m_function;
};

template<typename O, typename F, typename... Args>
class Method : public Callable<Args...>
{
public:
    Method(O* object, F method)
        : m_object(object)
        , m_method(method)
    {
    }

    virtual void operator()(const Args&... args) override { (m_object->*m_method)(args...); }

private:
    O* m_object;
    F m_method;
};

#endif // CALLABLES_H
