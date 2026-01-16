# Philosophers

## Description

Philosophers is a multithreading and synchronization project that simulates the classic Dining Philosophers Problem. The challenge is to manage multiple philosophers sitting at a round table, where each philosopher must alternate between eating, sleeping, and thinking. However, they need two forks to eat, and there's only one fork between each pair of philosophers.

The goal is to implement a solution that prevents deadlock, avoids data races, and ensures no philosopher dies of starvation. This project teaches fundamental concepts of concurrent programming, thread management, mutex synchronization, and race condition prevention.

## The Dining Philosophers Problem

The scenario:
- Philosophers sit at a round table with a bowl of spaghetti in the center
- There are as many forks as philosophers on the table
- Each philosopher needs TWO forks to eat
- After eating, philosophers put down their forks and sleep
- After sleeping, they wake up and think
- The simulation stops when a philosopher dies of starvation OR when all philosophers have eaten a specified number of times

**The Challenge:** How do you prevent deadlock (all philosophers holding one fork and waiting forever) while ensuring no philosopher starves?

## Instructions

### Clone the Repository

```bash
git clone https://github.com/diomende/philosophers.git
cd philosophers
```

### Compilation

```bash
cd philo
make
```

This will compile the project and create the `philo` executable.

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

**Parameters:**
- `number_of_philosophers`: Number of philosophers (and forks)
- `time_to_die`: Time in milliseconds - if a philosopher doesn't start eating within this time since their last meal, they die
- `time_to_eat`: Time in milliseconds it takes for a philosopher to eat
- `time_to_sleep`: Time in milliseconds a philosopher spends sleeping
- `number_of_times_each_philosopher_must_eat` (optional): Simulation stops when all philosophers have eaten at least this many times

**Example:**
```bash
./philo 5 800 200 200
./philo 5 800 200 200 7
./philo 4 410 200 200
./philo 2 310 200 100
```

## Implementation Details

### Approach: Threads with Mutexes

This implementation uses **POSIX threads (pthreads)** with **mutexes** for synchronization. Each philosopher is represented by a thread, and each fork is protected by a mutex.

### Key Design Decisions

#### 1. Deadlock Prevention Strategy

**Problem:** If all philosophers grab their left fork simultaneously, they'll wait forever for the right fork (deadlock).

**Solution:** Implemented an asymmetric fork-grabbing strategy:
- **Even-numbered philosophers** (2, 4, 6...): Grab left fork first, then right fork
- **Odd-numbered philosophers** (1, 3, 5...): Grab right fork first, then left fork

This breaks the circular wait condition and prevents deadlock.

```c
// From philo_actions.c
void grab_forks(t_philo *philo)
{
    if (philo->phil_id % 2 == 0)
    {
        pthread_mutex_lock(philo->l_fork);
        pthread_mutex_lock(philo->r_fork);
    }
    else
    {
        pthread_mutex_lock(philo->r_fork);
        pthread_mutex_lock(philo->l_fork);
    }
}
```

#### 2. Data Race Prevention

Three critical mutexes protect shared data:

- **`write_lock`**: Protects console output to prevent message interleaving
- **`meal_lock`**: Protects philosopher's last meal time and eat count
- **`death_lock`**: Protects the global "all_philos_alive" flag

All access to shared data is wrapped in mutex locks to ensure thread safety.

#### 3. Death Detection: The Waiter Thread

Instead of having philosophers check each other's status, a separate "waiter" monitoring function runs in the main thread:

```c
void waiter_routine(t_master *master)
{
    while (1)
    {
        // Check if all philosophers have eaten enough
        if (eat_limit_reached(master))
            return;
        
        // Check if any philosopher has died
        if (death_detector(master))
            return;
        
        usleep(500);
    }
}
```

**Why this approach?**
- Centralized monitoring reduces complexity
- Each philosopher thread focuses only on eating, sleeping, thinking
- Avoids philosophers constantly checking others' status
- Cleaner separation of concerns


#### 4. Staggered Start for Even Philosophers

Even-numbered philosophers wait 10ms before starting:

```c
if (philo->phil_id % 2 == 0)
    usleep_manager(philo, 10);
```

