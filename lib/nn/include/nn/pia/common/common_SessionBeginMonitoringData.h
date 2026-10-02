#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class SessionBeginMonitoringData
{
public:
    void Serialize(unsigned char*, unsigned int*, unsigned int) const; // 0x00731D84 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
