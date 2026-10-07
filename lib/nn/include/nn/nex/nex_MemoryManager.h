#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The memory of nex: every block has an 8 byte header in front of it whose first word is the
// function that frees it (null: free).
class MemoryManager
{
public:
    static void* Allocate(u32 size); // 0x00361FA0 | mk7dlp:callgraph [tier A]
    // (name is ours)
    static void Free(void* p); // 0x00361F78
};
} // namespace nex
} // namespace nn
