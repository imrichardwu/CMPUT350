// test code for provided int SList

#include "SList.h"

#include <iostream>

using namespace std;

int main() {
    SList list;

    for (int i = 0; i < 10; ++i) {
        list.add_head(i);
    }

    // 9 8 7 ... 0
    while (list.get_head()) {
        cout << list.get_head()->data << endl;
        list.remove_head();
    }

    return 0;
}
