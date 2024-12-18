#include <vector>
#include <atomic>
#include <tbb/tbb.h>
// #include <iostream>

constexpr int CHUNK_SIZE = 512;
constexpr int MAX_SIZE = 1e9;
void sequentialBfs(const std::vector<std::vector<int> > &graph, const int start = 0) {
    std::vector<int> front = {start};
    std::vector<bool> visited(graph.size(), false);
    visited[start] = true;
    while (!front.empty()) {
        std::vector<int> new_front;
        for (auto v: front) {
            for (auto u: graph[v]) {
                if (!visited[u]) {
                    visited[u] = true;
                    new_front.push_back(u);
                }
            }
        }
        front = new_front;
    }
    // for (int i = 0; i < graph.size(); i++) {
    //     if (!visited[i]) {
    //         std::cout << "don't visited " << i << std::endl;
    //     }
    // }
}

long long pscan(const std::vector<int> &vec, std::vector<long>& res, const int sz) {
    tbb::parallel_scan(
        tbb::blocked_range<size_t>(0, sz, CHUNK_SIZE),
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
        tbb::blocked_range<size_t>(0, sz, CHUNK_SIZE),
        [&](const tbb::blocked_range<size_t> &range) {
            for (size_t i = range.begin(); i < range.end(); i++) {
                res[i] -= vec[i];
            }
        }
    );
    return ans;
}

int pfilter(const std::vector<int> &vec, std::vector<int> &med, std::vector<long> &res_scan, const int sz, std::vector<int>& res) {
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, sz, CHUNK_SIZE),
        [&](const tbb::blocked_range<size_t> &range) {
            for (size_t i = range.begin(); i < range.end(); i++) {
                med[i] = vec[i] >= 0 ? 1 : 0;
            }
        });
    const auto scan_sz = pscan(med, res_scan, sz);
    // std::vector<int> res(scan_sz);
    std::atomic<int> counter(0);
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, sz, CHUNK_SIZE),
        [&](const tbb::blocked_range<size_t> &range) {
            for (size_t i = range.begin(); i < range.end(); ++i) {
                if ((i < sz - 1 && res_scan[i] < res_scan[i + 1])
                    || (i == sz - 1 && res_scan[i] < scan_sz)) {
                        res[res_scan[i]] = vec[i];
                        ++counter;
                }
            }
        });
    return counter.load();
}


void parallelBfs(const std::vector<std::vector<int> > &graph, const int start = 0) {
    auto visited = std::vector<std::atomic<bool> >(graph.size());
    for (auto &i: visited) {
        i.store(false);
    }
    visited[start].store(true);
    bool expected = false;
    // std::atomic<int> curV(0);
    std::vector<int> frontier(graph.size());
    frontier[0] = start;
    // std::vector<int> frontier = {start};
    int cnt = 1;
    std::vector<int> deg_(graph.size());
    std::vector<long> scan_deg_(graph.size());
    std::vector<int> v_sz(graph.size());
    std::vector<int> med(graph.size());
    int mx_deg = 0;
    for(int i = 0; i < graph.size(); ++i) {
        v_sz[i] = static_cast<int>(graph[i].size());
        mx_deg = mx_deg >= v_sz[i] ? mx_deg : v_sz[i];
    }
    while (cnt != 0) {
        tbb::parallel_for(
            tbb::blocked_range<size_t>(0, cnt, CHUNK_SIZE),
            [&](const tbb::blocked_range<size_t> &range) {
                for (size_t i = range.begin(); i < range.end(); ++i) {
                    deg_[i] = v_sz[frontier[i]];
                }
            });
        auto scan_sm = pscan(deg_, scan_deg_, cnt);
        auto new_frontier = std::vector<int>(scan_sm, -1);
        tbb::parallel_for(
            tbb::blocked_range<size_t>(0, cnt, CHUNK_SIZE),
            [&](const tbb::blocked_range<size_t> &out_range) {
                for (size_t i = out_range.begin(); i < out_range.end(); ++i) {
                    auto v = frontier[i];
                    for (size_t j = 0; j < graph[v].size(); ++j) {
                        auto u = graph[v][j];
                        if (visited[u] || !visited[u].compare_exchange_strong(expected, true)) continue;
                        new_frontier[scan_deg_[i] + j] = u;
                    }
                }
            });
        cnt = pfilter(new_frontier, med, scan_deg_, static_cast<int>(new_frontier.size()), frontier);
        // cnt = static_cast<int>(frontier.size());
        // cnt = curV.load();
        // curV.store(0);
    }
    // for (int i = 0; i < graph.size(); i++) {
    //     if (!visited[i]) {
    //         std::cout << "don't visited " << i << std::endl;
    //     }
    // }
}
