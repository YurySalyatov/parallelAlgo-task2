# Parallel BFS Benchmark

start with command: cmake -B build && cmake --build build && ./build/secondTask

## Results

### Correctness Test

**Correctness test PASSED**

### Speed Test

#### Sequential BFS Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 26.2826        |
| 2   | 26.191         |
| 3   | 26.1532        |
| 4   | 26.8345        |
| 5   | 26.7982        |

**Mean Time: 26.4519 seconds**

#### Parallel BFS Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 12.7437        |
| 2   | 11.3893        |
| 3   | 11.2523        |
| 4   | 11.1613        |
| 5   | 11.359         |

**Mean Time: 11.5811 seconds**

### Performance Ratio

**Ratio: 2.28406**

### Conclusion

**Speed test Not PASSED**