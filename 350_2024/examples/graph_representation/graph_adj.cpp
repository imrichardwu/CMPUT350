#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>

using namespace std::chrono;
using AdjacencyList = std::unordered_map<int, std::unordered_set<int>>;

bool is_edge(const AdjacencyList &adjacency_list, int i, int j) {
    if (adjacency_list.find(i) == adjacency_list.end()) {
        return false;
    }
    return adjacency_list.at(i).find(j) != adjacency_list.at(i).end();
}

void add_edge(AdjacencyList &adjacency_list, int i, int j) {
    adjacency_list[i].insert(j);
    adjacency_list[j].insert(i);
}

AdjacencyList create_graph_sparse(int size) {
    assert(size > 5 && size & 1);
    std::cout << "Building sparse graph .. ";
    AdjacencyList adjacency_list;
    int num_edges = 0;
    // triple diagonal
    for (int i = 1; i < size - 1; ++i) {
        add_edge(adjacency_list, i, i - 1);
        add_edge(adjacency_list, i, i + 1);
        add_edge(adjacency_list, i - 1, i);
        add_edge(adjacency_list, i + 1, i);
        num_edges += 4;
    }
    // First + last rows
    add_edge(adjacency_list, 0, 1);
    add_edge(adjacency_list, 1, 0);
    add_edge(adjacency_list, size - 1, size - 2);
    add_edge(adjacency_list, size - 2, size - 1);
    num_edges += 4;

    std::cout << num_edges << " edges" << std::endl;
    return adjacency_list;
}

AdjacencyList create_graph_dense(int size) {
    assert(size > 5 && size & 1);
    std::cout << "Building dense graph .. ";
    AdjacencyList adjacency_list;
    int num_edges = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (j > 0) {
                add_edge(adjacency_list, i, j - 1);
                ++num_edges;
            }
            if (j < size - 1) {
                add_edge(adjacency_list, i, j + 1);
                ++num_edges;
            }

            if (i > 0) {
                add_edge(adjacency_list, i - 1, j);
                ++num_edges;
            }
            if (i < size - 1) {
                add_edge(adjacency_list, i + 1, j);
                ++num_edges;
            }
        }
    }
    std::cout << num_edges << " edges" << std::endl;
    return adjacency_list;
}

// usage: ./graph_adj <SIZE> <IS_SPARSE>
int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <SIZE> <IS_SPARSE>" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    bool is_sparse = (bool)std::stoi(argv[2]);

    AdjacencyList adjacency_list = is_sparse ? create_graph_sparse(N) : create_graph_dense(N);

    std::default_random_engine gen;
    gen.seed(0);
    std::uniform_int_distribution<int> dist(0, N - 1);

    auto start = high_resolution_clock::now();
    int num_edges = 0;
    for (int i = 0; i < N * N; ++i) {
        num_edges += (int)is_edge(adjacency_list, dist(gen), dist(gen));
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "Num edges found: " << num_edges << " in " << (double)duration.count() / 1000 << " seconds."
              << std::endl;
}
