// generator.h
#ifndef GENERATOR_H
#define GENERATOR_H

#include <coroutine> // Include this if you're using C++20's <coroutine> header
// If you're using a pre-C++20 coroutine library, include that library's header instead

template <typename T>
struct generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        T value;
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        generator get_return_object() { return generator{handle_type::from_promise(*this)}; }
        void unhandled_exception() { std::terminate(); }
        std::suspend_always yield_value(T val)
        {
            value = val;
            return {};
        }
    };

    bool move_next() { return coro ? (coro.resume(), !coro.done()) : false; }
    T current_value() { return coro.promise().value; }

    generator(generator const &) = delete;
    generator(generator &&other) : coro(other.coro) { other.coro = {}; }
    ~generator()
    {
        if (coro)
            coro.destroy();
    }

private:
    generator(handle_type h) : coro(h) {}
    handle_type coro;
};

#endif // GENERATOR_H