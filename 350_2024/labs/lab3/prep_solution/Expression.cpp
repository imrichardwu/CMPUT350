#include <cassert>
#include <iostream>

using namespace std;

// expression base-class

class Expr {
public:
    Expr() = default;    // Ask the compiler to provide a default constructor
                         // i.e., Expr() {}
    virtual ~Expr() {}

    // return value of expression
    virtual int value() const = 0;

    // we are underpaid and therefore refuse to implement the CC and AO
    Expr(const Expr &) = delete;
    Expr &operator=(const Expr &) = delete;
};

//============================================================================

// unary expression base-class

class UnaryExpr : public Expr {
public:
    UnaryExpr(Expr *s)
        : succ(s) {}

    ~UnaryExpr() {
        delete succ;
    }

protected:
    Expr *succ;    // owner
};

//============================================================================

// binary expression base-class

class BinaryExpr : public Expr {
public:
    BinaryExpr(Expr *s1, Expr *s2)
        : succ1(s1), succ2(s2) {}

    ~BinaryExpr() {
        delete succ1;
        delete succ2;
    }

protected:
    Expr *succ1, *succ2;    // owner
};

//============================================================================

class ConstExpr : public Expr {
public:
    ConstExpr(int v)
        : val(v) {}

    int value() const override {
        return val;
    }

private:
    int val;
};

//============================================================================

class ParenExpr : public UnaryExpr {
public:
    ParenExpr(Expr *s)
        : UnaryExpr(s) {}

    int value() const override {
        return succ->value();
    }
};

//============================================================================

class PlusExpr : public BinaryExpr {
public:
    PlusExpr(Expr *s1, Expr *s2)
        : BinaryExpr(s1, s2) {}

    int value() const override {
        return succ1->value() + succ2->value();
    }
};

//============================================================================

class MinusExpr : public BinaryExpr {
public:
    MinusExpr(Expr *s1, Expr *s2)
        : BinaryExpr(s1, s2) {}

    int value() const override {
        return succ1->value() - succ2->value();
    }
};

//============================================================================

class TimesExpr : public BinaryExpr {
public:
    TimesExpr(Expr *s1, Expr *s2)
        : BinaryExpr(s1, s2) {}

    int value() const override {
        return succ1->value() * succ2->value();
    }
};

//============================================================================

class DivExpr : public BinaryExpr {
public:
    DivExpr(Expr *s1, Expr *s2)
        : BinaryExpr(s1, s2) {}

    int value() const override {
        int r = succ2->value();    // avoid double evaluation
        assert(r);
        return succ1->value() / r;
    }
};

//============================================================================

int main() {
    // evaluate expression (10+(30*((5-3)))) inside out

    ConstExpr *A = new ConstExpr(5);       // 5
    ConstExpr *B = new ConstExpr(3);       // 3
    MinusExpr *C = new MinusExpr(A, B);    // (5-3)
    ParenExpr *D = new ParenExpr(C);       // ((5-3))
    ConstExpr *E = new ConstExpr(30);      // 30
    TimesExpr *F = new TimesExpr(E, D);    // (30*((5-3)))
    ConstExpr *G = new ConstExpr(10);      // 10
    PlusExpr *H = new PlusExpr(G, F);      // (10+(30*((5-3))))

    cout << H->value() << endl;    // 70

    delete H;    // free all expression objects to make valgrind happy
    return 0;
}
