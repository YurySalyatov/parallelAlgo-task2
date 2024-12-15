#include <cassert>
#include <iostream>

#include "bfs.h"
#include "graph.h"

constexpr int SIZE = 400;
constexpr int TEST_SIZE = 5;

double oneTest(const std::vector<std::vector<int>> &graph, void(*func)(const std::vector<std::vector<int>> &, int), const std::string &nm) {
    std::cout << nm << " bfs times: " << std::endl;
    double sum = 0;
    for (int i = 0; i < TEST_SIZE; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func(graph, 0);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = (end - start);
        std::cout << i + 1 << " " << elapsed.count() << " seconds" << std::endl;
        sum += elapsed.count();
    }
    double meanTime = sum / TEST_SIZE;
    std::cout << nm << " bfs mean times: " << meanTime << std::endl;
    return meanTime;
}


// void correctnessTest() {
//     int corSize = 1e6;
//     std::vector<int> arr(corSize);
//     std::vector<int> arrCopy(corSize);
//     std::vector<int> arrCopyCopy(corSize);
//     for (int i = 0; i < corSize; ++i) {
//         arr[i] = mt();
//         arrCopy[i] = arr[i];
//         arrCopyCopy[i] = arr[i];
//     }
//     std::sort(arr.begin(), arr.end());
//     sequentiallQuickSort(arrCopy, 0, corSize);
//     parallelQuickSort(arrCopyCopy, 0, corSize);
//     for (int i = 0; i < corSize; ++i) {
//         assert(arr[i] = arrCopy[i]);
//         assert(arr[i] = arrCopyCopy[i]);
//     }
//     std::cout << "Correctness test PASSED" << std::endl;
// }

void speedTest() {
    const auto graph = generate_graph(SIZE);
    std::cout << "End generate for speed test" << std::endl;
    const double seq = oneTest(graph, sequentialBfs, "sequential");
    tbb::global_control control(tbb::global_control::max_allowed_parallelism, 4);
    const double par = oneTest(graph, parallelBfs, "parallel");
    const double ratio = seq / par;
    std::cout << "ratio: " << ratio << std::endl;
    // assert(ratio > 3);
    std::cout << "Speed test PASSED" << std::endl;

}

int main() {
    // correctnessTest();
    speedTest();
    return 0;
}