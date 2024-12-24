/*
  tic-tac-toe 2
  solves states

  IMPORTANT: ensure that the entire program works for arbitrary values of N
  (e.g., when setting N=4 below the program reads and solves 4x4 ttt games)
  The winning condition for N-by-N ttt is having N pieces of the same colour
  in any row, column, or main diagonal. Given that N can vary, you'll need
  loops to detect winning conditions

*/

#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <tuple>
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

    // edge length (may change!)
    static constexpr int N = 3;

    // number of squares
    static constexpr int SIZE = N * N;

    // how pieces (MIN,EMPTY,MAX) are displayed
    static constexpr std::array<char, 3> DISP = {'o', '-', 'x'};

    // initialize empty board
    // MAX to move
    State() {
        fill(&sq[0][0], &sq[0][0] + SIZE, EMPTY);
        to_move = MAX;
        filled = 0;
    }

    // return true iff board is full
    bool full() const {
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

       important: you must use get_disp() to display cells and the player to
       move
    */
    void print() const {
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                cout << get_disp(sq[y][x]);
            }
            cout << endl;
        }
        // print player to move and #filled squares
        cout << get_disp(to_move) << " (" << filled << ")" << endl;
    }

    // return MAX if MAX won, MIN if MIN won, and 0 otherwise
    // (if both players have N in a row, return either MIN or MAX)
    int max_value() const {
        // ... implement ...
    }

    // make move (x, y) for player to_move (including board change, filled, and
    // to_move changes) and return true if and only if move is legal
    // if the move is illegal, the state should not be changed
    // pre-condition: x, y within range
    bool make_move(int x, int y) {
        // ... implement ...
    }

    // return player to move
    int get_to_move() const {
        // ... implement ...
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
      which simplifies winning condition computation
      bounds are checked in debug mode and sq[y][x] = 0 works
    */
    std::array<std::array<signed char, N>, N> sq;

    // number of non-empty squares
    int filled;
};

// not needed in C++17 because of constexpr:
constexpr std::array<char, 3> State::DISP;
constexpr int State::EMPTY;

/* negamax search

   In the first tuple entry returns state value (+1,0,-1) in VIEW of the
   PLAYER to MOVE (assuming both players play optimally until the end of
   game). I.e.,

   if MAX is to move and MAX wins, return +1
   if MAX is to move and MAX loses, return -1
   if MAX is to move and game is a draw, return 0
   if MIN is to move and MIN wins, return +1
   if MIN is to move and MIN loses, return -1
   if MIN is to move and game is a draw, return 0

   Also returns coordinates best_x and best_y of the best move, or (-1,-1) if
   no move is available, in the second and third tuple component

   [ access component i by using std::get<i>(...) ]
*/
tuple<int, int, int> negamax(const State &st) {
    // ... implement ...

    /* hints:
       - start by checking whether st is a terminal state using function
         max_value
       - then loop through all possible moves calling negamax
         recursively (see AI part4 p.27 for details)
       - because you need to also return the best found move, you need to track
         it in the loop
    */

    return tuple<int, int, int>(score, best_x, best_y);
}

/* solve position given in state st and print the following to stdout:
   - first print the state
   - then if a player already won without making a move,
     print "a player already won"
   - otherwise, print "draw" if no moves are available
   - otherwise, print "p wins with (x,y)" for player-to-move p (x or o) and
     a best move (x,y) if player p wins
   - otherwise, print "p draws with (x,y)" for player-to-move p (x or o) and
     a best move (x,y) if player p draws
   - otherwise, print "p loses" if player-to-move p (x or o) loses
*/
void solve(const State &st) {
    // ... implement ...

    // hint: use function negamax and distinguish above cases by inspecting its
    // return value
}

int main() {
    std::vector<string> boards
    {
        "--- \
        --- \
        ---", // x draws with (0,0)

        "--- \
        -o- \
        ---", // x draws with (0,0)

        "--- \
        --o \
        ---", // x draws with (0,1)

        "xox \
        oxo \
        oxo", // draw

        "-oo \
        ooo \
        ooo", // a player already won
    
        "oxo \
        xx- \
        oox", // x wins with (2,1)

        "--o \
        -oo \
        ---", // x loses

        "-xo \
        oox \
        xoo", // x draws with (0,0)

        "--- \
        -x- \
        ---"  // x wins with (0,0)
    };

    for (const string &s : boards) {
        /* - construct state from string
           - also catch exception and print error message
           - solve state
        */
        try {
            State st;
            st.set(s);
            solve(st);
            cout << endl;
        } catch (InputException &e) {
            cerr << "corrupt input: " << e.reason << " in " << s << endl;
        }
    }

    return 0;
}
