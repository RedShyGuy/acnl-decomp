#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class qResult
{
public:
    qResult(const int&); // 0x003D4010 | mk7dlp:callgraph [tier A]
    qResult(); // 0x003D4030 | mk7dlp:callseq-callee [tier A]
    void operator =(const nn::nex::qResult&); // 0x003D4054 | mk7dlp:callgraph [tier A]
    void Equals(const bool&) const; // 0x0072E5FC | mk7dlp:bytes [tier A]
    // true if the code is not negative (no failure)
    operator bool() const; // 0x0072E634

    // (the member names are ours)
    s32 m_Code;        // 0x0
    u32 m_Unknown0x4;  // 0x4
    u32 m_Unknown0x8;  // 0x8
};
ASSERT_SIZE(qResult, 0xC);
} // namespace nex
} // namespace nn
