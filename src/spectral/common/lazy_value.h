#ifndef COMMON_LAZY_VALUE_H
#define COMMON_LAZY_VALUE_H
#include <optional>
#include <functional>
#include <stdexcept>

template<typename T>
class LazyValue
{
public:
    template<typename P>
    LazyValue(const P &init)
        : constructor([init]() -> T { return init(); }) {}

    LazyValue(LazyValue &&other)
        : value(std::move(other.value)) {}

    LazyValue(const LazyValue &) = delete;

    LazyValue &operator=(const LazyValue &) = delete;

    inline const T &get() const
    {
        if(!value) {
            value.emplace(std::move(constructor()));
        }
        return *value;
    }

    const T &operator*() const
    {
        return get();
    }

    const T *operator->() const
    {
        return &get();
    }

private:
    const std::function<T()> constructor;
    mutable std::optional<T> value;
};

template<typename T, typename P, typename = std::enable_if_t<!std::is_same<T, P>::value, void>>
LazyValue<T> make_lazy(const P &constructor)
{
    return LazyValue<T>(constructor);
}


template<typename T>
class LazyPtr
{
public:
    template<typename P>
    LazyPtr(const P &init)
        : constructor([init]() -> T *{ return init(); }) {}

    LazyPtr(LazyPtr &&other)
        : constructor(), value()
    {
        std::swap(value, other.value);
    }

    ~LazyPtr()
    {
        if constexpr(std::is_array<T>::value) {
            delete[] value;
        }
        else {
            delete value;
        }
    }

    LazyPtr(const LazyPtr &) = delete;

    LazyPtr &operator=(const LazyPtr &) = delete;
    LazyPtr &operator=(const LazyPtr &&other)
    {
        std::swap(value, other.value);
    }

    inline const T *get() const
    {
        if(value == nullptr) {
            value = constructor();
            if(value == nullptr) throw std::runtime_error("Incorrect constructor in LazyPtr");
        }
        return value;
    }

    const T &operator*() const
    {
        return *get();
    }

    const T *operator->() const
    {
        return get();
    }

private:
    const std::function<T *()> constructor;
    mutable T *value = nullptr;
};

template<typename T, typename P, typename = std::enable_if_t<!std::is_same<T, P>::value, void>>
LazyPtr<T> make_lazy_ptr(const P &constructor)
{
    return LazyPtr<T>(constructor);
}



#endif