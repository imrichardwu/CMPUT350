#include <cassert>
#include <iostream>
#include <list>

using namespace std;

// private inheritance: no base class members accessible from outside =>
// reusing implementation

/*

Option 1:

class Queue
  : private std::list<T>
{
public:

  using Base = std::list<T>;  // type duplication
  ...
};
*/

// Option 2: avoids type duplication

template <typename T>
using QBase = std::list<T>;

template <typename T>
class Queue : private QBase<T> {
public:
    using Base = QBase<T>;

    // returns true iff queue is empty
    bool empty() const {
        return Base::empty();
    }

    // push element to end of queue
    void push(const T &x) {
        Base::push_back(x);
    }

    // returns reference to front element (pre-cond: !empty)
    T &front() {
        assert(!empty());
        return Base::front();
    }

    // returns const reference to front element (pre-cond: !empty)
    const T &front() const {
        assert(!empty());
        return Base::front();
    }

    // removes front element (pred-cond: !empty)
    void pop() {
        assert(!empty());
        Base::pop_front();
    }

    // default constr, CC, AO, destructor work because base class takes care of itself
};

int main() {
    Queue<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    {
        const Queue<int> p(q);
        cout << p.front() << endl;    // this is why we need const front version
    }

    while (!q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }

    return 0;
}
