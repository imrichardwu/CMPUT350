#ifndef UTIL_H
#define UTIL_H

#include <array>
#include <climits>
#include <cstdint>
#include <ctime>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>

inline std::vector<std::string> str_split(const std::string &s, const char delimiter) {
    std::stringstream ss(s);
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(ss, segment, delimiter)) {
        seglist.push_back(segment);
    }
    return seglist;
}

template <int MAX_SIZE, int NUM_CLASSES>
struct Zob {
    std::array<std::array<uint64_t, NUM_CLASSES>, MAX_SIZE> table;

    Zob(int seed = 0) {
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint64_t> dis;
        for (int i = 0; i < MAX_SIZE; ++i) {
            for (int c = 0; c < NUM_CLASSES; ++c) {
                table[i][c] = dis(gen);
            }
        }
    }
};

class Timer {
    // Ensure clock_t is a 64 bit value
    // A 32 bit clock_t width will overflow after ~72 minutes which is longer than the expected runtime.
    // A 64 bit clock_t width will overflow after ~ 300,00 years
    static_assert(sizeof(std::clock_t) == 8);

public:
    Timer() = default;
    Timer(double seconds_limit) : seconds_limit(seconds_limit) {}

    void start() {
        cpu_start_time = std::clock();
    }

    bool is_timeout() {
        std::clock_t cpu_current_time = std::clock();
        double current_duration = ((cpu_current_time - cpu_start_time) / (double)CLOCKS_PER_SEC);
        return seconds_limit > 0 && current_duration >= seconds_limit;
    }

    double get_duration() {
        std::clock_t cpu_current_time = std::clock();
        double current_duration = ((cpu_current_time - cpu_start_time) / (double)CLOCKS_PER_SEC);
        return current_duration;
    }

    double get_time_remaining() {
        std::clock_t cpu_current_time = std::clock();
        double current_duration = ((cpu_current_time - cpu_start_time) / (double)CLOCKS_PER_SEC);
        return seconds_limit - current_duration;
    }

private:
    double seconds_limit = std::numeric_limits<double>::max();
    std::clock_t cpu_start_time = 0;
};

#endif    // UTIL_H