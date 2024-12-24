#include "Triang.h"

#include <GL/glut.h>

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>

using namespace std;

int delay = 0;     // frame delay in ms
int mode = 0;      // 0: random, 1: on diagonal
int fspeed = 0;    // 0: slow, 1: fast
size_t num = 0;    // total number of points

int state = 0;    // drawing state (see switch)

// triangulation
std::unique_ptr<Triangulation> triang;

// the Mersenne Twister with a popular choice of parameters
typedef std::mt19937 RNG;

RNG rng;
std::uniform_real_distribution<double> distr(0, 1);

double rnd01(RNG &rng) {
    return distr(rng);
}

void OnDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, static_cast<double>(triang->get_width()), static_cast<double>(triang->get_height()), 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();

    // render triangles

    RNG col_rng;
    col_rng.seed(1);

    for (const Triangle &tr : triang->get_triangles()) {
        glColor3f(
            static_cast<float>(rnd01(col_rng) * 0.5 + 0.5),
            static_cast<float>(rnd01(col_rng) * 0.5 + 0.5),
            static_cast<float>(rnd01(col_rng) * 0.5 + 0.5)
        );
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < 3; ++i) {
            // invert y
            // in OpenGL (0,0) is top left, (width,height) bottom right
            glVertex2f(static_cast<float>(tr.points[i].x), static_cast<float>(triang->get_height() - tr.points[i].y));
        }
        glEnd();
    }

    glPopAttrib();
    glutSwapBuffers();
}

void timer_function(int) {
    cout << "." << flush;

    int D = 20;
    int x = static_cast<int>(static_cast<double>(triang->get_width() - 2 * D) * rnd01(rng) + D);
    int y = static_cast<int>(static_cast<double>(triang->get_height() - 2 * D) * rnd01(rng) + D);

    if (mode) {
        y = static_cast<int>(
            (static_cast<double>(x) / static_cast<double>(triang->get_width()))
                * static_cast<double>(triang->get_height())
            + 0.5
        );
        // cout << "x " << x << " y " << y << endl;
        assert(y > 0 && y < static_cast<int>(triang->get_height()));
    }

    int current_delay = delay;

    switch (state) {
    case 0:    // add point

        if (triang->get_triangles().size() < num) {
            triang->add_point(Point(x, y));
        } else {
            state = 1;
        }
        break;

    case 1:    // wait

        current_delay = 1000;    // 1 sec
        state = 2;
        break;

    case 2:    // flip edge(s)

    {
        size_t c = triang->Delaunay_pass(fspeed == 0);
        cout << "Delaunay: " << c << " flip(s)" << endl;
        if (!c) {
            current_delay = 3000;    // 3 secs
            state = 3;
        }
        break;
    }

    case 3:    // reset
        triang.reset(new Triangulation(triang->get_width(), triang->get_height()));
        state = 0;
        break;
    }

    glutTimerFunc(static_cast<unsigned int>(current_delay), timer_function, 0);
}

int main(int argc, char *argv[]) {
    {
        // triangle test
        Point A(0, 0);
        Point B(5, 5);
        Point C(5, 0);

        assert(orientation_test(A, B, C) < 0);

        Triangle T;
        T.points[0] = A;
        T.points[1] = B;
        T.points[2] = C;

        assert(T.inside_test(Point(0, 0)) == 0);
        assert(T.inside_test(Point(5, 5)) == 0);
        assert(T.inside_test(Point(5, 0)) == 0);
        assert(T.inside_test(Point(1, 1)) == 0);
        assert(T.inside_test(Point(2, 1)) < 0);
        assert(T.inside_test(Point(-1, 1)) > 0);

        Point X(427, 83);
        Point Y(464, 58);
        Point Z(430, 64);
        assert(orientation_test(X, Y, Z) < 0);
    }

    {
        // circle test
        Point A(0, 10);
        Point B(10, 0);
        Point C(0, -10);
        Point D(-10, 0);    // on circle
        Point E(0, 0);      // in circle
        Point F(20, 20);    // outside circle

        assert(orientation_test(A, B, C) < 0);

        assert(circle_test(A, B, C, D) == 0LL);
        assert(circle_test(A, B, C, E) < 0LL);
        assert(circle_test(A, B, C, F) > 0LL);
    }

    if (argc != 1 && argc != 8) {
        cerr << "usage: " << argv[0] << " width height delay seed mode fspeed num" << endl
             << "       delay in ms, seed=0: use time, mode 0/1, flip speed 0/1, number of points" << endl;
        exit(1);
    }

    int width = 800;
    int height = 800;

    delay = 100;     // frame delay in ms
    int seed = 0;    // for rng, 0: use time
    mode = 0;
    fspeed = 0;
    num = 100;

    if (argc == 8) {
        // todo : error handling
        int i = 1;
        width = atoi(argv[i++]);
        height = atoi(argv[i++]);
        delay = atoi(argv[i++]);
        seed = atoi(argv[i++]);
        mode = atoi(argv[i++]);
        fspeed = atoi(argv[i++]);
        num = static_cast<unsigned long int>(atoi(argv[i++]));
    }

    if (seed == 0) {
        // rng seed dependent on wallclock time
        const auto millis =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();

        seed = static_cast<int>(millis);
    }

    rng.seed(static_cast<unsigned long>(seed));

    cout << "width: " << width << endl;
    cout << "height: " << height << endl;
    cout << "delay: " << delay << endl;
    cout << "seed: " << seed << endl;
    cout << "mode: " << mode << endl;
    cout << "fspeed: " << fspeed << endl;
    cout << "num: " << num << endl;

    triang.reset(new Triangulation(width, height));

    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    // glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow("CMPUT 350 - Triangulations");
    glutDisplayFunc(OnDisplay);
    glutTimerFunc(1, timer_function, 0);
    glutMainLoop();
    return 0;
}
