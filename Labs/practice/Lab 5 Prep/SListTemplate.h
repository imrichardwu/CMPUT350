#include <cassert>

template <typename T>
class SList {
public:
    // list node containing data payload and pointer to next element
    // (nullptr at end of list)
    struct Node {
        T data;
        Node *succ;
    };

private:
    Node *head;    // pointer to head node
    int n;         // number of nodes

public:
    // constructor: no nodes
    SList();

    // destructor
    ~SList();

    // not implemented yet
    SList(const SList &rhs) = delete;
    SList &operator=(const SList &rhs) = delete;

    // free all nodes and empty list
    void free();

    // add node containing data at front
    void add_head(T data);

    // returns pointer to head node
    Node *get_head() const {
        return head;
    }

    // removes head node
    void remove_head();

    // return number of nodes
    int size() const;
};



// constructor: initialize list, no nodes
template <typename T>
SList<T>::SList()
    : head(nullptr), n(0) {}

// destructor: free all nodes
template <typename T>
SList<T>::~SList() {
    free();
}

// frees all node memory and empties list
template <typename T>
void SList<T>::free() {
    while (head) {
        remove_head();
    }
}

// return number of nodes
template <typename T>
int SList<T>::size() const {
    return n;
}

// add node containing data at front
template <typename T>
void SList<T>::add_head(T data) {
    Node *q = new Node;    // allocate new node
    q->data = data;        // store data in new node
    q->succ = head;        // previous head is q successor
    head = q;              // new node is head
    ++n;                   // one more node
}

// removes head node
template <typename T>
void SList<T>::remove_head() {
    assert(head);            // can't remove head from empty list
    --n;                     // one fewer node
    Node *q = head->succ;    // save pointer to successor
    delete head;             // free head node
    head = q;                // head successor now head
}
