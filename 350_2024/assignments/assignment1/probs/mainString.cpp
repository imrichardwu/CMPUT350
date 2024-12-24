// tests String implementation

#include <iostream>

#include "String.h"

using namespace std;

int main() {
    String s("aaa"), t("bbbb");    // construct
    String u = t;                  // copy construct
    String v("cccc");              // construct
    v = s;                         // assign

    cout << "s: " << s.cstr() << " " << s.size() << " " << s.ref_count() << endl
         << "t: " << t.cstr() << " " << t.size() << " " << t.ref_count() << endl
         << "u: " << u.cstr() << " " << u.size() << " " << u.ref_count() << endl
         << "v: " << v.cstr() << " " << v.size() << " " << v.ref_count() << endl;

    return 0;
}

/*
  output:

s: aaa 3 2
t: bbbb 4 2
u: bbbb 4 2
v: aaa 3 2

*/
