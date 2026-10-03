#pragma once

#include "decomp.h"

namespace nn {
namespace pl {
namespace CTR {
// pedometer (ptm:u); a failure of the service stops the program
void GetStepHistory(u16* steps, u32 hours, s64 start); // 0x00123DC8 (name is ours, after PTM:GetStepHistory)
u32 GetTotalStepCount(); // 0x00123DE4 | nintendogs:bytes [tier B]

// the system font in shared memory, 0 before it is mapped
u32 GetSharedFontSize(); // 0x00143234 | fefates:bytes [tier B]
void* GetSharedFontAddress(); // 0x0014324C | fefates:bytes [tier B]
} // namespace CTR
} // namespace pl
} // namespace nn
