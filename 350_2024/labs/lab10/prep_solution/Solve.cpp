// game theoretic functions

#include "Solve.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

// write best response to row strategy and value for p1 to stdout

void best_response_to_row(const Matrix &A, const Vector &strat1) {
    // ... implemented ...

    assert(A.size() > 0);
    int N1 = A.size();
    int N2 = A[0].size();

    assert((int)strat1.size() == N1);

    double min_val = 0;
    int min_ind = -1;

    for (int j = 0; j < N2; ++j) {
        double sum = 0;

        for (int i = 0; i < N1; ++i) {
            sum += strat1[i] * A[i][j];
        }

        if (min_ind < 0 || sum < min_val) {
            min_val = sum;
            min_ind = j;
        }
    }

    cout << "best response to row strategy:";

    for (int j = 0; j < N2; ++j) {
        if (j == min_ind) {
            cout << " 1";
        } else {
            cout << " 0";
        }
    }

    cout << endl << "value for p1: " << min_val << endl;
}

// write expected value of game for the row player for given row and col
// strategies to stdout

void expected_value(const Matrix &A, const Vector &strat1, const Vector &strat2) {
    // ... implemented ...

    assert(A.size() > 0);
    int N1 = A.size();
    int N2 = A[0].size();

    assert((int)strat1.size() == N1);
    assert((int)strat2.size() == N2);

    double sum = 0;

    for (int i = 0; i < N1; ++i) {
        assert((int)A[i].size() == N2);
        for (int j = 0; j < N2; ++j) {
            sum += strat1[i] * A[i][j] * strat2[j];
        }
    }

    cout << "expected value for p1: " << sum << endl;
}
