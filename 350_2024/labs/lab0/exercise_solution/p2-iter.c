#include <stdio.h>
#include <stdlib.h>

// iterative solution which can handle HUGE lists

struct Node {
    int data;
    struct Node *next;
};

// return a list containing nodes with data n-1,n-2,...2,1,0
struct Node *create(int n) {
    if (n <= 0) {
        return 0;
    }

    // construct list in reverse
    struct Node *p = 0;

    for (int i = 0; i < n; ++i) {
        struct Node *q = malloc(sizeof(struct Node));
        q->data = i;
        q->next = p;
        p = q;
    }
    return p;
}

// deallocate list by freeing all nodes in list starting with node;
void deallocate(struct Node *node) {
    while (node) {
        struct Node *p = node->next;
        free(node);
        node = p;
    }
}

// print list
void print(struct Node *node) {
    while (node) {
        printf("%d ", node->data);
        node = node->next;
    }
}

int main() {
    // test code

    struct Node *node = create(10);
    print(node);
    printf("\n");
    // should be 9 8 7 ... 0
    deallocate(node);
    return 0;
}
