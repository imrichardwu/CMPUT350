// game theoretic functions

#include "Solve.h"

#include <cassert>
#include <iostream>
#include <iomanip>

using namespace std;


// Write best response to row strategy and value for p1 to stdout
void best_response_to_row(const Matrix &A, const Vector &strat1) {
    size_t num_cols = A[0].size(); // Number of moves for p2 (columns)
    Vector expected_payoffs(num_cols, 0.0);

    // Compute the expected payoff for each column strategy of p2
    for (size_t j = 0; j < num_cols; ++j) {
        for (size_t i = 0; i < strat1.size(); ++i) {
            expected_payoffs[j] += strat1[i] * A[i][j]; // strat1[i] is the probability of row i and A[i][j] is the payoff for row i and column j
        }
    }

    // Find the minimum expected payoff (p2 aims to minimize p1's payoff)
    // Why? Because p2 is the row player in the dual game???
    double min_payoff = expected_payoffs[0];
    for (size_t j = 1; j < num_cols; ++j) {
        if (expected_payoffs[j] < min_payoff) {
            min_payoff = expected_payoffs[j];
        }
    }

    // Identify all column strategies that achieve the minimum payoff
    vector<size_t> best_columns;
    for (size_t j = 0; j < num_cols; ++j) {
        if (abs(expected_payoffs[j] - min_payoff) < 1e-9) { // Using a tolerance for floating-point comparison
            best_columns.push_back(j);
        }
    }

    // Assign equal probability to all best responses
    Vector best_response(num_cols, 0.0);
    double equal_prob = 1.0 / best_columns.size();
    for (const auto &j : best_columns) {
        best_response[j] = equal_prob;
    }

    // Output the best response strategy and the game value
    printf("best response to row strategy: ");
    for (size_t j = 0; j < num_cols; ++j) {
        cout << best_response[j] << " ";
    }
    cout << endl;
    cout << fixed << setprecision(1) << "game value: " << min_payoff << endl;
}

// Write expected value of game for the row player for given row and col strategies to stdout
void expected_value(const Matrix &A, const Vector &strat1, const Vector &strat2) {
    double ev = 0.0;

    // Calculate the expected value: sum_i sum_j strat1[i] * A[i][j] * strat2[j]
    for (size_t i = 0; i < strat1.size(); ++i) {
        for (size_t j = 0; j < strat2.size(); ++j) {
            ev += strat1[i] * A[i][j] * strat2[j];
        }
    }

    // Output the expected game value
    cout << fixed << setprecision(1) << "expected game value: " << ev << endl;
}