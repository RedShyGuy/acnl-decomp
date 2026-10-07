#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace session {
nn::Result BeginSetup(); // 0x0042A0B4 | fefates:bytes [tier B]
// (names are ours, like the ones of transport)
nn::Result EndSetup(); // 0x0044D1F8
nn::Result Initialize(); // 0x0042A100 | fefates:bytes [tier B]
void Finalize(); // 0x0044D250 | fefates:bytes [tier B]
bool IsInSetupMode(); // 0x004310D0
bool IsInitialized(); // 0x004310E0
} // namespace session
} // namespace pia
} // namespace nn
