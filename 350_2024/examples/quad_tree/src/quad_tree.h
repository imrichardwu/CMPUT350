#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_

#include "constants.h"
#include "types.h"

#include <array>
#include <cassert>
#include <memory>
#include <vector>

namespace quadtree {

struct QuadNode {
    QuadNode(BoundingBox bbox)
        : bbox(bbox) {}

    [[nodiscard]] auto is_leaf() const -> bool {
        return !children[0];
    }

    [[nodiscard]] auto size() const -> int {
        return static_cast<int>(items.size());
    }

    void insert(Point p) {
        items.push_back(std::move(p));
    }

    std::array<std::unique_ptr<QuadNode>, 4> children;
    BoundingBox bbox;
    std::vector<Point> items;
};

class QuadTree {
public:
    using CollisionVec = std::vector<std::pair<Point, Point>>;

    QuadTree(int width, int height, int split_threshold = SPLIT_THRESHOLD, int max_depth = MAX_DEPTH)
        : split_threshold(split_threshold),
          max_depth(max_depth),
          root_node({.mid_x = width / 2, .mid_y = height / 2, .half_width = width / 2, .half_height = height / 2}) {}

    void add(const Point &p) {
        add(&root_node, p, 0);
    }

    void remove(const Point &p) {
        remove(&root_node, p);
    }

    void query_boxes(std::vector<BoundingBox> &boxes) const {
        boxes.clear();
        query_boxes(&root_node, boxes);
    }

    void query_points(std::vector<Point> &points) const {
        points.clear();
        query_points(&root_node, points);
    }

    auto query_intersections(CollisionVec &collisions) const -> int {
        collisions.clear();
        int counter = 0;
        query_intersections(&root_node, collisions, counter);
        return counter;
    }

private:
    // NOLINTNEXTLINE(*no-recursion)
    void add(QuadNode *node, const Point &p, int curr_depth) {
        // If a leaf node, try to insert
        if (node->is_leaf()) {
            // If we are at max depth OR under size threshold then we can insert into this leaf
            if (curr_depth >= max_depth || node->size() < split_threshold) {
                node->insert(p);
                return;
            }
            // Otherwise, node is full and we need to split current node into children
            // This will move all current items into the respective new leaf nodes
            split(node);
        }

        // Not a leaf (before or was just split)
        // Insert to quadrant (recursive) if it fits, or current node
        const auto quadrant = get_quadrant(p, node->bbox);
        if (quadrant == Quadrant::NONE) {
            node->items.push_back(std::move(p));
        } else {
            // NOLINTNEXTLINE(*-constant-array-index)
            add(node->children[static_cast<std::size_t>(quadrant)].get(), p, ++curr_depth);
        }
    }

    void split(QuadNode *node) {
        assert(node && node->is_leaf());
        // Create child nodes
        for (auto &child : node->children) {
            const auto idx = &child - &node->children[0];
            child = std::make_unique<QuadNode>(node->bbox.get_quadrant(static_cast<Quadrant>(idx)));
        }
        // Move values in the old parent into child quadrants if it can fit entirely
        decltype(node->items) new_items;
        for (const auto &item : node->items) {
            const auto quadrant = get_quadrant(item, node->bbox);
            if (quadrant == Quadrant::NONE) {
                new_items.push_back(std::move(item));
            } else {
                // NOLINTNEXTLINE(*-constant-array-index)
                node->children[static_cast<std::size_t>(quadrant)]->items.push_back(std::move(item));
            }
        }
        node->items = std::move(new_items);
    }

    // NOLINTNEXTLINE(*no-recursion)
    bool remove(QuadNode *node, const Point &p) {
        // If node is a leaf, remove from the stored items
        if (node->is_leaf()) {
            remove_item(node, p);
            return true;
        }

        // Not a leaf node
        // Either value exists in a child node OR it didn't fit and is in the curent node
        const auto quadrant = get_quadrant(p, node->bbox);
        if (quadrant == Quadrant::NONE) {
            remove_item(node, p);
        } else {
            // NOLINTNEXTLINE(*-constant-array-index)
            if (remove(node->children[static_cast<std::size_t>(quadrant)].get(), p)) {
                return merge(node);
            }
        }
        return false;
    }

