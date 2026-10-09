#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
// The extra pad (Circle Pad Pro, connected by ir).
class ExtraPad
{
public:
    // the extra pad is connected and sampling
    static bool IsSampling(); // 0x00354680 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace hid
} // namespace nn
