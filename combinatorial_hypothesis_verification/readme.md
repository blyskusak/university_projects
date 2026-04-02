# Combinatorial Hypothesis Verification

## Project Description
This project is a high-performance, multithreaded C application designed to computationally verify a combinatorial hypothesis regarding `d`-bounded multisets. The program calculates the maximum sum `α(d, A_0, B_0)` for a pair of undisputed (bezsporne), `d`-bounded multisets `A` and `B`, extending given initial multisets `A_0` and `B_0`.

The core problem requires exploring a massive state space using a backtracking algorithm. To achieve optimal execution times within strict system constraints, the original recursive, single-threaded algorithm was refactored into a custom iterative approach and then highly parallelized using POSIX Threads (`pthreads`).

## Core Mechanics and Constraints
* **Undisputed Multisets:** Two multisets `A` and `B` are considered undisputed if their total sums are equal, but the sums of any of their non-empty subsets are strictly different.
* **`d`-Boundedness:** All elements within the multisets belong to the set `{1, ..., d}`.
* **Bitset Optimization:** To prevent costly sum recalculations, the sets of all possible subset sums (`A_Σ` and `B_Σ`) are maintained incrementally using highly optimized bitwise operations (provided via a custom `sumset.h` library).
* **Strict Memory Limits:** The application strictly operates within a 128 MiB address space limit per thread to prevent excessive memory consumption during deep tree explorations.

## Project Structure
* `nonrecursive/`: Contains the single-threaded, iterative implementation. It introduces a custom memory-pooled stack to replace system call stack overhead and manage dynamic memory efficiently.
* `parallel/`: Contains the fully multithreaded implementation utilizing `pthreads`.
* `common/`: Shared I/O handling and bitset math libraries.
* `reference/`: Baseline recursive implementation used for correctness verification.

## Parallelization Strategy
The multithreading model utilizes a **Lock-Free Static Partitioning (Seed-and-Distribute)** approach to completely eliminate synchronization overhead during the heavy computation phase:

1. **Pre-generation (Seeding):** Before spawning any threads, the main thread unrolls the top levels of the recursion tree (up to a specific depth) to generate a global array of independent starting tasks.
2. **Lock-Free Distribution:** Tasks are assigned to threads using a round-robin stride (`i += thread_count` based on `thread_id`). This guarantees that threads operate on completely disjoint subsets of the task array, requiring zero mutex locks or atomic operations during the traversal.
3. **Custom Stack & Memory Pooling:** Each thread explores its assigned sub-trees using a custom iterative stack. To adhere to the 128 MiB memory limit and avoid the immense overhead of calling `malloc`/`free` for every node, the stack utilizes chunk-based memory pools (`sumset_pool`). Memory is allocated in batches and reused as the stack grows and shrinks.
4. **Local Aggregation:** Each thread maintains its own local `best_solution`. Once all threads join, the main thread simply aggregates the results to find the global maximum, preventing race conditions on the shared output state.

## Build & Run Instructions
The project uses `CMake` for building and requires a GCC compiler (>= 12.2) supporting the `-std=gnu17`, `-O3`, `-march=native`, and `-pthread` flags.

**Building:**
```bash
cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release
cd build/
make
```

**Running the parallel version:**
The program expects standard input in the following format:
1. Thread count (`t`), parameter `d`, size of `A_0` (`n`), size of `B_0` (`m`).
2. `n` elements of `A_0`.
3. `m` elements of `B_0`.

Example (8 threads, `d=10`, `A_0=∅`, `B_0={1}`):
```bash
echo -n -e '8 10 0 1\n1\n\n' | ./parallel/parallel
```

## Performance & Scalability
A comprehensive scalability analysis is provided in `report.pdf`. The performance of the parallel implementation was measured across various `d` values (e.g., `d` ∈ `{5, 10, 15, 20, 25, 30, 32, 34}`) and thread counts (1 to 64), demonstrating significant, near-linear speedups for highly complex sub-trees compared to the baseline single-threaded execution.
