// problem 2, C++ Code

#include <iostream>

// singly-linked list node
struct Node {
    int data;
    Node *next;
};

// return a list containing nodes with data n-1,n-2,...2,1,0
Node *create(int n) {
    if (n <= 0) {
        return 0;
    }

    Node *p = new Node;
    p->data = n - 1;
    p->next = create(n - 1);
    return p;
}

// deallocate list by freeing all nodes in list starting with node
// [ later we will see how to move this code to the Node destructor ]
void deallocate(Node *node) {
    if (node) {
        deallocate(node->next);    // free subsequent nodes
        delete node;               // free this node
    }
}

// print list
void print(Node *node) {
    if (node) {
        std::cout << node->data << " ";
        print(node->next);
    }
}

int main() {
    // test code

    // create list
    Node *node = create(10);

    // print list
    print(node);
    std::cout << std::endl;
    // should be 9 8 7 ... 0

    // free list
    deallocate(node);
    return 0;
}
