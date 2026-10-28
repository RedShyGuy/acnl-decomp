#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_Tick.h"

namespace nn {
namespace os {
// heap at 0x08000000; size in whole pages
size_t GetHeapSize(); // 0x0011D2A8 (name is ours)
nn::Result SetHeapSize(size_t size); // 0x0011D2B8 | nintendogs:bytes [tier A]
// the memory of the application (the configuration memory says how much)
size_t GetAppMemorySize(); // 0x0011D350 (name is ours, after 3dbrew's APPMEMALLOC)
// memory the process committed (resource limit "commit")
size_t GetUsingMemorySize(); // 0x0011D360 | nintendogs:bytes [tier A]
// linear (device) memory; once there is some, it grows / shrinks in steps of 1 MB
nn::Result SetDeviceMemorySize(size_t size); // 0x0011D3B4 | nintendogs:bytes [tier A]
void Initialize(); // 0x0011DF04 | fefates:bytes [tier B]
size_t GetDeviceMemorySize(); // 0x0013E878 | nintendogs:callgraph [tier A]
uptr GetDeviceMemoryAddress(); // 0x00140690 | nintendogs:callgraph [tier A]
nn::os::Tick GetCreationTime(); // 0x0034C074 | fefates:bytes [tier B]
// gives up the rest of the time slice (a sleep of 0; sead::ThreadMgr waits with it; name is ours)
void YieldThread(); // 0x00128430
} // namespace os
} // namespace nn

// C helpers next to nnosTickGetSystemCurrent (the names are ours): sleep the current thread for the
// nanoseconds, and give up the rest of its time slice (a sleep of 0)
extern "C" void OsSleepThread(s64 nanoSeconds); // 0x007B2C24 (name is ours)
extern "C" void OsYieldThread(); // 0x007B2C28 (name is ours)
