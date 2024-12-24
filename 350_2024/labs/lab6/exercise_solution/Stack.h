#ifndef STACK_H_
#define STACK_H_

#include <cassert>
#include <iostream>
#include <vector>

//  private inheritance: "implemented in terms of"

template <typename T>
class Stack : private std::vector<T> {
public:
    using Base = std::vector<T>;

    //  returns true iff stack contains no element
    bool empty() const {
        return Base::empty();
    }

    //  return reference to top element (pre-cond: !empty)
    T &top() {
        assert(!empty());
        return Base::back();
    }

    //  push element
    void push(const T &x) {
        Base::push_back(x);
    }

    //  remove top element from stack (pre-cond: !empty)
    void pop() {
        assert(!empty());
        Base::pop_back();
    }

    //  remove all elements
    void clear() {
        Base::clear();
    }

    //  default constructor, CC, AO, and destructor are sufficient
};

//  print stack
template <typename T>
std::ostream &operator<<(std::ostream &os, const Stack<T> &s) {
    auto t = s;

    while (!t.empty()) {
        os << t.top() << ' ';
        t.pop();
    }

    return os;
}

//  reverse stack
template <typename T>
void reverse_stack(Stack<T> &s) {
    auto t = s;

    s.clear();

    while (!t.empty()) {
        s.push(t.top());
        t.pop();
    }
}

#endif    // STACK_H_
