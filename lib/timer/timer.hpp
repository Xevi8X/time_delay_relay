#pragma once

class Timer {
public:
    /// @brief One tick of the timer.
    /// @return true if the timer has ended, false otherwise.
    bool tick() {
        if (count > 0) {
            --count;
        }
        return count == 0;
    }

    /// @brief Reset the timer to its initial state.
    void reset() {
        count = init_count;
    }

    void set_init_count(unsigned int ticks) {
        init_count = ticks;
        reset();
    }

    unsigned int get_init_count() const {
        return init_count;
    }

    unsigned int get_count() const {
        return count;
    }

private:
    unsigned int count = 0U;
    unsigned int init_count = 0U;
};