# Parallel BFS Benchmark

start with command: cmake -B build && cmake --build build && ./build/secondTask

## Results

### Correctness Test

**Correctness test PASSED**

### Speed Test

#### Sequential BFS Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 26.5913        |
| 2   | 31.6346        |
| 3   | 30.254         |
| 4   | 25.8598        |
| 5   | 29.3756        |

**Mean Time: 28.743 seconds**

#### Parallel BFS Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 15.352         |
| 2   | 8.77861        |
| 3   | 9.0829         |
| 4   | 8.8251         |
| 5   | 8.77391        |

**Mean Time: 10.1625 seconds**

### Performance Ratio

**Ratio: 2.82833**

### Conclusion

**Speed test Not PASSED**