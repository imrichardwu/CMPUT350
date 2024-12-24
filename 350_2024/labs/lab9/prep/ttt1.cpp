/*
  tic-tac-toe

  IMPORTANT: ensure that the entire program works for arbitrary values of N
  (e.g., when setting N=4 below the program reads 4x4 ttt games)
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
        // ... implement
    }

    // return true iff board is full
    bool full() const {
        // ... implement
    }

    /* initialize state from string (assuming MAX to move)
       throw InputException when an error is encountered in s

       important: you must use get_disp()
    */
    void set(const string &s) {
        /* create input stream from string, now you can use >>, etc.
           like for cin; e.g., is >> c; if (!is) ...
        */
        istringstream is(s);

        // ... implement
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
        // ... implement
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
      stores values (MAX,EMPTY,MIN)
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
        // construct state from string using function State::set
        // catch exception and print error message
        // otherwise print state using function State::print

        // ... implement
    }

    return 0;
}