    bool merge(QuadNode *node) {
        assert(node && !node->is_leaf());
        int values_to_merge = static_cast<int>(node->items.size());
        for (const auto &child : node->children) {
            // If a child is not a leaf, we cannot merge with a total collapse
            if (!child->is_leaf()) {
                return false;
            }
            values_to_merge += child->size();
        }
        // If total values under threshold, merge
        if (values_to_merge <= split_threshold) {
            node->items.reserve(static_cast<std::size_t>(values_to_merge));
            // Add child items and reset pointer
            for (auto &child : node->children) {
                node->items.insert(node->items.end(), child->items.begin(), child->items.end());
                child.reset();
            }
            return true;
        }
        return false;
    }

    void remove_item(QuadNode *node, const Point &p) {
        for (auto &item : node->items) {
            if (item == p) {
                const auto idx = &item - &node->items[0];
                std::swap(node->items[static_cast<std::size_t>(idx)], node->items.back());
                node->items.pop_back();
                return;
            }
        }
    }

    [[nodiscard]] auto get_quadrant(const Point &p, const BoundingBox &outer_bbox) const -> Quadrant {
        const BoundingBox point_bbox = point_to_bbox(p);
        const bool is_left = point_bbox.right() <= outer_bbox.mid_x;
        const bool is_right = point_bbox.left() >= outer_bbox.mid_x;
        const bool is_above = point_bbox.bottom() <= outer_bbox.mid_y;
        const bool is_below = point_bbox.top() >= outer_bbox.mid_y;
        if (is_left && is_above) {
            return Quadrant::NORTH_WEST;
        } else if (is_right && is_above) {
            return Quadrant::NORTH_EAST;
        } else if (is_left && is_below) {
            return Quadrant::SOUTH_WEST;
        } else if (is_right && is_below) {
            return Quadrant::SOUTH_EAST;
        }
        return Quadrant::NONE;
    }

    // NOLINTNEXTLINE(*no-recursion)
    void query_boxes(const QuadNode *node, std::vector<BoundingBox> &boxes) const {
        if (!node) {
            return;
        }
        boxes.push_back(node->bbox);
        for (const auto &child : node->children) {
            query_boxes(child.get(), boxes);
        }
    }

    // NOLINTNEXTLINE(*no-recursion)
    void query_points(const QuadNode *node, std::vector<Point> &points) const {
        if (!node) {
            return;
        }
        for (const auto &item : node->items) {
            points.push_back(item);
        }
        for (const auto &child : node->children) {
            query_points(child.get(), points);
        }
    }

    // NOLINTNEXTLINE(*no-recursion)
    void query_intersections(const QuadNode *node, CollisionVec &collisions, int &counter) const {
        assert(node);
        // Query all points stored in this node
        for (std::size_t i = 0; i < node->items.size(); ++i) {
            for (std::size_t j = i + 1; j < node->items.size(); ++j) {
                ++counter;
                if (do_boxes_overlap(point_to_bbox(node->items[i]), point_to_bbox(node->items[j]))) {
                    collisions.emplace_back(node->items[i], node->items[j]);
                }
            }
        }
        if (!node->is_leaf()) {
            // Query items in this node with items in child nodes
            for (const auto &child : node->children) {
                for (const auto &p : node->items) {
                    query_intersections(child.get(), p, collisions, counter);
                }
            }

            // Query child interior collisions
            for (const auto &child : node->children) {
                query_intersections(child.get(), collisions, counter);
            }
        }
    }

    // NOLINTNEXTLINE(*no-recursion)
    void query_intersections(const QuadNode *node, const Point &point, CollisionVec &collisions, int &counter) const {
        // Check collision for items in this node with given point
        const auto point_bbox = point_to_bbox(point);
        for (const auto &p : node->items) {
            ++counter;
            if (do_boxes_overlap(point_to_bbox(p), point_bbox)) {
                collisions.emplace_back(p, point);
            }
        }

        // Check point with all children of this node
        if (!node->is_leaf()) {
            for (const auto &child : node->children) {
                query_intersections(child.get(), point, collisions, counter);
            }
        }
    }

    int split_threshold;
    int max_depth;
    QuadNode root_node;
};

}    // namespace quadtree

#endif    // QUAD_TREE_H_
