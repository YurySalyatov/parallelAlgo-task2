# Parallel QuickSort Benchmark

start with command: cmake -B build && cmake --build build && ./build/secondTask

## Results

### Correctness Test

**Correctness test PASSED**

### Speed Test

#### Sequential QuickSort Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 47.9354        |
| 2   | 50.7802        |
| 3   | 51.2079        |
| 4   | 47.8929        |
| 5   | 47.8548        |

**Mean Time: 49.1343 seconds**

#### Parallel QuickSort Times

| Run | Time (seconds) |
|-----|----------------|
| 1   | 51.375         |
| 2   | 25.0165        |
| 3   | 29.6623        |
| 4   | 16.2659        |
| 5   | 16.4615        |

**Mean Time: 27.75624 seconds**

### Performance Ratio

**Ratio: 1.77**

### Conclusion

**Speed test Not PASSED**