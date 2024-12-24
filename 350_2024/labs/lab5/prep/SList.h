#pragma once

class SList {
public:
    // list node containing data payload and pointer to next element
    // (nullptr at end of list)
    struct Node {
        int data;
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
    void add_head(int data);

    // returns pointer to head node
    Node *get_head() const {
        return head;
    }

    // removes head node
    void remove_head();

    // return number of nodes
    int size() const;
};
