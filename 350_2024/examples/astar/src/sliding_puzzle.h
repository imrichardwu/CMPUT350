#ifndef SLIDING_PUZZLE_H_
#define SLIDING_PUZZLE_H_

#include <array>

#include "base_game.h"
#include "util.h"

class SlidingPuzzleState : public BaseState {
public:
    SlidingPuzzleState(const std::string &board_string);
    SlidingPuzzleState(const SlidingPuzzleState &) = default;
    SlidingPuzzleState& operator=(const SlidingPuzzleState &) = default;

    ~SlidingPuzzleState() = default;

    // Access
    std::vector<int> child_actions() const override;
    void apply_action(int action) override;
    double get_heuristic() const override;
    uint64_t get_hash() const override;
    bool is_solution() const override;
    std::string to_str() const override;

    // Cloning and AOs
    void assign(BaseState *other) override {
        SlidingPuzzleState *o = static_cast<SlidingPuzzleState *>(other);
        board = o->board;
        hash = o->hash;
        empty_idx = o->empty_idx;
    }

    SlidingPuzzleState *clone_impl() const override {
        return new SlidingPuzzleState(*this);
    }

    bool is_equal(const BaseState &other) const override {
        auto o = static_cast<const SlidingPuzzleState &>(other);
        return board == o.board;
    }

    static const int ROWS = 4;
    static const int COLS = 4;
    inline static const std::string name{"sliding_puzzle"};

private:
    std::array<int, ROWS * COLS> board;             // Board
    uint64_t hash = 0;
    int empty_idx = -1;
};


class SlidingPuzzleState2 {
public:
    SlidingPuzzleState2() = delete;
    SlidingPuzzleState2(const std::string &board_string);
    SlidingPuzzleState2(const SlidingPuzzleState2 &) = default;
    SlidingPuzzleState2& operator=(const SlidingPuzzleState2 &) = default;
    bool operator==(const SlidingPuzzleState2 &other) const {
        return board == other.board;
    }

    // Access
    std::vector<int> child_actions() const;
    void apply_action(int action);
    double get_heuristic() const;
    uint64_t get_hash() const;
    bool is_solution() const;
    std::string to_str() const;

    static const int ROWS = 4;
    static const int COLS = 4;
    inline static const std::string name{"sliding_puzzle"};

private:
    std::array<int, ROWS * COLS> board;             // Board
    uint64_t hash = 0;
    int empty_idx = -1;
};

#endif    // SLIDING_PUZZLE_H_