#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class SessionEndMonitoringData
{
public:
    void Serialize(unsigned char*, unsigned int*, unsigned int) const; // 0x00731C90 | fefates:bytes-fuzzy [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
