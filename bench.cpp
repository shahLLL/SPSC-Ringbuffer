#include "headers/spsc-ringbuffer.hpp"

#include <benchmark/benchmark.h>

#include <atomic>
#include <cstdint>
#include <thread>
#include <iostream>
#include <chrono>

// Aliases
using sampleType = std::int64_t;

template <SizeT exponent>
static void benchMain(benchmark::State& state) {

    SPSCRingBuffer<sampleType, exponent> ringBuffer;

    std::atomic<bool> start{false};
    std::atomic<bool> stop{false};
    std::atomic<std::int64_t> popped{0};

    // Consumer thread
    std::thread consumer([&] {
        while (!start.load(std::memory_order_acquire)) {} // Wait until ready

        sampleType expected = 0;
        sampleType val;

        while (!stop.load(std::memory_order_acquire)) {
            if (ringBuffer.pop(val)) {
                if (val != expected) {
                    std::cerr << "CORRUPTION: expected " << expected
                              << " got " << val << std::endl;
                    std::abort();
                }
                ++expected;
                popped.fetch_add(1, std::memory_order_relaxed);
            }
        }

        // Remove remaining items after stop is signalled
        while (ringBuffer.pop(val)) {
            if (val != expected) {
                std::cerr << "CORRUPTION on drain\n";
                std::abort();
            }
            ++expected;
            popped.fetch_add(1, std::memory_order_relaxed);
        }
    });

    // Warm-up: fill the queue once so caches are hot
    constexpr SizeT Cap = SizeT{1} << exponent;
    for (sampleType i = 0; i < static_cast<sampleType>(Cap - 1); ++i) { while (!ringBuffer.push(i)) {} }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    // Timed Loop
    start.store(true, std::memory_order_release);
    sampleType seq = static_cast<sampleType>(Cap - 1);
    for (auto _ : state) {
        while (!ringBuffer.push(seq)) {} // Wait while full
        ++seq;
    }
    stop.store(true, std::memory_order_release);
    consumer.join();

    // Report results
    const auto total = popped.load(std::memory_order_relaxed);
    state.SetItemsProcessed(total);
    state.counters["ops/sec"] = benchmark::Counter(
        static_cast<double>(total),
        benchmark::Counter::kIsRate);
}

// Sample Runs
BENCHMARK(benchMain<10>)->Unit(benchmark::kMillisecond);  // 1 Ki
BENCHMARK(benchMain<14>)->Unit(benchmark::kMillisecond);  // 16 Ki
BENCHMARK(benchMain<17>)->Unit(benchmark::kMillisecond);  // 128 Ki

BENCHMARK_MAIN();