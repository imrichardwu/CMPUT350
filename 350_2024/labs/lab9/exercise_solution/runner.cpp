#include <cassert>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <streambuf>
#include <stdlib.h>
#include <string>
#include <sstream>


#define main YY_main
#include "ttt2.cpp"
#undef main


namespace SOLUTION {
    #include "ttt2sol.cpp"
};

 std::vector<string> boards_3x3 {
    "xxx \
    --- \
    ---",

    "--- \
    -o- \
    ---",

    "x-- \
    -xo \
    --x",

    "o-- \
    -oo \
    x-o",

    "xox \
    oxo \
    oxo",

    "-oo \
    ooo \
    ooo",

    "oxo \
    xx- \
    oox",

    "x-o \
    xoo \
    x--",

    "-xo \
    oox \
    xoo", 

    "--- \
    -x- \
    ooo" 
};

std::vector<std::array<int, 2>> moves {
    {0, 0},
    {2, 2},
    {1, 2},
    {2, 1},
    {0, 0},
    {0, 0},
    {2, 0},
    {2, 2},
    {0, 0},
    {0, 1},
};

class CoutRedirect {

public:
    CoutRedirect() {
        old = std::cout.rdbuf( buffer.rdbuf() ); // redirect cout to buffer stream
    }

    std::string getString() {
        return buffer.str(); // get string
    }

    ~CoutRedirect( ) {
        std::cout.rdbuf( old ); // reverse redirect
    }

private:
    std::stringstream buffer;
    std::streambuf * old;
};



void test_max_value(bool &ok) {
    State state;
    SOLUTION::State sol_state;
    bool local_ok = true;
    for (const auto & b : boards_3x3) {
        sol_state.set(b);
        state.set(b);
        local_ok &= sol_state.max_value() == state.max_value();
    }
    if (!local_ok) {
        std::cout << "Failed max_value" << std::endl;
    }
    ok &= local_ok;
}

void test_make_move(bool &ok) {
    State state;
    SOLUTION::State sol_state;
    bool local_ok_state = true;
    bool local_ok_res = true;
    bool local_ok_to_move = true;
    for (int i = 0; i < (int)boards_3x3.size(); ++i) {
        sol_state.set(boards_3x3[i]);
        state.set(boards_3x3[i]);
        std::string str_sol_state, str_state;
        bool res_sol, res;
        int to_move_sol_pre, to_move_sol_post;
        int to_move_pre, to_move_post;

        {
            CoutRedirect cr;
            to_move_sol_pre = sol_state.get_to_move();
            res_sol = sol_state.make_move(moves[i][0], moves[i][1]);
            to_move_sol_post = sol_state.get_to_move();
            sol_state.print();
            str_sol_state = cr.getString();
        }
        {
            CoutRedirect cr;
            to_move_pre = state.get_to_move();
            res = state.make_move(moves[i][0], moves[i][1]);
            to_move_post = state.get_to_move();
            state.print();
            str_state = cr.getString();
        }
        local_ok_state &= str_sol_state == str_state;
        local_ok_res &= res_sol == res;
        local_ok_to_move &= to_move_sol_pre == to_move_pre && to_move_sol_post && to_move_post;
    }
    if (!local_ok_state) {
        std::cout << "Failed make_move state update" << std::endl;
    }
    if (!local_ok_res) {
        std::cout << "Failed make_move returned value" << std::endl;
    }
    if (!local_ok_to_move) {
        std::cout << "Failed get_to_move" << std::endl;
    }
    ok &= local_ok_state && local_ok_res && local_ok_to_move;
}


void test_negamax(bool &ok) {
    SOLUTION::State sol_state;
    State state;
    bool local_ok_score = true;
    bool local_ok_bestx = true;
    bool local_ok_besty = true;
    for (const auto & b : boards_3x3) {
        sol_state.set(b);
        state.set(b);
        auto res_sol = SOLUTION::negamax(sol_state);
        auto res = negamax(state);
        local_ok_score &= std::get<0>(res_sol) == std::get<0>(res);
        local_ok_bestx &= std::get<1>(res_sol) == std::get<1>(res);
        local_ok_besty &= std::get<2>(res_sol) == std::get<2>(res);
    }
    if (!local_ok_score) {
        std::cout << "Failed negamax score" << std::endl;
    }
    if (!local_ok_bestx) {
        std::cout << "Failed negamax bestx" << std::endl;
    }
    if (!local_ok_besty) {
        std::cout << "Failed negamax besty" << std::endl;
    }
    ok &= local_ok_score && local_ok_bestx && local_ok_besty;
}



int main() {
    bool ok = true;

    test_max_value(ok);
    test_make_move(ok);
    test_negamax(ok);

    if (ok) {
        std::cout << "all tests passed" << std::endl;
        return 0;
    } else {
        std::cout << "at least one test failed!" << std::endl;
        return 1;
    }
}