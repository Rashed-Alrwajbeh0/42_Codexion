*This activity has been created as part of the 42 curriculum by ralrawaj.*

# Codexion

## Description

**Codexion** is a concurrency project written in C with POSIX threads. It is a variation of the classic *Dining Philosophers* problem, themed around a shared co-working space.

- `N` coders sit in a circle. Between each pair of neighbours there is one **USB dongle** (so there are exactly `N` dongles).
- To **compile**, a coder needs **both** the dongle on their left and the one on their right.
- After compiling, a coder **debugs**, then **refactors**, then goes back to wait for dongles again.
- A dongle that was just released needs a **cooldown** before it can be used again.
- If a coder goes too long without starting a compilation, they **burn out** and the simulation stops.
- The simulation ends successfully when every coder has compiled the required number of times.

The goal is to design a solution that is free of **deadlocks**, avoids **starvation**, respects **cooldowns**, detects **burnout precisely**, and prints a **clean, serialized log**.

Dongle allocation is decided by a **scheduler** that can be selected at launch:

| Scheduler | Behaviour |
|-----------|-----------|
| `fifo`    | Coders are served in the order in which they asked for dongles. |
| `edf`     | *Earliest Deadline First*: the coder whose burnout deadline (`last_compilation_time + time_to_burnout`) is closest is served first. |

### Overview of the architecture

- **Coder threads** (`N` of them): request dongles, compile, debug, refactor, repeat.
- **One monitor thread**: decides who receives dongles (scheduling), enforces cooldowns, and detects burnout / completion.
- **Priority queue**: an array (FIFO) or a binary min-heap (EDF) holding the coders that are waiting for dongles.

### Project structure

| File | Role |
|------|------|
| `main.c` | Entry point, single-coder special case, top-level cleanup |
| `arguments.c` | Argument parsing and validation |
| `init_dongles.c` | Dongle array, queue allocation, time helper (`calctime`) |
| `init_common.c` | Shared state (`stop` flag, `print` mutex, `queue_controler` mutex) |
| `init_coders.c` | Coder structures and left/right dongle assignment |
| `make_threads.c` | Orchestrates setup, run and teardown |
| `start_threads.c` | Creates coder threads and the monitor thread, joins them |
| `coder_work.c` | Coder life cycle (wait turn → compile → debug → refactor) |
| `monitor.c` | Burnout detection, queue serving, dongle granting |
| `priority_queue.c` / `priority_types.c` | FIFO queue, EDF heap and comparison logic |
| `free.c` / `helper_init_functions.c` | Logging, availability check, condition variables, cleanup |
| `structs.h` / `codexion.h` | Data structures and prototypes |

## Instructions

### Compilation

