#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std::chrono;
using AdjacencyMatrix = std::vector<int>;

bool is_edge(const AdjacencyMatrix &adjacency_matrix, int i, int j, int size) {
    return adjacency_matrix[static_cast<std::size_t>(i * size + j)];
}

void add_edge(AdjacencyMatrix &adjacency_matrix, int i, int j, int size) {
    adjacency_matrix[static_cast<std::size_t>(i * size + j)] = true;
    adjacency_matrix[static_cast<std::size_t>(i * size + j)] = true;
}

AdjacencyMatrix create_graph_sparse(int size) {
    assert(size > 5 && size & 1);
    std::cout << "Building sparse graph .. ";
    AdjacencyMatrix adjacency_matrix(static_cast<std::size_t>(size * size), false);
    int num_edges = 0;
    // triple diagonal
    for (int i = 1; i < size - 1; ++i) {
        add_edge(adjacency_matrix, i, i - 1, size);
        add_edge(adjacency_matrix, i, i + 1, size);
        add_edge(adjacency_matrix, i - 1, i, size);
        add_edge(adjacency_matrix, i + 1, i, size);
        num_edges += 4;
    }
    // First + last rows
    add_edge(adjacency_matrix, 0, 1, size);
    add_edge(adjacency_matrix, 1, 0, size);
    add_edge(adjacency_matrix, size - 1, size - 2, size);
    add_edge(adjacency_matrix, size - 2, size - 1, size);
    num_edges += 4;

    std::cout << num_edges << " edges" << std::endl;
    return adjacency_matrix;
}

AdjacencyMatrix create_graph_dense(int size) {
    assert(size > 5 && size & 1);
    std::cout << "Building dense graph .. ";
    AdjacencyMatrix adjacency_matrix(static_cast<std::size_t>(size * size), false);
    std::cout << adjacency_matrix.size() << std::endl;
    int num_edges = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (j > 0) {
                add_edge(adjacency_matrix, i, j - 1, size);
                ++num_edges;
            }
            if (j < size - 1) {
                add_edge(adjacency_matrix, i, j + 1, size);
                ++num_edges;
            }

            if (i > 0) {
                add_edge(adjacency_matrix, i - 1, j, size);
                ++num_edges;
            }
            if (i < size - 1) {
                add_edge(adjacency_matrix, i + 1, j, size);
                ++num_edges;
            }
        }
    }
    std::cout << num_edges << " edges" << std::endl;
    return adjacency_matrix;
}

// usage: ./graph_mat <SIZE> <IS_SPARSE>
int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <SIZE> <IS_SPARSE>" << std::endl;    // NOLINT(*-pointer-arithmetic)
        return 1;
    }

    int N = std::stoi(argv[1]);                   // NOLINT(*-pointer-arithmetic)
    bool is_sparse = (bool)std::stoi(argv[2]);    // NOLINT(*-pointer-arithmetic)

    AdjacencyMatrix adjacency_matrix = is_sparse ? create_graph_sparse(N) : create_graph_dense(N);

    std::default_random_engine gen;
    gen.seed(0);
    std::uniform_int_distribution<int> dist(0, N - 1);

    auto start = high_resolution_clock::now();
    int num_edges = 0;
    for (int i = 0; i < N * N; ++i) {
        num_edges += (int)is_edge(adjacency_matrix, dist(gen), dist(gen), N);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "Num edges found: " << num_edges << " in " << (double)duration.count() / 1000 << " seconds."
              << std::endl;
}
