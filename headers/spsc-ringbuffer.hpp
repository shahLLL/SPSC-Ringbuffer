#pragma once
#include <cstddef>
#include <atomic>

// Aliases
using SizeT = std::size_t;

template<typename T, SizeT powerOf2>
class SPSCRingBuffer{
    std::atomic<SizeT> pushCursor{0};
    std::atomic<SizeT> popCursor{0};
    SizeT capacity = 2**powerOf2; // Capacity must be power of 2 to enusre efficent increment.
    static constexpr SizeT mask = capacity - 1;
    T buf[capacity];

    public:
        bool push(T& addval) {
            SizeT pushCursorSpot = pushCursor.load(std::memory_order_relaxed);
            SizeT incrementOne = (pushCursorSpot + 1) & (mask);
            if(incrementOne == popCursor.load(std::memory_order_acquire)) { return false; } // Full
            buf[incrementOne] = addVal;
            pushCursor.store(incrementOne, std::memory_order_release);
            return true;
        };

        bool pop(T& popedVal) {
            SizeT popCursorSpot = popCursor.load(std::memory_order_relaxed);
            if(pushCursor.load(std::memory_order_acquire) == popCursorSpot) { return false; } // Empty
            popedVal = buf[popCursorSpot];
            popCursor.store((popCursorSpot + 1) & (mask), std::memory_order_release);
            return true;
        };
};