#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace util {
// the module of the result codes of pia (those in common_Result.h; name is ours)
const bit32 RESULT_MODULE_PIA = 82;

// the result is one of pia (its module)
bool IsPiaResult(const nn::Result& result); // 0x00413978 | fefates:bytes [tier B]
} // namespace util
} // namespace pia
} // namespace nn
