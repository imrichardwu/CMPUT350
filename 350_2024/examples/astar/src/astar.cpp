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

std::pair<bool, std::vector<int>> a_star(std::unique_ptr<BaseState> puzzle) {
    int expanded = 0;
    int generated = -1;
    std::unique_ptr<Node> root_node = std::make_unique<Node>(nullptr, puzzle->clone(), 0, puzzle->get_heuristic(), -1);

    std::vector<std::unique_ptr<Node>> node_buffer;
    std::vector<Node *> open;                                           // Open from algorithm
    std::unordered_set<Node *, NodeHash, NodeCompareEqual> open_set;    // Fast access to modify open items
    std::unordered_set<Node *, NodeHash, NodeCompareEqual> closed;      // Closed from algorithm
    open.push_back(root_node.get());
    open_set.insert(root_node.get());

    Timer timer;
    timer.start();

    while (!open.empty()) {
        // Get minimal f-cost node in open, add to closed
        Node *n = open.front();
        std::pop_heap(open.begin(), open.end(), NodeCompareOrdered());
        open.pop_back();
        open_set.erase(n);
        closed.insert(n);
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

        // Consider all children
        for (const auto &action : n->state->child_actions()) {
            // Generate child state
            std::unique_ptr<Node> child_node = std::make_unique<Node>(n, n->state->clone(), 0, 0, -1);
            child_node->state->apply_action(action);
            child_node->set_values(n, n->g + 1, child_node->state->get_heuristic(), action);

            // Previously expanded node containing child state
            auto closed_iter = closed.find(child_node.get());
            if (closed_iter != closed.end()) {    // Older is better
                if ((*closed_iter)->g <= child_node->g) {
                    continue;
                }
                closed.erase(child_node.get());    // Found better solution
            }

            // Previously added to open node containing child state
            auto open_iter = open_set.find(child_node.get());
            if (open_iter != open_set.end()) {
                if ((*open_iter)->g <= child_node->g) {    // Older is better
                    continue;
                }
                // Lazy remove trick, 
                // set value to minimum, remake heap, and remove
                (*open_iter)->f = -1;
                open_set.erase(child_node.get());
                std::make_heap(open.begin(), open.end());
                std::cout << open.front()->f <<std::endl;
                std::pop_heap(open.begin(), open.end(), NodeCompareOrdered());
                open.pop_back();
            }

            // Ok to add new node to open
            ++generated;
            open.push_back(child_node.get());
            open_set.insert(child_node.get());
            std::push_heap(open.begin(), open.end(), NodeCompareOrdered());
            node_buffer.emplace_back(std::move(child_node));
        }
    }

    std::cout << "No solution found." << std::endl;
    return {false, {}};
}

// /usr/bin/time --verbose ./src/astar sliding_puzzle 7,4,5,12,15,0,6,1,8,14,11,13,10,9,2,3
int main(int argc, char *argv[]) {
    std::vector<std::string> all_args;
    if (argc > 1) {
        all_args.assign(argv + 1, argv + argc);
    } else {
        std::cerr << "Usage: ./astar puzzle_name <arg1> <arg2> ..." << std::endl;
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
    for (auto const & a : result.second) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
}