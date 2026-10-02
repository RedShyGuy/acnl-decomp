#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace os {
namespace CTR {
namespace detail {
// true: also report results that are not fatal (as on development units)
void SetInternalErrorHandlingMode(bool mode); // 0x0011D534 | nintendogs:callgraph [tier A]
// the reaction of nn::os to a failed system call
void HandleInternalError(nn::Result result); // 0x001308F0

extern bool s_InternalErrorHandlingMode; // 0x00975F70 (name is ours)
} // namespace detail
} // namespace CTR
} // namespace os
} // namespace nn
