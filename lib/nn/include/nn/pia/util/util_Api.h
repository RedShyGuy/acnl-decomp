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

// for the game: two classifications of a result (0 if it is not one of pia) and the error code
// to show (180000 + n for those of pia); the names are ours
s32 GetResultKind(const nn::Result& result);     // 0x00413990 (name is ours)
s32 GetResultCategory(const nn::Result& result); // 0x00413A68 (name is ours)
u32 GetErrorCode(const nn::Result& result);      // 0x00413DC8 (name is ours)
} // namespace util
} // namespace pia
} // namespace nn
