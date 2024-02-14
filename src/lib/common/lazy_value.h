#ifndef COMMON_LAZY_VALUE_H
#define COMMON_LAZY_VALUE_H
#include <optional>
#include <functional>

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

#endif