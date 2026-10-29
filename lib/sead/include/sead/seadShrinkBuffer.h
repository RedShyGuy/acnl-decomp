#pragma once

#include "decomp.h"

namespace sead {
class Heap;

// A buffer of words from a heap that is shrunk to the words written: allocate takes size bytes (0: the
// largest block the heap has), the user writes (the game: GPU commands), shrink gives the rest back.
// No RTTI; the class and all names are ours.
class ShrinkBuffer
{
public:
    // heap NULL: the current heap of the thread
    void allocate(Heap* heap, u32 size); // 0x00549FD4 (name is ours)
    // keeps the words up to end and returns their number
    s32 shrink(Heap* heap, u32* end); // 0x0054A03C (name is ours)

    u32* mBuffer;  // 0x0
    s32 mWordNum;  // 0x4, set by shrink
};
ASSERT_SIZE(ShrinkBuffer, 0x8);
} // namespace sead
