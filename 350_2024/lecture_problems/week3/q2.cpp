/**
 * Question:
 * Mark each code line with "yes" if its legal,
 * and "no" if its illegal. (assume all previous lines are legal)
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

int main() {
    // X *px = new Y;
    {
        // Yes: base class pointer pointing to derived class OK
    }
    // Y *py = new X;
    {
        // no:  derived class pointer pointing to base class (missing data)
    }
    // py->f();
    {
        // yes: f is inherited from X and public
    }
    // px->f();
    {
        // yes: f is public in X
    }
    // Y y = *px;
    {
        // no:  X can't be assigned to Y (missing data)
    }
    // y.d = 0;
    {
        // no:  d is private in y
    }
    // py->a = 0;
    {
        // yes: } is inherited from X and public
    }
    // py->v();
    {
        // no: v is private in Y
    }
}
