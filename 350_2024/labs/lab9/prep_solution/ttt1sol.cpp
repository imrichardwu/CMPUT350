/*
  tic-tac-toe 1

  IMPORTANT: ensure that the entire program works for arbitrary values of N
  (e.g., when setting N=4 below the program reads and solves 4x4 ttt games)
  The winning condition for NxN ttt is having N pieces of the same colour
  in any row, column, or main diagonal
*/

#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// thrown when set() encounters an illegal input, describing the reason for
// the exception
struct InputException {
    InputException(const std::string &reason) : reason(reason) {}

    std::string reason;
};

class State {
public:
    // players (and how their pieces are stored)
    static constexpr int MAX = 1;       // DON'T CHANGE
    static constexpr int MIN = -MAX;    // DON'T CHANGE

    // empty cell
    static constexpr int EMPTY = 0;    // DON'T CHANGE

    // edge length
    static constexpr int N = 3;

    // number of squares
    static constexpr int SIZE = N * N;

    // how pieces (MIN,EMPTY,MAX) are displayed
    static constexpr std::array<char, 3> DISP = {'o', '-', 'x'};

    // initialize empty board
    // MAX to move
    State() {
        // ... implemented
        fill(&sq[0][0], &sq[0][0] + SIZE, EMPTY);
        to_move = MAX;
        filled = 0;
    }

    // return true iff board is full
    bool full() const {
        // ... implemented
        return filled >= SIZE;
    }

    /* initialize state from string (MAX to move) throw InputException when an
       error is encountered in s

       important: you must use get_disp()
    */
    void set(const string &s) {
        /* create input stream from string, now you can use >>, etc.
           like for cin; e.g. is >> c; if (!is) ...
        */
        istringstream is(s);
        char c;

        to_move = MAX;
        filled = 0;

        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                is >> c;
                if (!is) {
                    // missing a symbol
                    throw InputException("missing symbol");
                }
                if (c == get_disp(MAX)) {
                    sq[y][x] = MAX;
                    ++filled;
                } else if (c == get_disp(MIN)) {
                    sq[y][x] = MIN;
                    ++filled;
                } else if (c == get_disp(EMPTY)) {
                    sq[y][x] = EMPTY;
                } else {
                    // invalid symbol
                    throw InputException(string("invalid symbol ") + c);
                }
            }
        }
        is >> c;
        if (is) {
            // trailing symbol(s)
            throw InputException(string("trailing symbol ") + c);
        }
    }

    /* print state to cout

       format:

       xox
       o-x
       xxo
       x (8)

       last line: player to move, number of filled squares
       followed by new-line

       important: you must use get_disp() to display cells and the player to move
    */
    void print() const {
        // ... implemented
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                cout << get_disp(sq[y][x]);
            }
            cout << endl;
        }
        // print player to move and #filled squares
        cout << get_disp(to_move) << " (" << filled << ")" << endl;
    }

    // return character corresponding to square content (MIN,EMPTY,MAX)
    static char get_disp(int cont) {
        assert(cont == MAX || cont == MIN || cont == EMPTY);
        return DISP[cont + 1];
    }

private:
    // MAX or MIN to move
    int to_move;

    /*
      playing field (2d array)
      stores values (MIN,EMPTY,MAX)
      which simplifies winning computation
      bounds are checked in debug mode and sq[y][x] = 0 works
    */
    std::array<std::array<signed char, N>, N> sq;

    // number of non-empty squares
    int filled;
};

constexpr std::array<char, 3> State::DISP;
constexpr int State::EMPTY;

int main() {
    std::vector<string> boards {
        "--- \
        --- \
        ---",

        "--- \
        -o- \
        ---",

        "--- \
        --o \
        ---",

        "-oo \
        ooo \
        ooo",
    
        "--o \
        -oo \
        ---",

        "-xo \
        xox \
        xox",

        "-xo \
        oox \
        xoo",

        "--- \
        -x- \
        ---"
    };

    for (const string &s : boards) {
        // construct state from string
        // catch exception and print error message
        // print state

        // ... implemented
        try {
            State st;
            st.set(s);
            st.print();
            cout << endl;
        } catch (InputException &e) {
            cerr << "corrupt input: " << e.reason << " in " << s << endl;
        }
    }

    return 0;
}
