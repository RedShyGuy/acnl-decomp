#pragma once

#include "decomp.h"

namespace nn {
namespace os {
namespace CTR {
// runs on a New Nintendo 3DS (asked once)
bool IsRunOnSnake(); // 0x0011D4C8 | fefates:bytes [tier B]
// the per thread state of the ARM C++ runtime's exception handling, in the thread local region
void SetupThreadCppExceptionEnvironment(); // 0x00123D64 | nintendogs:bytes [tier A]
bool IsRunningAsExtApplication(); // 0x0034C750 | fefates:bytes [tier B]
uptr GetWramAddress(); // 0x00144B70 | tier C
size_t GetWramSize(); // 0x00144B60 | tier C
// there is WRAM for the process (symbols.json names 0x0014316C nn::ac::CTR::IsInitialized, but the
// function reads GetWramSize's variable; the name is ours)
bool IsWramEnabled(); // 0x0014316C
} // namespace CTR
} // namespace os
} // namespace nn
