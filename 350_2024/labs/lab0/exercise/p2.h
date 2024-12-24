#pragma once

struct Node {
    int data;
    struct Node *next;
};

// return a list containing nodes with data n-1,n-2,...2,1,0
// check for any preconditions
struct Node *create(int n);

// deallocate list by freeing all nodes in list starting with node
void deallocate(struct Node *node);

// print list
void print(struct Node *node);

// Test code
// check whether above functions work by creating a list and printing it
// also use valgrind for detecting memory leaks
void test();
