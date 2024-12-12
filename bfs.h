//
// Created by yurys on 12.12.2024.
//
#include <vector>
#include <queue>
#include <atomic>


void sequentialBfs(std::vector<std::vector<int>> graph) {
    std::queue<int> queue;
    std::vector<bool> visited(graph.size(), false);
    queue.push(0);
    while(!queue.empty()) {
        int v = queue.front();
        queue.pop();
        visited[v] = true;
        for(auto u : graph[v]) {
            if(!visited[u]) {
                queue.push(u);
            }
        }
    }
}

void parallelBfs(std::vector<std::vector<int>> graph) {
    std::vector<std::atomic<bool>> visited(graph.size(), false);
    std::

}
