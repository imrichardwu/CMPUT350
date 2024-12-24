// game theoretic functions

#include "Solve.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

/*
  helper function (DON'T CHANGE!):

  - read lp_solve result from file name
  - assuming number of moves is move_num
  - move probabilities are stored in strat
  - return game value for player p1
*/

static double read_result(const char *name, int move_num, Vector &strat) {
    strat.clear();

    ifstream is;
    is.open(name, ifstream::in);

    string foo;
    double value, v;

    is >> foo >> foo >> foo >> foo >> value;

    if (!is || foo != "function:") {
        cerr << "*** corrupt output: " << foo << endl;
        exit(1);
    }

    is >> foo >> foo >> foo >> foo >> foo;    // skip text

    is >> foo >> v;

    if (!is || foo != "Z") {
        cerr << "*** corrupt output: expected Z" << endl;
        exit(1);
    }

    for (int i = 0; i < move_num; i++) {
        double v;

        is >> foo >> v;

        if (!is) {
            cerr << "*** variable value read error " << foo << endl;
            exit(1);
        }

        int ind;

        if (sscanf(foo.c_str(), "x%d", &ind) != 1) {
            cerr << "*** no variable " << foo << endl;
            exit(1);
        }

        if (ind != i) {
            cerr << "*** variable index corrupt " << foo << endl;
            exit(1);
        }

        strat.push_back(v);
    }

    is.close();

    if (fpclassify(value) == FP_NAN) {
        cerr << "*** invalid game value " << value << endl;
        exit(1);
    }

    return value;
}

// solve matrix game, print MiniMax strategies and value for p1 to stdout

void solve(Matrix &A) {
    double v1, v2;            // game values computed for both LPs (should be equal)
    Vector strat1, strat2;    // strategy for p1 and p2

    int N1 = A.size();    // number of rows
    assert(N1 > 0);
    int N2 = A[0].size();    // number of columns
    assert(N2 > 0);

    // compute minimum over A, which needs to be added to Z (game value) because
    // all variables are assumed to be >= 0 in lp_solve

    double min_A = A[0][0];

    (void)min_A;

    // ... implement ... remove (void)min_A;

    {
        /*
          LP for p1:

          max Z+min_A
          Z+min_A - sum_i=0..N1-1: A(i,j) * x_i <= 0 for all j=0..N2-1
          sum_i=0..N1-1: x_i = 1
          x_i >= 0 for all i=0..N1-1 (implicit)
        */

        FILE *fp = fopen("p1.lp", "w");

        // ... implement: write LP description to file ... (consider fprintf)
        // for the exact format, which is similar to the LP above,
        // run ./solve -s < test2x4.mgs and look at p1.lp

        fclose(fp);

        // launch external LP solver
        {
            int res = system("lp_solve < p1.lp > p1.res");
            (void)res;
        }    // make lab g++ happy

        // read result from output file
        v1 = read_result("p1.res", N1, strat1);
    }

    {
        /*
          LP for p2:

          min Z+min_A
          Z+min_A - sum_j=0..N2-1: A(i,j) * x_j >= 0 for all i=0..N1-1
          sum_j=0..N2-1: x_j = 1
          x_j >= 0 for all j=0..N2-1 (implicit)
        */

        FILE *fp = fopen("p2.lp", "w");

        // ... implement: write LP description to file ... (consider fprintf)
        // for the exact format, which is similar to the LP above,
        // run ./solve -s < test2x4.mgs and look at p2.lp

        fclose(fp);

        // launch external LP solver
        {
            int res = system("lp_solve < p2.lp > p2.res");
            (void)res;
        }    // make lab g++ happy

        // read result from output file
        v2 = read_result("p2.res", N2, strat2);
    }

    cout << "value for row player p1: " << v1 << endl;

    assert(abs(v1 - v2) < 1e-3);

    cout << "strategy p1:";
    for (auto &x : strat1) {
        cout << " " << x;
    }
    cout << endl;

    cout << "strategy p2:";
    for (auto &x : strat2) {
        cout << " " << x;
    }
    cout << endl;
}

// write best response to row strategy and value for p1 to stdout

void best_response_to_row(const Matrix &A, const Vector &strat1) {
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

// write best response to col strategy and value for p1 to stdout

void best_response_to_col([[maybe_unused]] const Matrix &A, [[maybe_unused]] const Vector &strat2) {
    // ... implement ...

    /* note: attribute [[maybe_unused]] is used to compile this code skeleton
       without warnings; please remove the attributes once you are done with
       implementing this function
     */
}

// write expected value of game for the row player for given row and col
// strategies to stdout

void expected_value(const Matrix &A, const Vector &strat1, const Vector &strat2) {
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
