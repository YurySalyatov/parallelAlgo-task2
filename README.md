# Parallel BFS Benchmark

start with command: cmake -B build && cmake --build build && ./build/secondTask

## Results

### Correctness Test

**Correctness test PASSED**

### Speed Test

#### Sequential BFS Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 19.8038        |
| 2   | 21.9025        |
| 3   | 25.2737        |
| 4   | 25.2046        |
| 5   | 23.3971        |

**Mean Time: 23.1163 seconds**

#### Parallel BFS Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 9.63838        |
| 2   | 9.15143        |
| 3   | 9.19777        |
| 4   | 9.1316         |
| 5   | 9.20809        |

**Mean Time: 9.26545 seconds**

### Performance Ratio

**Ratio: 2.4949**

### Conclusion

**Speed test Not PASSED**