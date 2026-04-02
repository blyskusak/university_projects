#ifndef EVENTSTREAM_H
#define EVENTSTREAM_H

#include <utility>
#include <optional>
#include <tuple>
#include <map>
#include <type_traits>
#include <limits>
#include <cstddef>

namespace eventstream {
    enum class control {
        continue_,
        stop
    };

    auto emit(auto x) {
        return [x](auto observer, auto state) {
            auto [new_state, ctrl] = observer(x, state);
            return new_state;
        };
    }

    auto generate(auto init, auto step) {
        return [init, step](auto observer, auto state) mutable {
            using StepRet = decltype(step(init));
            using ValType = typename StepRet::value_type;

            std::optional<ValType> current_val(std::in_place, std::move(init));
            
            auto current_state = state;

            while (true) {
                auto [next_state, ctrl] = observer(*current_val, current_state);
                // Setting returned state as current one.
                current_state = next_state;

                // If observer returned stop signal, end generate.
                if (ctrl == control::stop)
                    break;


                auto next_val = step(*current_val);
                if (!next_val.has_value())
                    break;
            
                // Setting current val safely (depending on if its lambda or normal value).
                if constexpr (std::is_move_assignable_v<ValType>) {
                    *current_val = std::move(*next_val);
                } else {
                    current_val.emplace(std::move(*next_val));
                }
            }

            return current_state;
        };
    }

    auto counter() {
        return [current_val = 1](auto observer, auto state) mutable {
            auto current_state = state;

            while (true) {
                auto [next_state, ctrl] = observer(current_val, current_state);
                current_state = next_state;

                if (current_val == std::numeric_limits<int>::max())
                    current_val = std::numeric_limits<int>::min();
                else
                    current_val++;

                if (ctrl == control::stop)
                    break;
            }
            return current_state;
        };
    }

    auto map(auto f, auto s) {
        return [f, s](auto observer, auto state) mutable {
            // Used wrapped observer to use f on data from s.
            auto wrapped_observer = [f, observer](auto event, auto st) mutable {
                return observer(f(event), st);
            };
            return s(wrapped_observer, state);
        };
    }

    auto filter(auto pred, auto s) {
        return [pred, s](auto observer, auto state) mutable {
            // Used wrapper observer to use pred on data from s.
            auto wrapped_observer = [pred, observer](auto event, auto st) mutable
                -> std::pair<decltype(st), control> 
            {
                if (pred(event))
                    return observer(event, st);
                return {st, control::continue_};
            };

            return s(wrapped_observer, state);
        };
    }

    auto take(std::size_t n, auto s) {
        return [n, s](auto observer, auto state) mutable {
            // The same as earlier, but waiting for count to hit n.
            auto wrapped_observer = [n, observer, count = std::size_t{0}](auto event, auto st) mutable 
                -> std::pair<decltype(st), control> 
            {
                if (count >= n)
                    return {st, control::stop};
                
                count++;
                auto [new_state, ctrl] = observer(event, st);
                
                if (count == n)
                    return {new_state, control::stop};
                return {new_state, ctrl};
            };
            
            return s(wrapped_observer, state);
        };
    }

    auto flatten(auto ss) {
        return [ss](auto observer, auto state) mutable {
            bool stop_requested = false;

            // Observer for stream of streams.
            auto outer_observer = [&](auto sub_stream, auto current_state) mutable
                -> std::pair<decltype(current_state), control> 
            {
                // Observer for sub_stream.
                auto inner_observer = [&](auto event, auto st) mutable
                    -> std::pair<decltype(st), control> 
                {
                    auto [new_st, ctrl] = observer(event, st);
                    
                    if (ctrl == control::stop)
                        stop_requested = true;
                    
                    return {new_st, ctrl};
                };

                auto final_inner_state = sub_stream(inner_observer, current_state);

                if (stop_requested)
                    return {final_inner_state, control::stop};
                return {final_inner_state, control::continue_};
            };
            return ss(outer_observer, state);
        };
    }

    // Detail namespace.
    namespace detail {
        template <typename F>
        struct pipe_closure {
            F f;
        };

        template <typename F>
        auto make_pipe(F&& f) {
            return pipe_closure<std::decay_t<F>>{std::forward<F>(f)};
        }

        // Used to get what it returns and takes as input.
        template <typename T>
        struct function_traits : function_traits<decltype(&T::operator())> {};

        // Traditional functions.
        template <typename R, typename... Args>
        struct function_traits<R(*)(Args...)> {
            using args_tuple = std::tuple<std::decay_t<Args>...>;
            using return_type = R;
        };

        // Const lambdas.
        template <typename C, typename R, typename... Args>
        struct function_traits<R(C::*)(Args...) const> {
            using args_tuple = std::tuple<std::decay_t<Args>...>;
            using return_type = R;
        };

        // Mutable lambdas.
        template <typename C, typename R, typename... Args>
        struct function_traits<R(C::*)(Args...)> {
            using args_tuple = std::tuple<std::decay_t<Args>...>;
            using return_type = R;
        };

        // Wrapper for memoize.
        template <typename F>
        struct memoize_wrapper {
            using Traits = function_traits<std::decay_t<F>>;
            using ArgsTuple = typename Traits::args_tuple;
            using ActualRetType = typename Traits::return_type;
            
            // MapValueType is used to keep value in map with info if returned value is void.
            static constexpr bool is_void = std::is_same_v<ActualRetType, void>;
            using MapValueType = std::conditional_t<is_void, bool, ActualRetType>;

            F f;
            std::map<ArgsTuple, MapValueType> cache;

            // Constructor.
            memoize_wrapper(F func) : f(std::move(func)) {}

            template <typename... U>
            ActualRetType operator()(U&&... args) {
                ArgsTuple key{std::forward<U>(args)...};
                
                // Checking if value with key was already evaluated.
                auto it = cache.find(key);
                if (it != cache.end()) {
                    if constexpr (!is_void) return it->second;
                    else return;
                }

                // If it is not in cache, we evaluate it and add to cache.
                if constexpr (is_void) {
                    std::apply(f, key);
                    cache.insert({key, true});
                    return;
                } else {
                    ActualRetType result = std::apply(f, key);
                    cache.insert({key, result});
                    return result;
                }
            }
        };
    }

    template <typename Stream, typename F>
    auto operator|(Stream s, detail::pipe_closure<F> pc) {
        return pc.f(s);
    }

    // Used for operator| - when function gets only one input (or 0 in case of flatten).
    // They get less input values in this operator.
    auto map(auto f) {
        return detail::make_pipe([f](auto s) {
            return map(f, s);
        });
    }

    auto filter(auto pred) {
        return detail::make_pipe([pred](auto s) {
            return filter(pred, s);
        });
    }

    auto take(std::size_t n) {
        return detail::make_pipe([n](auto s) {
            return take(n, s);
        });
    }

    auto flatten() {
        return detail::make_pipe([](auto ss) {
            return flatten(ss);
        });
    }

    auto tap(auto side_effect) {
        return detail::make_pipe([side_effect](auto s) {
            return [side_effect, s](auto observer, auto state) mutable {
                // Adding side effect on event.
                auto wrapped_observer = [side_effect, observer](auto event, auto st) mutable {
                    side_effect(event);
                    return observer(event, st);
                };
                return s(wrapped_observer, state);
            };
        });
    }

    auto memoize(auto f) {
        return detail::memoize_wrapper<std::decay_t<decltype(f)>>(f);
    }
} 

#endif // EVENTSTREAM_H