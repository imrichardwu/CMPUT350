// p2.c
// include needed headerfiles here
// ...

// return a list containing nodes with data n-1,n-2,...2,1,0
// check for any preconditions
struct Node *create(int n) {
    // ...
}

// deallocate list by freeing all nodes in list starting with node
void deallocate(struct Node *node) {
    // ...
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
    // test code
    // ...
}
