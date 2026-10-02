#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace session {
void BeginSetup(); // 0x0042A0B4 | fefates:bytes [tier B]
void Initialize(); // 0x0042A100 | fefates:bytes [tier B]
void Finalize(); // 0x0044D250 | fefates:bytes [tier B]
} // namespace session
} // namespace pia
} // namespace nn
