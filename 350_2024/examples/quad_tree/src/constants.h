#ifndef QUAD_TREE_CONSTANTS_H_
#define QUAD_TREE_CONSTANTS_H_

namespace quadtree {

constexpr int DELAY = 50;             // 50ms frame delay
constexpr int WIDTH = 600;            // Width of window
constexpr int HEIGHT = 600;           // Height of window
constexpr int MAX_DEPTH = 8;          // Max depth of the quad tree
constexpr int SPLIT_THRESHOLD = 3;    // How many items can occupy a node before a split is triggered
constexpr int RADIUS = 10;            // Radius of the objects
constexpr double SPEED = 5;           // Speed of the objects
constexpr int CIRC_LINE_SEGS = 16;    // Circle approximation

}    // namespace quadtree

#endif    // QUAD_TREE_CONSTANTS_H_
