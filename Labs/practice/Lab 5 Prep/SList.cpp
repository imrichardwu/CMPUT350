// SList implementation

#include "SList.h"

#include <cassert>

// constructor: initialize list, no nodes
SList::SList()
    : head(nullptr), n(0) {}

// destructor: free all nodes
SList::~SList() {
    free();
}

// frees all node memory and empties list
void SList::free() {
    while (head) {
        remove_head();
    }
}

// return number of nodes
int SList::size() const {
    return n;
}

// add node containing data at front
void SList::add_head(int data) {
    Node *q = new Node;    // allocate new node
    q->data = data;        // store data in new node
    q->succ = head;        // previous head is q successor
    head = q;              // new node is head
    ++n;                   // one more node
}

// removes head node
void SList::remove_head() {
    assert(head);            // can't remove head from empty list
    --n;                     // one fewer node
    Node *q = head->succ;    // save pointer to successor
    delete head;             // free head node
    head = q;                // head successor now head
}
