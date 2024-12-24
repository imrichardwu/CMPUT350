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
        // Check rows
        for (int y=0; y<N; ++y) {
            int first = sq[y][0];
            if(first == EMPTY) continue;
            bool win = true;
            for (int x=1; x<N; ++x) {
                if (sq[y][x] != first) {
                    win = false;
                    break;
                }
            }
            if(win){
                return first;
            }
        }

        // Check columns
        for (int x=0; x<N; ++x) {
            int first = sq[0][x];
            if(first == EMPTY) continue;
            bool win = true;
            for (int y=1; y<N; ++y) {
                if (sq[y][x] != first) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return first;
            }
        }

        // Check main diagonal
        int first = sq[0][0];
        if (first != EMPTY) {
            bool win = true;
            for (int i=1; i<N; ++i) {
                if (sq[i][i] != first) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return first;
            }
        }

        // Check anti-diagonal
        first = sq[0][N-1];
        if (first != EMPTY) {
            bool win = true;
            for (int i=1; i<N; ++i) {
                if (sq[i][N-1-i] != first) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return first;
            }
        }

        return EMPTY;
    }

    // make move (x, y) for player to_move (including board change, filled, and
    // to_move changes) and return true if and only if move is legal
    // if the move is illegal, the state should not be changed
    // pre-condition: x, y within range
    bool make_move(int x, int y) {
        assert(x >= 0 && x < N);
        assert(y >= 0 && y < N);
        if (sq[y][x] != EMPTY) {
            return false;
        }
        sq[y][x] = to_move;
        ++filled;
        to_move = -to_move;
        return true;
    }

    // return player to move
    int get_to_move() const {
        return to_move;
    }

    // return character corresponding to square content (MIN,EMPTY,MAX)
    static char get_disp(int cont) {
        assert(cont == MAX || cont == MIN || cont == EMPTY);
        return DISP[cont + 1];
    }

    const std::array<std::array<signed char, N>, N> & get_sq() const { return sq; }

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
    /* hints:
       - start by checking whether st is a terminal state using function
         max_value
       - then loop through all possible moves calling negamax
         recursively (see AI part4 p.27 for details)
       - because you need to also return the best found move, you need to track
         it in the loop
    */
    int winner = st.max_value();
    int current_player = st.get_to_move();

    if (winner != State::EMPTY) {
        if (winner == -current_player) {
            // Opponent has created a win
            return make_tuple(-1, -1, -1);
        }
        else if (winner == current_player) {
            // Current player already has a win
            return make_tuple(1, -1, -1);
        }
    }

    if (st.full()) {
        // Draw
        return make_tuple(0, -1, -1);
    }

    // Initialize best score to -infinity
    int score = -2; 
    int best_x = -1, best_y = -1;

    for (int y=0; y<State::N; ++y) {
        for (int x=0; x<State::N; ++x) {
            if (st.get_sq()[y][x] == State::EMPTY) {
                State child = st;
                if(child.make_move(x,y)){
                    // Recursive call
                    tuple<int, int, int> child_result = negamax(child);
                    int child_score = -std::get<0>(child_result);

                    if (child_score > score) {
                        score = child_score;
                        best_x = x;
                        best_y = y;
                        if (score == 1) {
                            // Best possible
                            return make_tuple(score, best_x, best_y);
                        }
                    }
                }
            }
        }
    }

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
    // hint: use function negamax and distinguish above cases by inspecting its
    // return value

    st.print();

    int winner = st.max_value();

    if (winner != State::EMPTY) {
        cout << "a player already won" << endl;
        return;
    }

    if (st.full()) {
        cout << "draw" << endl;
        return;
    }

    // Call negamax
    tuple<int, int, int> result = negamax(st);
    int score = std::get<0>(result);
    int best_x = std::get<1>(result);
    int best_y = std::get<2>(result);

    char p = State::get_disp(st.get_to_move());

    if (score == 1) {
        // Player can force a win
        cout << p << " wins with (" << best_x << "," << best_y << ")" << endl;
    }
    else if (score == 0) {
        // Player can force a draw
        cout << p << " draws with (" << best_x << "," << best_y << ")" << endl;
    }
    else {
        // Player will lose
        cout << p << " loses" << endl;
    }
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
