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
};
} // namespace nex
} // namespace nn
