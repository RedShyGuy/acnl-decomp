#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class Watermark
{
public:
    void SetName(const char*); // 0x00151168 | fefates:bytes [tier B]
    void Update(long long); // 0x0042A048 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
