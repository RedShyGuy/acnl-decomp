#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace session {
class SyncClock
{
public:
    void SetBaseTime(const nn::pia::common::Time&); // 0x0044D2D0 | fefates:bytes [tier B]
    void ClearBaseTime(); // 0x0044D2EC | fefates:bytes [tier B]
    SyncClock(); // 0x0044D304 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
