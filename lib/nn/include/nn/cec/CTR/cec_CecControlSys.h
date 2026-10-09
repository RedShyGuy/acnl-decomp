#pragma once

#include "decomp.h"

namespace nn {
namespace cec {
namespace CTR {
// The cec control of system programs (only IsInitializedSys is in this program; its flag is never
// set here).
class CecControlSys
{
public:
    static bool IsInitializedSys(); // 0x00143684 | nintendogs:callgraph [tier A]
};
} // namespace CTR
} // namespace cec
} // namespace nn
