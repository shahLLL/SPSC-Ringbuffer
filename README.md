# ⚪️ SPSC-Ringbuffer

<div align="center">
  <img src="images/background.jpg" alt="Background" width="75%"/>
  <br><br>
</div>

# 👀 Overview
This is a C++ implementation of a Lock-Free Single-Producer, Single-Consumer Ringbuffer.
This codebase has been written using C++23 and uses:
- **[CMake](https://cmake.org/)**: Build Tool
- **[Google Benchmark](https://github.com/google/benchmark)**: Benchmarking Framework

The following optimisations and tecniques have been used for this Ring Buffer:

- **Atomics**: Used to prevent data races and provide lock-free effecient behavior.
- **Bit Operations**: Combined with an array limited in size to an exponent of 2, allows for effecient cursor increment
- **Alignas Specifier**: Used to pad cursors, preventing false sharing.
- **Cached Cursors**: Optimises execution by reducing the amount of load operations performed.

# 🛠️ Usage & Build
In order to build this project run the following commands sequentially:
```
cmake -B build
cmake --build build
```

Two executables as a result will be genrated: **bench** and **bench-debug**. Bench is the more effecient build,
used to gather true benchmark metrics, while bench-debug is run using thread sanitizer to validate no data races.

The following sample runs were produced locally on a standard MacBook Air with an [Apple M4](https://en.wikipedia.org/wiki/Apple_M4) memory chip and 16GB of memory, compiled with **AppleClang 17.0.0.17000013**

## Bench Sample Run:
```
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
benchMain<10>      0.000 ms        0.000 ms    318755578 items_per_second=445.827M/s ops/sec=445.827M/s
benchMain<14>      0.000 ms        0.000 ms     20993471 items_per_second=29.067M/s ops/sec=29.067M/s
benchMain<17>      0.000 ms        0.000 ms     20412091 items_per_second=29.2767M/s ops/sec=29.2767M/s
```
## Bench Debug Sample Run:
```
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
benchMain<10>      0.001 ms        0.001 ms      1044168 items_per_second=1.68813M/s ops/sec=1.68813M/s
benchMain<14>      0.001 ms        0.001 ms      1000000 items_per_second=1.87462M/s ops/sec=1.87462M/s
benchMain<17>      0.001 ms        0.001 ms      1187910 items_per_second=1.45382M/s ops/sec=1.45382M/s
```

# 🤝 Usage & Contribution
Suggestions, Usage, and Contributions are welcomed in this project, with adherence to the [LICENSE](./LICENSE)