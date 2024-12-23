#include <vector>
#include <atomic>
#include <tbb/tbb.h>
#include <queue>
// #include <iostream>

constexpr int PFOR_CHUNK_SIZE = 5 * 1e3;
constexpr int PSCAN_CHUNK_SIZE = 1e3;

void sequentialBfs(const std::vector<std::vector<int> > &graph, std::vector<int> &dist, const int start = 0) {
    std::queue<int> front;
    std::vector<bool> visited(graph.size(), false);
    front.push(start);
    visited[start] = true;
    dist[start] = 0;
    while (!front.empty()) {
        int v = front.front();
        front.pop();
        for (auto u: graph[v]) {
            if (!visited[u]) {
                dist[u] = dist[v] + 1;
                visited[u] = true;
                front.push(u);
            }
        }
    }
}

long long pscan(const std::vector<int> &vec, std::vector<long> &res, const long long sz) {
    tbb::parallel_scan(
            tbb::blocked_range<size_t>(0, sz, PSCAN_CHUNK_SIZE),
            0,
            [&](const tbb::blocked_range<size_t> &range, const long sum, const bool is_final_sum) {
                long tmp = sum;
                for (size_t i = range.begin(); i < range.end(); ++i) {
                    tmp += vec[i];
                    if (is_final_sum) {
                        res[i] = tmp;
                    }
                }
                return tmp;
            },
            [&](long a, long b) {
                return a + b;
            }
    );
    const long ans = res[sz - 1];
    tbb::parallel_for(
            tbb::blocked_range<size_t>(0, sz, PFOR_CHUNK_SIZE),
            [&](const tbb::blocked_range<size_t> &range) {
                for (size_t i = range.begin(); i < range.end(); i++) {
                    res[i] -= vec[i];
                }
            }
    );
    return ans;
}

long long pfilter(const std::vector<int> &vec, std::vector<int> &med, std::vector<long> &res_scan, const long long sz,
                  std::vector<int> &res) {
    tbb::parallel_for(
            tbb::blocked_range<size_t>(0, sz, PFOR_CHUNK_SIZE),
            [&](const tbb::blocked_range<size_t> &range) {
                for (size_t i = range.begin(); i < range.end(); i++) {
                    med[i] = vec[i] >= 0 ? 1 : 0;
                }
            });
    const auto scan_sz = pscan(med, res_scan, sz);
    tbb::parallel_for(
            tbb::blocked_range<size_t>(0, sz, PFOR_CHUNK_SIZE),
            [&](const tbb::blocked_range<size_t> &range) {
                for (size_t i = range.begin(); i < range.end(); ++i) {
                    if ((i < sz - 1 && res_scan[i] < res_scan[i + 1])

                        Юрий Салятов, [23.12.2024 18:05]
                    (i == sz - 1 && res_scan[i] < scan_sz)) {
                        res[res_scan[i]] = vec[i];
                    }
                }
            });
    return scan_sz;
}


void parallelBfs(const std::vector<std::vector<int> > &graph, std::vector<int> &dist, const int start = 0) {
    auto visited = std::vector<std::atomic<bool> >(graph.size());
    tbb::parallel_for(
            tbb::blocked_range<size_t>(0, graph.size(), PFOR_CHUNK_SIZE),
            [&](const tbb::blocked_range<size_t> &range) {
                for (size_t i = range.begin(); i < range.end(); ++i) {
                    visited[i].store(false);
                }
            });
    visited[start].store(true);
    bool expected = false;
    std::vector<int> frontier(graph.size());
    frontier[0] = start;
    long long cnt = 1;
    std::vector<int> deg_(graph.size());
    std::vector<long> scan_deg_(graph.size());
    std::vector<int> v_sz(graph.size());
    std::vector<int> med(graph.size());
    std::vector<int> new_frontier(4 * graph.size());
    tbb::parallel_for(
            tbb::blocked_range<size_t>(0, graph.size(), PFOR_CHUNK_SIZE),
            [&](const tbb::blocked_range<size_t> &range) {
                for (size_t i = range.begin(); i < range.end(); ++i) {
                    v_sz[i] = static_cast<int>(graph[i].size());
                }
            });
    while (cnt != 0) {
        tbb::parallel_for(
                tbb::blocked_range<size_t>(0, cnt, PFOR_CHUNK_SIZE),
                [&](const tbb::blocked_range<size_t> &range) {
                    for (size_t i = range.begin(); i < range.end(); ++i) {
                        deg_[i] = v_sz[frontier[i]];
                    }
                });
        auto scan_sm = pscan(deg_, scan_deg_, cnt);
        tbb::parallel_for(
                tbb::blocked_range<size_t>(0, cnt, PFOR_CHUNK_SIZE),
                [&](const tbb::blocked_range<size_t> &out_range) {
                    for (size_t i = out_range.begin(); i < out_range.end(); ++i) {
                        auto v = frontier[i];
                        for (size_t j = 0; j < graph[v].size(); ++j) {
                            auto u = graph[v][j];
                            if (visited[u]  !visited[u].compare_exchange_strong(expected, true)) {
                                new_frontier[scan_deg_[i] + j] = -1;
                            } else {
                                new_frontier[scan_deg_[i] + j] = u;
                                dist[u] = dist[v] + 1;
                            }
                        }
                    }
                });
        cnt = pfilter(new_frontier, med, scan_deg_, scan_sm, frontier);
    }
}