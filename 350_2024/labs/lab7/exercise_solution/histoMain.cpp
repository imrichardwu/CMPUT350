// driver: counts words on stdin and prints histogramm

#include <iostream>
#include <string>

#include "Histo.h"

using namespace std;

int main() {
    Histo h;

    for (;;) {
        string w;
        cin >> w;
        if (!cin) {
            // eof or error
            break;
        }

        // update histogram
        h.count(w);
    }

    // print histogram
    h.print(cout);
    return 0;
}
