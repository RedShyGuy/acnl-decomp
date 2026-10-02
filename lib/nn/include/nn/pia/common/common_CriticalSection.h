#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class CriticalSection
{
public:
    CriticalSection(); // TODO: default ctor added so derived stubs compile - may not exist
    CriticalSection(int); // 0x004273A4 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
