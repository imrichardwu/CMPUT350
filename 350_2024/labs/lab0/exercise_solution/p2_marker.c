#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Include the students submission
#include "p2.h"

// return a list containing nodes with data n-1,n-2,...2,1,0 
struct Node *create_sol(int n) {
    if (n <= 0) {
        return 0;
    }

    struct Node *p = malloc(sizeof(struct Node));
    p->data = n - 1;
    p->next = create_sol(n - 1);
    return p;
}

//  deallocate list by freeing all nodes in list starting with node
void deallocate_sol(struct Node *node) {
    if (node) {
        deallocate_sol(node->next);    //  free subsequent nodes
        free(node);                    // free this node
    }
}

// Run for an array of inputs
int main(int argc, char *argv[]) {
    (void)argc;
    if (argc <= 1) {
        printf("Error: need to run with command line argument of array size.\n");
        exit(1);
    }
    int N = atoi(argv[1]);

    struct Node *head = create(N);
    struct Node *head_sol = create_sol(N);

    struct Node *head_ptr = head;
    struct Node *head_sol_ptr = head;
    if (N <= 0) {
        printf(head_ptr == 0 ? "Pass.\n" : "Fail, head not NULL.\n");
    } else {
        while (head_sol_ptr && head_ptr) {
            if (head_sol_ptr->data != head_ptr->data) {
                printf("Failed, data miss-match.\n");
                exit(1);
            }
            head_sol_ptr = head_sol_ptr->next;
            head_ptr = head_ptr->next;
        }
        // Ensure both linked lists have reached the end
        printf((!head_sol_ptr && !head_ptr) ? "Pass.\n" : "Fail, size miss-match.\n");
    }
    deallocate(head);
    deallocate_sol(head_sol);
}
