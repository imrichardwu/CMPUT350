#include "Stack.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

template <typename T>
void test(bool &ok) {
    const int N = 10;

    ok = true;

    // test empty, top, push, pop

    {
        Stack<T> s;

        // test empty after construction
        if (!s.empty()) {
            cout << "constructed stack not empty" << endl;
            ok = false;
        }

        // test push/empty/top

        for (int i = 0; i < N; ++i) {
            s.push((const T)i);    // check whether push works with rvalues
            if (s.empty()) {
                cout << "stack empty after push" << endl;
                ok = false;
            }
            if (!(s.top() == static_cast<T>(i))) {
                cout << "top element incorrect after push" << endl;
                ok = false;
            }
        }

        // test pop/empty/top

        for (int i = N - 1; i >= 0; --i) {
            if (s.empty()) {
                cout << "stack empty before pop" << endl;
            }
            if (!(s.top() == static_cast<T>(i))) {
                cout << "top element incorrect before pop" << endl;
                ok = false;
            }
            s.pop();
        }

        if (!s.empty()) {
            cout << "stack not empty after all pops" << endl;
            ok = false;
        }
    }

    // test clear

    {
        Stack<T> s;
        const int N = 300;

        for (int i = 0; i < N; ++i) {
            s.push(static_cast<T>(i));
        }

        s.clear();

        if (!s.empty()) {
            cout << "stack not empty after clear" << endl;
            ok = false;
        }
    }

    // test operator<<
    // (can also be implemented using "visual inspection")
    {
        Stack<T> s;
        const int N = 5;

        // populate stack
        for (int i = 0; i < N; ++i) {
            s.push(static_cast<T>(i));
        }

        ostringstream out1, out2;

        // create expected output
        for (int i = N - 1; i >= 0; --i) {
            out1 << i << " ";
        }

        // create actual output
        out2 << s;

        // compare
        string str1 = out1.str(), str2 = out2.str();

        if (str1 != str2) {
            cout << "operator<< output incorrect '" << str1 << "' ::: '" << str2 << "'" << endl;
            ok = false;
        }
    }

    // test reverse_stack

    {
        Stack<T> s;
        const int N = 19;

        // populate stack
        for (int i = 0; i < N; ++i) {
            s.push(static_cast<T>(i));
        }

        reverse_stack(s);

        // check result
        for (int i = 0; i < N; ++i) {
            if (s.top() != static_cast<T>(i)) {
                cout << "reverse_stack result wrong " << i << " " << s.top() << endl;
                ok = false;
                break;
            }
            s.pop();
        }
    }
}

int main() {
    bool ok = true;
    test<int>(ok);
    test<float>(ok);
    if (ok) {
        cout << "all tests passed" << endl;
        return 0;
    } else {
        cout << "at leat one test failed" << endl;
        return 1;
    }
}
