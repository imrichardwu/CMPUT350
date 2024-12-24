#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "base_game.h"
#include "sliding_puzzle.h"

// ---------------------- Node ----------------------
struct Node {
    Node(Node *parent, std::unique_ptr<BaseState> state, float g, float h, int action)
        : parent(parent), state(std::move(state)), g(g), h(g), f(g + h), action(action) {}

    void set_values(Node *parent_, float g_, float h_, int action_) {
        parent = parent_;
        g = g_;
        h = h_;
        f = g + h;
        action = action_;
    }

    Node *parent;
    std::unique_ptr<BaseState> state;
    float g;
    float h;
    float f;
    int action;
};

// Compare function for nodes (f-cost, then g-cost on tiebreaks)
struct NodeCompareOrdered {
    bool operator()(const Node *left, const Node *right) const {
        return left->f > right->f || (left->f == right->f && left->g > right->g);
    }
};

// Node hash (state hash)
struct NodeHash {
    std::size_t operator()(const Node *node) const {
        return node->state->get_hash();
    }
};

// Node equality compare (underlying state) for closed
struct NodeCompareEqual {
    bool operator()(const Node *left, const Node *right) const {
        return *(left->state) == *(right->state);
    }
};
// ---------------------- Node ----------------------

static const int ALLOCATE_INCREMENT = 66884915;
void allocate_nodes(std::vector<std::unique_ptr<Node>> &node_list, BaseState *state) {
    const int num_to_add = ALLOCATE_INCREMENT;
    node_list.reserve(num_to_add + node_list.size());

    std::generate_n(std::back_inserter(node_list), ALLOCATE_INCREMENT, [&]() { 
        return std::make_unique<Node>(nullptr, state->clone(), 0, state->get_heuristic(), -1); 
    });

    // for (int i = 0; i < num_to_add; ++i) {
    //     node_list.emplace_back(std::make_unique<Node>(nullptr, state->clone(), 0, state->get_heuristic(), -1));
    // }
}

std::pair<bool, std::vector<int>> a_star(std::unique_ptr<BaseState> puzzle) {
    int expanded = 0;
    int generated = -1;
    std::unique_ptr<Node> root_node = std::make_unique<Node>(nullptr, puzzle->clone(), 0, puzzle->get_heuristic(), -1);

    std::vector<std::unique_ptr<Node>> node_buffer;
    std::priority_queue<Node *, std::vector<Node *>, NodeCompareOrdered> open;    // Open from algorithm
    std::unordered_set<Node *, NodeHash, NodeCompareEqual> closed;      // Closed from algorithm
    open.push(root_node.get());


    allocate_nodes(node_buffer, puzzle.get());

    Timer timer;
    timer.start();

    // allocate_nodes(node_buffer, puzzle.get());

    while (!open.empty()) {
        // Get minimal f-cost node in open, add to closed
        Node *n = open.top();
        open.pop();
        ++expanded;

        // Check for goal
        if (n->state->is_solution()) {
            double duration = timer.get_duration();
            // Reconstruct solution path
            Node *current = n;
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
        for (const auto &action : n->state->child_actions()) {
            // Generate child state
            if (generated + 1 >= (int)node_buffer.size()) {    // Buffer empty, reallocate
                allocate_nodes(node_buffer, puzzle.get());
            }
            Node *child_node = node_buffer[++generated].get();
            child_node->state->assign(n->state.get());
            child_node->state->apply_action(action);
            child_node->set_values(n, n->g + 1, child_node->state->get_heuristic(), action);

            if (closed.find(child_node) == closed.end()) {
                open.push(child_node);
            }
        }
    }

    std::cout << "No solution found." << std::endl;
    return {false, {}};
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
    if (all_args[0] == SlidingPuzzleState::name) {
        puzzle = std::make_unique<SlidingPuzzleState>(all_args[1]);
    } else {
        std::cerr << "Unknown puzzle name." << std::endl;
        exit(1);
    }

    std::cout << *puzzle << std::endl;

    std::pair<bool, std::vector<int>> result = a_star(puzzle->clone());
    for (auto const &a : result.second) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
}