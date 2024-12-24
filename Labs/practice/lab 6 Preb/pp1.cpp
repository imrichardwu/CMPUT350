#include <iostream>
#include <list>
#include <cassert>

template <typename T>
class Queue: private std::list<T> {
public:
    using Base = std::list<T>;

    bool empty() const {
        return Base::empty();
    }

    // returns reference to front element
    T& front() {
        assert(!empty());
        return Base::front();
    }

    const T& front() const {
        assert(!empty());
        return Base::front();
    }

    // removes front element 
    void pop() {
        assert(!empty());
        Base::pop_front();
    }

    void push(const T&& value) {
        Base::push_back(value);
    }
};

int main() {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    q.pop();

    std::cout << q.front() << std::endl;

}