```bash
make        # build the executable
make clean  # remove object files
make fclean # remove object files and the executable
make re     # rebuild from scratch
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are required (exactly 8).

| Argument | Description |
|----------|-------------|
| `number_of_coders` | Number of coders (and of dongles) |
| `time_to_burnout` | Max time (ms) a coder can go without starting a compilation |
| `time_to_compile` | Time (ms) spent compiling while holding two dongles |
| `time_to_debug` | Time (ms) spent debugging |
| `time_to_refactor` | Time (ms) spent refactoring |
| `number_of_compiles_required` | Compilations each coder must complete |
| `dongle_cooldown` | Time (ms) a dongle is unavailable after being released |
| `scheduler` | `fifo` or `edf` |

All numeric arguments must be positive integers; invalid input prints an error and exits.

### Examples

```bash
./codexion 5 800 200 100 100 3 50 fifo   # 5 coders, FIFO scheduling
./codexion 5 800 200 100 100 3 50 edf    # same scenario, EDF scheduling
./codexion 1 500 100 100 100 3 0 fifo    # single coder: can never compile, burns out
```

### Output format

Every line has the form `<timestamp_ms> <coder_id> <message>`, where the timestamp is relative to program start. Possible messages:

```
has taken a dongle
is compiling
is debugging
is refactoring
burned out
```

Once a burnout occurs, no other message is printed after the `burned out` line.

## Blocking cases handled

### Deadlock prevention (Coffman's conditions)

A deadlock requires all four Coffman conditions to hold at once. The solution breaks several of them:

- **Hold and wait — broken.** Dongles are granted by the monitor **all-or-nothing**: a coder is only woken up when *both* of its dongles are free (`is_available`), and both are marked `is_in_use` in the same critical section (`grant_dongles`). A coder therefore never holds one dongle while waiting for the other.
- **Circular wait — broken.** As a second line of defence, mutex acquisition order is asymmetric: the last coder (`id == N`) locks left-then-right, every other coder locks right-then-left. This breaks the cycle that would otherwise exist around the table.
- **No preemption / mutual exclusion** are inherent to the resource (a dongle can only be used by one coder at a time), so they are kept; the two conditions above are enough to rule out deadlock.

Because the monitor reserves dongles before waking a coder, the per-dongle `pthread_mutex_lock` calls inside `take_dongles` are never contended in practice.

### Starvation prevention

- **FIFO:** requests are served in arrival order. When the monitor finds that the head of the queue cannot be served yet (dongle in use or cooling down), it puts the skipped coders back at the **front** of the queue (`add_to_queue_front`), preserving their original relative order so nobody is overtaken unfairly.
- **EDF:** the queue is a binary heap ordered by burnout deadline. Coders that are closest to burning out are served first, which directly targets the starvation that would cause a burnout. Coders that have never compiled are served first (ordered by id), then the rest by deadline.
- Because the monitor serves **every** coder in the queue on each pass (not only the head), a coder whose dongles are free is never blocked behind a coder whose dongles are busy.

### Dongle cooldown handling

When a coder releases its dongles, the release time is stored in each dongle (`last_use`). `is_available` only returns true when both dongles are not in use **and** `now - last_use >= dongle_cooldown` for each dongle that has already been used. A dongle is therefore never handed out during its cooldown.

### Precise burnout detection

- The monitor loop runs continuously with a very short sleep (`usleep(100)`, i.e. 0.1 ms), so a burnout is detected within a fraction of a millisecond of its deadline.
- Burnout is evaluated as `now - last_compilation_time >= time_to_burnout`, where `last_compilation_time` is the timestamp of the start of the coder's last compilation.
- When a burnout is detected the monitor sets the shared `stop` flag, wakes every coder that is still running (`free_after_finish`), and prints the `burned out` line with `burned_out_print`.
- The special case of a single coder (one dongle only, so compilation is impossible) is handled in `main`: it prints that the coder took a dongle, waits `time_to_burnout`, then prints `burned out`.

### Log serialization

All output goes through `print` (or `burned_out_print`), which locks a dedicated **`print` mutex** around `printf`. Lines from different threads can therefore never be interleaved. `print` also checks the shared `stop` flag first, so no line is emitted after a burnout has been declared; only `burned_out_print` bypasses that check, so the burnout message is always the last line.

### Clean termination

The monitor stops when either a burnout happens or all coders have finished (`finished_coders == number_of_coders`). On burnout, coders blocked on their condition variable are woken up, observe the `stop` flag and exit. All threads are joined before any shared memory is freed, so there are no use-after-free accesses.

## Thread synchronization mechanisms

The implementation uses only POSIX primitives: `pthread_mutex_t`, `pthread_cond_t`, `pthread_create` and `pthread_join`.

### Mutexes

| Mutex | Protects |
|-------|----------|
| **Dongle mutexes** (one per dongle, `t_dongle.dongle`) | Physical exclusive access to a dongle while a coder compiles |
| **`print`** | Standard output, so log lines are atomic and never interleaved |
| **`queue_controler`** | All shared scheduling state: the priority queue, `is_in_use` flags of dongles, each coder's `is_ready` and `last_compilation_time`, the `stop` flag, and the `finished_coders` counter |

### Condition variables

Each coder owns its **own** `pthread_cond_t` (allocated in `put_cond_variables`). A coder waits on it until the monitor grants it dongles, which avoids waking every thread (no thundering herd) and lets the monitor wake exactly the coder it chose.

### How coders and the monitor communicate

```
Coder                                         Monitor
-----                                         -------
add_to_queue(coder)  [queue_controler]  --->  top_priority() picks next coder
wait_turn(): while(!is_ready)                 is_available()? (dongles free +
    cond_wait(cond, queue_controler)            cooldown elapsed)
                                              grant_dongles():
                                                 is_in_use = 1 on both dongles
                                                 is_ready  = 1
                                                 cond_signal(coder->cond)
        <--- wakes up ---------------------------------+
take_dongles(), compile, release_dongles()
is_in_use = 0, last_use = now  [queue_controler]
finished_coders++  [queue_controler]    --->  check_finish_coders()
```

### How race conditions are prevented

- **Lost wakeups and spurious wakeups.** `wait_turn` always waits inside a `while (!coder->is_ready)` loop while holding `queue_controler`. The monitor sets `is_ready = 1` *under the same mutex* before signalling, so a signal can never be missed, even if it is sent before the coder starts waiting, and spurious wakeups are harmlessly re-checked.
- **Two coders claiming the same dongle.** The `is_in_use` flags are only read (`is_available`) and written (`grant_dongles`, `release_dongles`) by the monitor and the coders under `queue_controler`, and the monitor reserves both dongles before waking anyone.
- **Queue corruption.** Every push (`add_to_queue`, `add_to_queue_front`) and pop (`top_priority`) is performed with `queue_controler` held.
- **Burnout vs. compilation start.** `last_compilation_time` is written by the coder and read by the monitor, both under `queue_controler`, so the monitor never sees a half-updated value.
- **Shutdown races.** The `stop` flag is set by the monitor under `queue_controler`; each coder checks it under the same mutex right after being woken and returns without starting a new compilation. `free_after_finish` sets `is_ready` under the mutex before signalling, so blocked coders are guaranteed to wake up and exit.
- **Log races.** Only `print` / `burned_out_print` write to stdout, always while holding the `print` mutex.
- **Memory safety.** The main thread joins every coder thread and the monitor thread before freeing the shared structures.

## Resources

### References

- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`, `man pthread_cond_signal`, `man gettimeofday`, `man usleep`
- [Dining philosophers problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

### Use of AI

AI (Claude) was used to **help with structure this README.md** from the project's source code .


testing of the project .

help with norminette
