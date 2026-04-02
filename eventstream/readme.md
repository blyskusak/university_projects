# Event Stream Processing (C++23)

## Project Description
This project is a header-only library written in modern C++ (C++23), designed for functional event stream processing. It allows building complex data processing pipelines using lazy evaluation, closures (lambdas), and advanced template metaprogramming.

Streams in this library are evaluated on-demand (pull/push model) using injected observers that manage the state and dictate whether the data flow should continue or stop.

## Core Mechanics and Architecture
* **Functional Programming:** The library fully embraces immutable stream logic, relying on state passing and lambda expressions to process data.
* **Pipelining:** Overloads the `|` (pipe) operator to allow clean, declarative chaining of operations (e.g., `stream | filter(pred) | map(func)`).
* **Advanced Metaprogramming (Type Traits):** The `memoize` function utilizes a custom `function_traits` implementation that deduces argument types at compile time and converts them into a `std::tuple` to act as a cache key in a `std::map`.
* **Lazy Generation:** Streams like `counter` or `generate` produce values only when the subsequent observer is ready for them, avoiding unnecessary computations.
* **Type Safety and Conversions:** The library gracefully handles implicit argument conversions and correctly caches functions returning `void` using `std::conditional_t`.

## Available Operations
All operations are enclosed within the `eventstream` namespace.

### Stream Generators (Sources)
* `emit(x)`: Creates a stream emitting a single event.
* `generate(init, step)`: Generates an infinite (or finite) stream based on an initial state and a step function returning `std::optional`.
* `counter()`: Generates an infinite stream of consecutive integers (with underflow/overflow handling).

### Transformers and Filters (Operators)
* `map(f)`: Transforms each event using the provided function.
* `filter(pred)`: Passes through only the events that satisfy the predicate.
* `take(n)`: Limits the stream to the first `n` events, dispatching a stop signal (`control::stop`) afterwards.
* `flatten()`: Flattens a stream of streams into a single, continuous stream.
* `tap(side_effect)`: Injects a side effect (e.g., logging) without altering the events or the control flow.

### Utility Tools
* `memoize(f)`: Wraps a given function, adding a caching layer to prevent the re-evaluation of previously computed argument sets. Each wrapper instance maintains its own independent cache.

## Requirements and Compilation
The library strictly requires a compiler supporting the **C++23** standard. 
As a header-only library, integration requires simply including the file in your source code:
```cpp
#include "eventstream.h"
```

Example compilation command for a program using this library (e.g., GCC):
```bash
g++ -Wall -Wextra -O2 -std=c++23 main.cpp
```
