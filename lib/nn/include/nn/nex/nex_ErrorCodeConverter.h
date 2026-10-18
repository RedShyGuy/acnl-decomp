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
    // the network error code of a SystemError error (the first network error code for the others)
    static u32 ConvertSystemErrorToNetworkErrorCode(u32 error); // 0x00387BFC (name is ours)
};
} // namespace nex
} // namespace nn
