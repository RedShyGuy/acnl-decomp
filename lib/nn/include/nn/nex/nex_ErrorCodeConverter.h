#pragma once

#include "decomp.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// (no RTTI; the class name is from the fefates symbols)
class ErrorCodeConverter
{
public:
    // the network error code of a result (the number for the user)
    static u32 ConvertToNetworkErrorCode(const qResult& result); // 0x003879A4 | fefates:callgraph [tier C]
};
} // namespace nex
} // namespace nn
