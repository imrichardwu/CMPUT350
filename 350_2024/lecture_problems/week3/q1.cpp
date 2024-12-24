/**
 * Question:
 * Draw the layout of X and Y objects in memory and
 * indicate how much memory each component occupies.
 */

class X {
public:
    int a;
    void f() {}

private:
    int b;
    void g() {}
};

class Y : public X {
public:
    int c;
    void u() {}

private:
    int d;
    void v() {}
};

// Solution
/**
 *
 *      X                        Y
 *  +-----------+           +-----------+
 *  |  a        | 4	        |  a        | 4
 *  +-----------+ 	        +-----------+
 *  |  b        | 4	        |  b        | 4
 *  +-----------+  	        +-----------+
 *                          |  c        | 4    sizeof(Y) = 16
 *   sizeof(X) = 8          +-----------+
 *                          |  d        | 4
 *                          +-----------+
 */
