#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
void BeginSetup(); // 0x0044D32C | fefates:bytes [tier B]
void Initialize(); // 0x0044D378 | fefates:bytes [tier B]
void Conv2StationIndex(nn::pia::StationId); // 0x00454150 | fefates:bytes [tier B]
void EndSetup(); // 0x0045F8FC | fefates:bytes [tier B]
void Finalize(); // 0x0045F958 | fefates:bytes [tier B]
} // namespace transport
} // namespace pia
} // namespace nn
