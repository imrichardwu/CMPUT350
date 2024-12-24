#ifndef SLIST_TEMPLATE_H_
#define SLIST_TEMPLATE_H_

#include <cassert>

template <typename T>
class SList {
public:
    // list node containing data payload and pointer to next element
    // (nullptr at end of list)
    struct Node {
        Node(const T &data_, Node *succ_)
            : data(data_), succ(succ_) {}

        T data;
        Node *succ;
    };

private:
    Node *head;    // pointer to head node
    int n;         // number of nodes

public:
    // constructor: initialize list, no nodes
    SList()
        : head(nullptr), n(0) {}

    // destructor: free all nodes
    ~SList() {
        free();
    }

    // not implemented yet
    SList(const SList &rhs) = delete;
    SList &operator=(const SList &rhs) = delete;

    // frees all node memory and empties list
    void free() {
        while (head) {
            remove_head();
        }
    }

    // add node containing data at front
    void add_head(const T &data) {
        Node *q = new Node(data, head);    // allocate and initialize new node
        head = q;                          // new node is head
        ++n;                               // one more node
    }

    // returns pointer to head node
    Node *get_head() const {
        return head;
    }

    // removes head node
    void remove_head() {
        assert(head);            // can't remove head from empty list
        --n;                     // one fewer node
        Node *q = head->succ;    // save pointer to successor
        delete head;             // free head node
        head = q;                // head successor now head
    }

    // return number of nodes
    int size() const {
        return n;
    }
};

#endif    // SLIST_TEMPLATE_H_
