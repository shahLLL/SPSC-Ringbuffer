#pragma once
#include <cstddef>
#include <atomic>
#include <new>

// Aliases
using SizeT = std::size_t;

template<typename T, SizeT exponent>
class SPSCRingBuffer{
    alignas(std::hardware_destructive_interference_size) std::atomic<SizeT> pushCursor{0};
    alignas(std::hardware_destructive_interference_size) SizeT cachedPushCursor = pushCursor;
    alignas(std::hardware_destructive_interference_size) std::atomic<SizeT> popCursor{0};
    alignas(std::hardware_destructive_interference_size) SizeT cachedPopCursor = popCursor;
     
    const static constexpr capacity = SizeT{1} << exponent; // Capacity must be power of 2 to enusre efficent increment.
    const static constexpr SizeT mask = capacity - 1;
    T buf[capacity];

    public:
        bool push(const T& addVal) {
            SizeT pushCursorSpot = pushCursor.load(std::memory_order_relaxed);
            SizeT incrementOne = (pushCursorSpot + 1) & (mask);

            if(cachedPopCursor == incrementOne) { 
                cachedPopCursor = popCursor.load(std::memory_order_acquire);
                // Full
                if(cachedPopCursor == incrementOne) return false;
            } 
            buf[incrementOne] = addVal;
            pushCursor.store(incrementOne, std::memory_order_release);
            return true;
        };

        bool pop(T& popedVal) {
            SizeT popCursorSpot = popCursor.load(std::memory_order_relaxed);
            if(cachedPushCursor == popCursorSpot) { 
                cachedPushCursor = pushCursor.load(std::memory_order_acquire);
                // Empty
                if(cachedPushCursor == popCursorSpot) return false;
            } 
            popedVal = buf[popCursorSpot];
            popCursor.store((popCursorSpot + 1) & (mask), std::memory_order_release);
            return true;
        };
};