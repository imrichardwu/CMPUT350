#include <iostream>
#include <cassert>

class Expr {
public:
    Expr() {};
    virtual ~Expr() {};
    // retyrb value of expression
    virtual int value() const = 0;
    // we are underpaid and therfore refuse to impplement the CC and AD
    Expr(const Expr&) = delete;
    Expr & operator=(const Expr&) = delete;
};

class ConstExpr: public Expr {
public:
    ConstExpr(int v): val(v) {}
    int value() const override {
        return val;
    }
private:
    int val;
};

class ParenExpr: public Expr {
public:
    ParenExpr(Expr *sub): subExpr(sub) {}
    ~ParenExpr() override {
        delete subExpr;
    }

    int value() const override {
        return subExpr->value();
    }
   
private:
    Expr *subExpr;
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(Expr * left, Expr * right): left(left), right(right) {}

    ~BinaryExpr() override {
        delete left;
        delete right;
    }

protected:
    Expr *left;
    Expr *right;
};

class PlusExpr: public BinaryExpr {
public: 
    PlusExpr(Expr *left, Expr *right): BinaryExpr(left, right) {}

    int value() const override {
        return left->value() + right->value();
    }
};

class MinusExpr: public BinaryExpr {
public: 
    MinusExpr(Expr *left, Expr *right): BinaryExpr(left, right) {}

    int value() const override {
        return left->value() - right->value();
    }
};

class TimesExpr: public BinaryExpr {
public: 
    TimesExpr(Expr *left, Expr *right): BinaryExpr(left, right) {}

    int value() const override {
        return left->value() * right->value();
    }
};

class DivExpr: public BinaryExpr {
public: 
    DivExpr(Expr *left, Expr *right): BinaryExpr(left, right) {}

    int value() const override {
        assert(right->value() != 0);
        return left->value() / right->value();
    }
};

int main() {
    Expr *A = new ConstExpr(5);
    Expr *B = new ConstExpr(3);
    Expr *C = new MinusExpr(A, B);
    Expr *D = new ParenExpr(C);
    Expr *E = new ConstExpr(30);
    Expr *F = new TimesExpr(D, E);
    Expr *G = new ConstExpr(10);
    Expr *H = new PlusExpr(F, G);
    delete H;
    std::cout << H->value() << std::endl;
}