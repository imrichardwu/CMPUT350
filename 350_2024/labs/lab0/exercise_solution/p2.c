#include <stdio.h>
#include <stdlib.h>

// Note:

// The recursive implementations below will run into issues when the list is
// long and the runtime stack will be exhausted. Can you find a way to avoid
// this problem? A solution will be discussed in the lab (see p2-iter.c)

struct Node {
    int data;
    struct Node *next;
};

// return a list containing nodes with data n-1,n-2,...2,1,0 
struct Node *create(int n) {
    if (n <= 0) {
        return 0;
    }

    struct Node *p = malloc(sizeof(struct Node));
    p->data = n - 1;
    p->next = create(n - 1);
    return p;
}

//  deallocate list by freeing all nodes in list starting with node
void deallocate(struct Node *node) {
    if (node) {
        deallocate(node->next);    //  free subsequent nodes
        free(node);                // free this node
    }
}

// print list
void print(struct Node *node) {
    if (node) {
        printf("%d ", node->data);
        print(node->next);
    }
}

// check whether above functions work by creating a list and printing it
// also use valgrind for detecting memory leaks
void test() {
    //  test code
    struct Node *node = create(10);
    print(node);    // should be 9 8 7 ... 0
    printf("\n");
    deallocate(node);
}
