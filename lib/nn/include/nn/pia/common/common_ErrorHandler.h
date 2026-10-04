#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
// The class and TraceResult are from the fefates symbols; the rest is ours.
class ErrorHandler
{
public:
    // the trace flag CallContext passes (2)
    static const u64 TRACE_FLAG_CALL_CONTEXT = 2;

    // the trace output is not in the release build; what is left returns the module bits of the
    // result (value & 0x3FC00)
    static bit32 TraceResult(unsigned long long flag, const nn::Result& result); // 0x00426CE8 | fefates:callgraph [tier C]
};
} // namespace common
} // namespace pia
} // namespace nn
