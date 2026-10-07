#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace local {
nn::Result BeginSetup(); // 0x00414598 | fefates:bytes [tier B]
nn::Result EndSetup(); // 0x00425DD8 (name is ours, as in common/inet)
nn::Result Initialize(); // 0x004145E4 | fefates:bytes [tier B]
void Finalize(); // 0x00425E30 | fefates:bytes [tier B]
bool IsDuringSetup(); // 0x00416624
bool IsInitialized(); // 0x00416634
} // namespace local
} // namespace pia
} // namespace nn