**Purpose:**
- Reduces initial fork contention
- Helps establish a rhythm where philosophers naturally alternate
- Improves simulation stability, especially with tight timing constraints

#### 5. Special Case: Single Philosopher

Handled separately in main():

```c
if (p_nbr == 1)
{
    printf("0 1 has taken a fork\n");
    usleep(ft_atoi(av[2]) * 1000);
    printf("%li 1 died\n", ft_atoi(av[2]));
    return (0);
}
```

A single philosopher can only grab one fork and will inevitably die.

### Program Flow

1. **Initialization Phase:**
   - Parse and validate arguments
   - Allocate master structure
   - Initialize simulation info (timing, philosopher count)
   - Create and initialize all mutexes (forks, write_lock, meal_lock, death_lock)
   - Initialize philosopher array with proper fork assignments

2. **Simulation Phase:**
   - Create a thread for each philosopher
   - Each philosopher executes their routine: eat → sleep → think (repeat)
   - Main thread runs waiter_routine to monitor for death or completion
   - Simulation ends when a philosopher dies OR all have eaten enough times

3. **Cleanup Phase:**
   - Join all philosopher threads
   - Destroy all mutexes
   - Free all allocated memory

### Data Structures

```c
typedef struct s_master {
    t_info   *info;        // Simulation parameters
    t_philo  *philo_arr;   // Array of philosophers
    t_mutex  *mutex;       // All mutexes
} t_master;

typedef struct s_philo {
    t_info          *info;
    t_mutex         *mutex;
    pthread_t       thread;
    pthread_mutex_t *l_fork;     // Left fork
    pthread_mutex_t *r_fork;     // Right fork
    long            lst_eat_tm;  // Last meal timestamp
    int             phil_id;     // Philosopher ID (1-indexed)
    int             eat_count;   // Meals eaten
    int             philo_hunger;// IS_HUNGRY or IS_FULL
} t_philo;
```

## Testing

### Basic Tests

```bash
# No one should die
./philo 5 800 200 200

# No one should die, simulation should stop after each eats 7 times
./philo 5 800 200 200 7

# A philosopher should die
./philo 4 310 200 100

# One philosopher should die
./philo 1 800 200 200
```

### What to Look For

✅ **Good signs:**
- No philosopher dies when they shouldn't
- Death is reported within 10ms of actual death time
- No message appears after a death message
- Messages don't overlap or appear scrambled
- Simulation stops correctly when all philosophers are full

## Challenges Faced & Solutions

### Challenge 1: Race Conditions on Shared Data
**Problem:** Multiple threads accessing `last_eat_time` and `all_philos_alive` caused unpredictable behavior.

**Solution:** Implemented separate mutexes (`meal_lock`, `death_lock`) to protect each category of shared data.

### Challenge 2: Accurate Death Detection
**Problem:** Large usleep intervals caused delayed death detection.

**Solution:** Created `usleep_manager()` that checks death status every 500 microseconds while sleeping, ensuring deaths are detected within milliseconds.

### Challenge 3: Deadlock Risk
**Problem:** If all philosophers grab one fork simultaneously, circular waiting occurs.

**Solution:** Asymmetric fork-grabbing (even vs odd philosophers) breaks the circular wait condition.

### Challenge 4: Output Synchronization
**Problem:** Multiple threads printing simultaneously caused garbled output.

**Solution:** All output goes through `write_action()` protected by `write_lock` mutex.

## Key Learnings

Through this project, I gained practical experience with:
- **Multithreading:** Creating, managing, and joining POSIX threads
- **Synchronization:** Using mutexes to protect shared resources and prevent race conditions
- **Deadlock Prevention:** Implementing strategies to avoid circular waiting
- **Concurrent Programming Patterns:** Designing thread-safe code and understanding critical sections
- **Timing and Precision:** Working with microsecond-level timing in multithreaded environments
- **Resource Management:** Proper cleanup of threads and mutexes
- **Debugging Concurrent Programs:** Identifying and fixing race conditions, deadlocks, and timing issues
- **Algorithm Design:** Breaking down complex problems into manageable, thread-safe components
