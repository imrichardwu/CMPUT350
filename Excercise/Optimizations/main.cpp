#include "stonesngems.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace stonesngems;

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

constexpr std::size_t NUM_STEPS = 100000;
constexpr std::size_t MILLISECONDS_PER_SECOND = 1000;

void test_speed() {
    RNDGameState state(kDefaultGameParams);

    std::vector<Action> actions;
    std::cout << "starting ..." << std::endl;

    const auto t1 = high_resolution_clock::now();
    for (std::size_t i = 0; i < NUM_STEPS; ++i) {
        state.legal_actions(actions);
        for (const auto &a : actions) {
            state.apply_action(a);
        }

        const uint64_t hash = state.get_hash();
        if (hash == 0) {
            // Should never happen
            break;
        }
    }
    const auto t2 = high_resolution_clock::now();
    const duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "Total time for " << NUM_STEPS << " steps: " << ms_double.count() / MILLISECONDS_PER_SECOND
              << std::endl;
    std::cout << "Time per step :  " << ms_double.count() / MILLISECONDS_PER_SECOND / NUM_STEPS << std::endl;
}

int main() {
    test_speed();
}
