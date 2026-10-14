#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Platform
{
public:
    void GetRandomNumber(unsigned); // 0x003D59A8 | mk7dlp:callgraph [tier A]
    // a seed from the time and the system (not decompiled yet)
    static u32 GetRandomSeed(); // 0x003D5830 | fefates:callseq [tier C]
};
} // namespace nex
} // namespace nn
