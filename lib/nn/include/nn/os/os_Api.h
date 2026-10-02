#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_Tick.h"

namespace nn {
namespace os {
// heap at 0x08000000; size in whole pages
nn::Result SetHeapSize(size_t size); // 0x0011D2B8 | nintendogs:bytes [tier A]
// memory the process committed (resource limit "commit")
size_t GetUsingMemorySize(); // 0x0011D360 | nintendogs:bytes [tier A]
// linear (device) memory; once there is some, it grows / shrinks in steps of 1 MB
nn::Result SetDeviceMemorySize(size_t size); // 0x0011D3B4 | nintendogs:bytes [tier A]
void Initialize(); // 0x0011DF04 | fefates:bytes [tier B]
size_t GetDeviceMemorySize(); // 0x0013E878 | nintendogs:callgraph [tier A]
uptr GetDeviceMemoryAddress(); // 0x00140690 | nintendogs:callgraph [tier A]
nn::os::Tick GetCreationTime(); // 0x0034C074 | fefates:bytes [tier B]
} // namespace os
} // namespace nn
