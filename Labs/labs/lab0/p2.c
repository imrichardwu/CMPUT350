#include <stdio.h>
#include <stdlib.h>
#include "p2.h"

// p2.c
// include needed headerfiles here
// ...

// return a list containing nodes with data n-1,n-2,...2,1,0
// check for any preconditions
struct Node *create(int n) {
    struct Node * head = NULL;
    struct Node * temp = NULL;

    for(int i = 0; i < n; i++) {
        temp = (struct Node*)malloc(sizeof(struct Node));
        if(temp == NULL) {
            exit(1);
        }
        temp->data = i;
        temp->next = head;
        head = temp;
    }
    return head;
}

// deallocate list by freeing all nodes in list starting with node
void deallocate(struct Node *node) {
    struct Node * current = NULL;
    while(node != NULL) {
        current = node;
        node = node->next;
        free(current);
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
    struct Node * myList = create(5);
    print(myList);
    deallocate(myList);
}
