/*
  tic-tac-toe 2
  solves states

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

    // edge length
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
            // trailing symbol
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
        // ... implemented ...

        // check rows
        for (int y = 0; y < N; ++y) {
            int s = 0;
            for (int x = 0; x < N; ++x) {
                s += sq[y][x];
            }
            if (s == N * MAX) {
                return MAX;
            }
            if (s == N * MIN) {
                return MIN;
            }
        }

        // check columns
        for (int x = 0; x < N; ++x) {
            int s = 0;
            for (int y = 0; y < N; ++y) {
                s += sq[y][x];
            }
            if (s == N * MAX) {
                return MAX;
            }
            if (s == N * MIN) {
                return MIN;
            }
        }

        // check main diagonals
        {
            int s = 0;
            for (int x = 0; x < N; ++x) {
                s += sq[x][x];
            }
            if (s == N * MAX) {
                return MAX;
            }
            if (s == N * MIN) {
                return MIN;
            }
        }

        {
            int s = 0;
            for (int x = 0; x < N; ++x) {
                s += sq[x][N - 1 - x];
            }
            if (s == N * MAX) {
                return MAX;
            }
            if (s == N * MIN) {
                return MIN;
            }
        }

        return 0;
    }

    // make move (x, y) for player to_move
    // and return true iff move is legal
    // pre-condition: x, y within range
    bool make_move(int x, int y) {
        // ... implemented ...

        assert(x >= 0 && x < N && y >= 0 && y < N);
        auto &c = sq[y][x];
        if (c) {
            return false;    // already occupied
        }

        c = to_move;
        to_move = -to_move;
        ++filled;
        return true;
    }

    // return player to move
    int get_to_move() const {
        // ... implemented ...
        return to_move;
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
    // ... implemented ...

    int v = st.max_value();
    if (v != 0 || st.full()) {
        // game finished, return value in view of player to move and illegal move
        return tuple<int, int, int>(st.get_to_move() == State::MAX ? v : -v, -1, -1);
    }

    assert(!st.full());

    int score = -2;    // less than valid scores MIN,0,MAX
    assert(score < State::MIN);
    State new_state(st);
    bool move_found = false;
    int best_x = -1, best_y = -1;

    // loop through all successors
    for (int x = 0; x < State::N; ++x) {
        for (int y = 0; y < State::N; ++y) {
            if (new_state.make_move(x, y)) {
                move_found = true;
                int v = -get<0>(negamax(new_state));
                if (v > score) {
                    // found better move
                    score = v;
                    best_x = x;
                    best_y = y;
                    if (score >= State::MAX) {
                        goto done;    // done, can't get any better ...
                    }
                }
                // setup state for next move
                new_state = st;
            }
        }
    }

done:;

    assert(move_found);
    assert(best_x >= 0 && best_x < State::N && best_y >= 0 && best_y < State::N);    // move was found

    return tuple<int, int, int>(score, best_x, best_y);
    // this works, too, in C++17:
    // return { score, best_x, best_y };
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
    // ... implemented ...

    st.print();
    int v = st.max_value();

    if (v != 0) {
        cout << "a player already won";

    } else if (st.full()) {
        cout << "draw";

    } else {
        char tm = State::get_disp(st.get_to_move());
        auto t = negamax(st);
        int v = get<0>(t);
        int best_x = get<1>(t);
        int best_y = get<2>(t);

        if (v > 0) {
            cout << tm << " wins with (" << best_x << "," << best_y << ")";
        } else if (v == 0) {
            cout << tm << " draws with (" << best_x << "," << best_y << ")";
        } else {
            cout << tm << " loses";
        }
    }

    cout << endl;
}

// not needed in C++17 because of constexpr:
constexpr std::array<char, 3> State::DISP;
constexpr int State::EMPTY;

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
