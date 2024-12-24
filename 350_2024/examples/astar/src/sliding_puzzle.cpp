#include "sliding_puzzle.h"

#include <cassert>
#include <cmath>
#include <unordered_map>

#include "util.h"

// ------------------------- Consts and definitions -------------------------
const std::unordered_map<int, std::string> kBoardStrMap{
    {0, "  "}, {1, " 1"}, {2, " 2"},  {3, " 3"},  {4, " 4"},  {5, " 5"},  {6, " 6"},  {7, " 7"},
    {8, " 8"}, {9, " 9"}, {10, "10"}, {11, "11"}, {12, "12"}, {13, "13"}, {14, "14"}, {15, "15"},
};

constexpr int TOTAL_SIZE = SlidingPuzzleState::ROWS * SlidingPuzzleState::COLS;
const std::array<int, TOTAL_SIZE> kSolutionBoard{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
// const std::array<int, TOTAL_SIZE> kSolutionBoard{1, 2, 3, 4, 5, 6, 7, 8, 0};
const Zob<TOTAL_SIZE, TOTAL_SIZE> zobrist_table;    // Table for hashing

enum Direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
constexpr int NUM_ACTIONS = 4;

bool in_bounds(Direction direction, int idx) {
    int row = idx / SlidingPuzzleState::COLS;
    int col = idx % SlidingPuzzleState::COLS;
    switch (direction) {
        case Direction::UP:
            return row > 0;
        case Direction::RIGHT:
            return col < SlidingPuzzleState::COLS - 1;
        case Direction::DOWN:
            return row < SlidingPuzzleState::ROWS - 1;
        case Direction::LEFT:
            return col > 0;
        default:
            __builtin_unreachable();
    }
}

// Not safe, doesn't validate direction is legal
int get_new_idx(Direction direction, int idx) {
    switch (direction) {
        case Direction::UP:
            return idx - SlidingPuzzleState::COLS;
        case Direction::RIGHT:
            return idx + 1;
        case Direction::DOWN:
            return idx + SlidingPuzzleState::COLS;
        case Direction::LEFT:
            return idx - 1;
        default:
            __builtin_unreachable();
    }
}

double manhattan_distance(int idx1, int idx2) {
    int row1 = idx1 / SlidingPuzzleState::COLS;
    int col1 = idx1 % SlidingPuzzleState::COLS;
    int row2 = idx2 / SlidingPuzzleState::COLS;
    int col2 = idx2 % SlidingPuzzleState::COLS;
    return std::abs(static_cast<double>(row1 - row2)) + std::abs(static_cast<double>(col1 - col2));
}

//  ------------------------- Member functions -------------------------
SlidingPuzzleState::SlidingPuzzleState(const std::string &board_string) {
    // Not safe, doesn't validate board input
    // Example board input: 14,13,15,7,11,12,9,5,6,0,2,1,4,8,10,3
    int idx = -1;
    hash = 0;
    std::vector<std::string> board_str_segment = str_split(board_string, ',');
    assert(board_str_segment.size() == board.size());

    for (auto const &s : board_str_segment) {
        int p = std::stoi(s);
        board[++idx] = p;
        hash ^= zobrist_table.table[idx][p];
        if (p == 0) {
            empty_idx = idx;
        }
    }
}

std::vector<int> SlidingPuzzleState::child_actions() const {
    // Actions in reference to moving empty tile
    std::vector<int> actions;
    for (int i = 0; i < NUM_ACTIONS; ++i) {
        if (in_bounds(static_cast<Direction>(i), empty_idx)) {
            actions.push_back(i);
        }
    }
    return actions;
}

void SlidingPuzzleState::apply_action(int action) {
    // Do nothing if invalid
    if (!in_bounds(static_cast<Direction>(action), empty_idx)) {
        return;
    }
    // Move
    int new_idx = get_new_idx(static_cast<Direction>(action), empty_idx);
    board[empty_idx] = board[new_idx];
    hash ^= zobrist_table.table[new_idx][board[new_idx]];        // Undo previous piece
    hash ^= zobrist_table.table[empty_idx][board[empty_idx]];    // Add new piece
    board[new_idx] = 0;
    hash ^= zobrist_table.table[new_idx][0];      // Undo previous piece
    hash ^= zobrist_table.table[empty_idx][0];    // Add new piece
    empty_idx = new_idx;
}

double SlidingPuzzleState::get_heuristic() const {
    double heuristic = 0;
    for (int i = 0; i < TOTAL_SIZE; ++i) {
        if (board[i] == 0) {
            continue;
        }
        heuristic += manhattan_distance(i, board[i] - 1);
    }
    return heuristic;
}

uint64_t SlidingPuzzleState::get_hash() const {
    return hash;
}

bool SlidingPuzzleState::is_solution() const {
    return board == kSolutionBoard;
}

std::string SlidingPuzzleState::to_str() const {
    std::stringstream ss;
    int idx = 0;
    for (auto const &c : board) {
        ss << kBoardStrMap.at(c) << " ";
        if (++idx % COLS == 0) {
            ss << std::endl;
        }
    }
    return ss.str();
}


//  ------------------------- Member functions -------------------------
SlidingPuzzleState2::SlidingPuzzleState2(const std::string &board_string) {
    // Not safe, doesn't validate board input
    // Example board input: 14,13,15,7,11,12,9,5,6,0,2,1,4,8,10,3
    int idx = -1;
    hash = 0;
    std::vector<std::string> board_str_segment = str_split(board_string, ',');
    assert(board_str_segment.size() == board.size());

    for (auto const &s : board_str_segment) {
        int p = std::stoi(s);
        board[++idx] = p;
        hash ^= zobrist_table.table[idx][p];
        if (p == 0) {
            empty_idx = idx;
        }
    }
}

std::vector<int> SlidingPuzzleState2::child_actions() const {
    // Actions in reference to moving empty tile
    std::vector<int> actions;
    for (int i = 0; i < NUM_ACTIONS; ++i) {
        if (in_bounds(static_cast<Direction>(i), empty_idx)) {
            actions.push_back(i);
        }
    }
    return actions;
}

void SlidingPuzzleState2::apply_action(int action) {
    // Do nothing if invalid
    if (!in_bounds(static_cast<Direction>(action), empty_idx)) {
        return;
    }
    // Move
    int new_idx = get_new_idx(static_cast<Direction>(action), empty_idx);
    board[empty_idx] = board[new_idx];
    hash ^= zobrist_table.table[new_idx][board[new_idx]];        // Undo previous piece
    hash ^= zobrist_table.table[empty_idx][board[empty_idx]];    // Add new piece
    board[new_idx] = 0;
    hash ^= zobrist_table.table[new_idx][0];      // Undo previous piece
    hash ^= zobrist_table.table[empty_idx][0];    // Add new piece
    empty_idx = new_idx;
}

double SlidingPuzzleState2::get_heuristic() const {
    double heuristic = 0;
    for (int i = 0; i < TOTAL_SIZE; ++i) {
        if (board[i] == 0) {
            continue;
        }
        heuristic += manhattan_distance(i, board[i] - 1);
    }
    return heuristic;
}

uint64_t SlidingPuzzleState2::get_hash() const {
    return hash;
}

bool SlidingPuzzleState2::is_solution() const {
    return board == kSolutionBoard;
}

std::string SlidingPuzzleState2::to_str() const {
    std::stringstream ss;
    int idx = 0;
    for (auto const &c : board) {
        ss << kBoardStrMap.at(c) << " ";
        if (++idx % COLS == 0) {
            ss << std::endl;
        }
    }
    return ss.str();
}