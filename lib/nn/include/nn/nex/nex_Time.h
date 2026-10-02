#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Time
{
public:
    void GetTime(); // 0x0035BF84 | mk7dlp:callgraph [tier A]
    void ConvertTimeoutToDeadline(unsigned); // 0x003CE0B8 | mk7dlp:callseq-callee [tier A]
};
} // namespace nex
} // namespace nn
