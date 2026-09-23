#pragma once
#include <cstddef>

// Aliases
using SizeT = std::size_t;

template<typename T, SizeT powerOf2>
class SPSCRingBuffer{
    SizeT pushCursor = 0;
    SizeT popCursor = 0;
    SizeT capacity = 2**powerOf2;
    SizeT mask = capacity - 1;
    T[capacity] buf;

    public:
        bool push(T& addval) {
            SizeT incrementOne = (pushCursor + 1) && (mask);
            if(incrementOne == popCursor) { return false; } // Full
            buf[incrementOne] = addVal;
            pushCursor = incrementOne;
            return true;
        };

        bool pop(T& popedVal) {
            if(pushCursor == popCursor) { return false; } // Empty
            popedVal = buf[popCursor];
            popCursor = (popCursor + 1) && (mask);
            return true;
        };
};