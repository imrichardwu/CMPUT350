/*
  CMPUT 350 Lab 11

  Processing zero-sum matrix games read from stdin

  (Part 1: Prep)

  Players: p1 (row) and p2 (column)

  Input format:

  N M (2 ints)   // N = #moves(p1) = #rows > 0  M = #moves(p2) = #columns > 0
  N*M doubles    // A[i][j] (i=0..N-1, j=0..M-1) row-major ordering

  A[i][j] = payoff for p1 when p1 chooses move i and p2 chooses move j

  Example (RPS):

    3 3
    0 -1 +1
    +1 0 -1
    -1 +1 0

  followed by:

  N doubles (row player strategy, for options -r and -e)
  M doubles (col player strategy, for option -e)

  Output:

  -r:

    - best response strategy to given row strategy
    - game value for p1

  -e:

    - expected game value for p1 for given row and col strategy
*/

#include <cassert>
#include <iostream>

#include "Solve.h"

using namespace std;

// check whether end of input has been reached

void check_end() {
    string foo;
    cin >> foo;

    if (!!cin) {
        cerr << "*** input end not reached" << endl;
        exit(1);
    }
}

// read strategy from stdin and normalize it

void read_strat(int n, Vector &strat) {
    strat.clear();

    double sum = 0;

    for (int i = 0; i < n; ++i) {
        double v;

        cin >> v;

        if (!cin) {
            cerr << "*** unexpected end of input, strategy expected" << endl;
            exit(1);
        }

        if (v < 0 || v > 1) {
            cerr << "*** strategy value out of range: " << v << endl;
            exit(1);
        }

        strat.push_back(v);
        sum += v;
    }

    if (sum <= 0.99 || sum >= 1.01) {
        cerr << "*** bogus strategy, sum: " << sum << endl;
    }

    // normalize strategy

    for (auto &x : strat) {
        x *= 1.0 / sum;
    }
}

// writes title followed by strategy probabilties to stdout

void write_strat(const string &title, const Vector &strat) {
    cout << title << ":";

    for (const auto &x : strat) {
        cout << " " << x;
    }
    cout << endl;
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        cerr << "*** call: " << argv[0] << " (-r | -e) " << endl
             << "- input is read from stdin (dims payoff-matrix [row or col strategy or both])" << endl
             << "- payoffs are in view of the row player (p1)" << endl
             << "- options:" << endl
             << " -r : compute best response to row strategy" << endl
             << " -e : compute expected game value for p1" << endl;
        exit(1);
    }

    string opt(argv[1]);

    // read payoff matrix from stdin

    int N1 = 0;    // #moves p1 (rows)
    int N2 = 0;    // #moves p2 (columns)
    Matrix A;      // payoff matrix in view of p1

    cin >> N1 >> N2;

    assert(!!cin && N1 > 0 && N2 > 0);

    // set up rows

    for (int i = 0; i < N1; ++i) {
        A.push_back(Vector(N2));
    }

    // check dimensions

    assert((int)A.size() == N1);

    for (int i = 0; i < N1; ++i) {
        assert((int)A[i].size() == N2);
    }

    cout << N1 << " by " << N2 << " game" << endl;

    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < N2; ++j) {
            cin >> A[i][j];
            if (!cin) {
                cerr << "*** illegal input or premature end of input" << endl;
                exit(1);
            }
            printf("%+.6f ", A[i][j]);
        }
        cout << endl;
    }

    if (opt == "-r") {
        // best response to row strategy

        Vector strat1;

        read_strat(N1, strat1);
        write_strat("row strategy", strat1);
        check_end();
        best_response_to_row(A, strat1);

    } else if (opt == "-e") {
        // expected value for p1

        Vector strat1, strat2;

        read_strat(N1, strat1);
        write_strat("row strategy", strat1);
        read_strat(N2, strat2);
        write_strat("col strategy", strat2);
        check_end();
        expected_value(A, strat1, strat2);
    }

    return 0;
}
