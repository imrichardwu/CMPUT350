#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "base_game.h"
#include "sliding_puzzle.h"

// ---------------------- Node ----------------------
template <typename T>
struct Node {
    Node(Node *parent, const T &state, float g, float h, int action)
        : parent(parent), state(state), g(g), h(g), f(g + h), action(action) {}

    void set_values(Node *parent_, float g_, float h_, int action_) {
        parent = parent_;
        g = g_;
        h = h_;
        f = g + h;
        action = action_;
    }

    Node *parent;
    T state;
    float g;
    float h;
    float f;
    int action;
};

// Compare function for nodes (f-cost, then g-cost on tiebreaks)
template <typename T>
struct NodeCompareOrdered {
    bool operator()(const Node<T> *left, const Node<T> *right) const {
        return left->f > right->f || (left->f == right->f && left->g > right->g);
    }
};

// Node hash (state hash)
template <typename T>
struct NodeHash {
    std::size_t operator()(const Node<T> *node) const {
        return node->state.get_hash();
    }
};

// Node equality compare (underlying state) for closed
template <typename T>
struct NodeCompareEqual {
    bool operator()(const Node<T> *left, const Node<T> *right) const {
        return left->state == right->state;
    }
};
// ---------------------- Node ----------------------

static const int ALLOCATE_INCREMENT = 70000000;
template <typename T>
void allocate_nodes(std::vector<Node<T>> &node_list, const T &state) {
    const int num_to_add = ALLOCATE_INCREMENT;
    node_list.reserve(num_to_add + node_list.size());
    Node<T> n(nullptr, state, 0, state.get_heuristic(), -1);
    std::fill_n(std::back_inserter(node_list), num_to_add, n);

    // for (int i = 0; i < num_to_add; ++i) {
    //     node_list.emplace_back(nullptr, state, 0, state.get_heuristic(), -1);
    // }
}


template <typename T>
std::pair<bool, std::vector<int>> a_star(const T &puzzle) {
    int expanded = 0;
    int generated = -1;
    Node<T> root_node = Node<T>(nullptr, puzzle, 0, puzzle.get_heuristic(), -1);

    std::vector<Node<T>> node_buffer;
    std::priority_queue<Node<T> *, std::vector<Node<T> *>, NodeCompareOrdered<T>> open;    // Open from algorithm
    std::unordered_set<Node<T> *, NodeHash<T>, NodeCompareEqual<T>> closed;      // Closed from algorithm
    open.push(&root_node);

    allocate_nodes(node_buffer, puzzle);

    Timer timer;
    timer.start();

    while (!open.empty()) {
        // Get minimal f-cost node in open, add to closed
        Node<T> *n = open.top();
        open.pop();
        ++expanded;

        // Check for goal
        if (n->state.is_solution()) {
            double duration = timer.get_duration();
            // Reconstruct solution path
            Node<T> *current = n;
            std::vector<int> solution_path;
            while (current->parent) {
                solution_path.insert(solution_path.begin(), current->action);
                current = current->parent;
            }
            std::cout << "Solution Found. expanded: " << expanded << ", nodes stored: " << generated
                      << ", depth: " << solution_path.size() << ", CPU time (s): " << duration << std::endl;
            std::cout << std::flush;
            return {true, solution_path};
        }

        // Consistent heuristic
        if (closed.find(n) != closed.end()) {
            continue;
        }

        closed.insert(n);

        // Consider all children
        for (const auto &action : n->state.child_actions()) {
            // Generate child state
            if (generated + 1 >= (int)node_buffer.size()) {    // Buffer empty, reallocate
                allocate_nodes(node_buffer, puzzle);
            }
            Node<T> *child_node = &node_buffer[++generated];
            child_node->state = n->state;
            child_node->state.apply_action(action);
            child_node->set_values(n, n->g + 1, child_node->state.get_heuristic(), action);

            if (closed.find(child_node) == closed.end()) {
                open.push(child_node);
            }
        }
    }

    std::cout << "No solution found." << std::endl;
    return {false, {}};
}

template <typename T>
void templated_main(const std::string &input_str) {
    T puzzle(input_str);

    std::cout << puzzle.to_str() << std::endl;

    std::pair<bool, std::vector<int>> result = a_star(puzzle);
    for (auto const &a : result.second) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> all_args;
    if (argc > 1) {
        all_args.assign(argv + 1, argv + argc);
    } else {
        std::cerr << "Usage: ./astar_fast puzzle_name <arg1> <arg2> ..." << std::endl;
        exit(1);
    }

    std::unique_ptr<BaseState> puzzle;
    if (all_args[0] == SlidingPuzzleState2::name) {
        templated_main<SlidingPuzzleState2>(all_args[1]);
    } else {
        std::cerr << "Unknown puzzle name." << std::endl;
        exit(1);
    }
}