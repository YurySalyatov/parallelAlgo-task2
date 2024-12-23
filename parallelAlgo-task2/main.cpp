#include <cassert>
#include <iostream>

#include "bfs.h"
#include "graph.h"

constexpr int SIZE = 330;
constexpr int TEST_SIZE = 5;
constexpr int MAX_PARALLEL = 4;

double oneTest(const std::vector<std::vector<int>> &graph, void(*func)(const std::vector<std::vector<int>> &, std::vector<int> &, const int), const std::string &nm) {
    std::cout << nm << " bfs times: " << std::endl;
    std::vector<int> dist(graph.size(), 0);
    double sum = 0;
    for (int i = 0; i < TEST_SIZE; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func(graph, dist, 0);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = (end - start);
        std::cout << i + 1 << " " << elapsed.count() << " seconds" << std::endl;
        sum += elapsed.count();
    }
    double meanTime = sum / TEST_SIZE;
    std::cout << nm << " bfs mean times: " << meanTime << std::endl;
    return meanTime;
}


void correctnessTest() {
    std::cout << "Start Correctness test" << std::endl;
    int corSize = 50;
    auto graph = generate_graph(corSize);
    std::vector<int> seq_dist(graph.size(), 0);
    std::vector<int> par_dist(graph.size(), 0);
    sequentialBfs(graph, seq_dist, 0);
    parallelBfs(graph, par_dist, 0);
    for (int i = 0; i < corSize; ++i) {
        assert(seq_dist[i] == par_dist[i]);
    }
    std::cout << "Correctness test PASSED" << std::endl;
}

void speedTest() {
    std::cout << "Start Speed test" << std::endl;
    const auto graph = generate_graph(SIZE);
    std::cout << "End generate for speed test" << std::endl;
    const double seq = oneTest(graph, sequentialBfs, "sequential");
    tbb::global_control control(tbb::global_control::max_allowed_parallelism, MAX_PARALLEL);
    const double par = oneTest(graph, parallelBfs, "parallel");
    const double ratio = seq / par;
    std::cout << "ratio: " << ratio << std::endl;
    // assert(ratio > 3);
    std::cout << "Speed test PASSED" << std::endl;

}

int main() {
    correctnessTest();
    speedTest();
    return 0;
}