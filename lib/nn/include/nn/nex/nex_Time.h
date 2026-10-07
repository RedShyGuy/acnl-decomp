#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Time
{
public:
    void GetTime(); // 0x0035BF84 | mk7dlp:callgraph [tier A]
    // the time timeout milliseconds from now
    static Time ConvertTimeoutToDeadline(unsigned timeout); // 0x003CE0B8 | mk7dlp:callseq-callee [tier A]

    // (the layout is from pia::inet::NatTraverser; the member name is ours)
    u64 m_Value; // 0x0
};
ASSERT_SIZE(Time, 0x8);
} // namespace nex
} // namespace nn
