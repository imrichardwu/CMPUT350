#include "constants.h"
#include "quad_tree.h"
#include "types.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cassert>
#include <cmath>
#include <deque>
#include <iostream>
#include <random>
#include <vector>

namespace quadtree {

struct World {
    static void add_point(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // Check if we are removing a point if we are clicking inside an existing one
            for (const auto &p : query_points()) {
                if (std::abs(p.x - x) < RADIUS && std::abs(p.y - y) < RADIUS) {
                    quad_tree.remove(p);
                    --point_counter;
                    return;
                }
            }
            std::uniform_real_distribution<double> dist(0, 1);
            double theta = dist(rng);
            double phi = 2 * M_PI * theta;
            auto r = static_cast<float>(dist(rng));
            auto g = static_cast<float>(dist(rng));
            auto b = static_cast<float>(dist(rng));
            quad_tree.add({.x = x, .y = y, .heading_x = cos(phi), .heading_y = sin(phi), .r = r, .g = g, .b = b});
            ++point_counter;
        }
    }

    [[nodiscard]] static auto query_boxes() -> const std::vector<BoundingBox> & {
        quad_tree.query_boxes(boxes);
        return boxes;
    }

    [[nodiscard]] static auto query_points() -> std::vector<Point> & {
        quad_tree.query_points(points);
        return points;
    }

    static void step() {
        if (!is_running || point_counter == 0) {
            return;
        }
        // Remove point from quad_tree, move, then re-add
        for (auto &p : query_points()) {
            quad_tree.remove(p);
            p.move();
            quad_tree.add(p);
        }

        // Check for collisions
        int count = quad_tree.query_intersections(collisions);
        collision_counts.pop_front();
        collision_counts.push_back(count);
        for (auto &collision : collisions) {
            // Remove pair of points, update heading, re-add
            quad_tree.remove(collision.first);
            quad_tree.remove(collision.second);
            std::swap(collision.first.heading_x, collision.second.heading_x);
            std::swap(collision.first.heading_y, collision.second.heading_y);
            quad_tree.add(collision.first);
            quad_tree.add(collision.second);
        }
        print_stats();
    }

    static void print_stats() {
        double avg = 0;
        for (const auto &count : collision_counts) {
            avg += static_cast<double>(count);
        }
        avg /= static_cast<double>(collision_counts.size());
        std::cout << "\033[F" << "\r";
        std::cout << "Points " << point_counter << ", naive = " << point_counter * point_counter
                  << ", quad_tree = " << avg << std::endl;
    }

    static inline QuadTree quad_tree{WIDTH, HEIGHT};
    static inline std::vector<Point> points;
    static inline std::vector<BoundingBox> boxes;
    static inline QuadTree::CollisionVec collisions;
    static inline std::mt19937 rng{0};
    static inline bool is_running = false;
    static inline int point_counter = 0;
    static inline std::deque<int> collision_counts = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

namespace {
void timer_function(int) {
    World::step();
    glutTimerFunc(DELAY, timer_function, 0);
}

void keyPressed(unsigned char key, [[maybe_unused]] int x, [[maybe_unused]] int y) {
    if (key == 'r') {
        World::is_running = true;
    } else if (key == 's') {
        World::is_running = false;
    }
}

void OnDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    //  (0,0) top left, (width,height) bottom right
    glutPostRedisplay();

    // Render points
    for (const auto &p : World::query_points()) {
        //  approximate circle
        glColor3f(p.r, p.g, p.b);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= CIRC_LINE_SEGS; ++j) {
            const double angle((j % CIRC_LINE_SEGS) * M_PI * 2 / CIRC_LINE_SEGS);
            glVertex2f(static_cast<float>(p.x + cos(angle) * RADIUS), static_cast<float>(p.y + sin(angle) * RADIUS));
        }
        glEnd();
    }
    glColor3f(1, 1, 1);

    // Render bounding boxes
    for (const auto &bbox : World::query_boxes()) {
        glBegin(GL_LINE_LOOP);
        glVertex2i(bbox.left(), bbox.top());
        glVertex2i(bbox.right(), bbox.top());
        glVertex2i(bbox.right(), bbox.bottom());
        glVertex2i(bbox.left(), bbox.bottom());
        glEnd();
    }

    glPopAttrib();
    glutSwapBuffers();
}
}    // namespace

}    // namespace quadtree

int main(int argc, char *argv[]) {
    std::cout << "Instructions:" << std::endl;
    std::cout << "\t- Click to add points" << std::endl;
    std::cout << "\t- Click on a point to remove it" << std::endl;
    std::cout << "\t- Press 'r' to run the simulation" << std::endl;
    std::cout << "\t- Press 's' to stop the simulation" << std::endl;
    std::cout << std::endl;

    glutInit(&argc, argv);
    glutInitWindowSize(quadtree::WIDTH, quadtree::HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutCreateWindow("CMPUT 350 - QuadTree");
    glutDisplayFunc(quadtree::OnDisplay);
    glutMouseFunc(&quadtree::World::add_point);
    glutKeyboardFunc(quadtree::keyPressed);
    glutTimerFunc(1, quadtree::timer_function, 0);
    glutMainLoop();
    return 0;
}
