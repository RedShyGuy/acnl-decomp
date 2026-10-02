#pragma once

#include "decomp.h"
#include "nn/os/os_AddressSpaceManager.h"

namespace nn {
namespace os {
class StackMemoryBlock;

namespace detail {
// set up by nn::os::Initialize
void SaveThreadLocalRegionAddress(); // 0x0011DFA4 | tier C
void InitializeSharedMemory(); // 0x0011DF90 | tier C
void InitializeStackMemory(); // 0x0011DF28 | fefates:bytes [tier B]
void InitializeThreadEnvrionment(); // 0x00128370 | nintendogs:callseq [tier A]
void InitializeFpscr(); // 0x00128384 (name is ours)
void InvokeAllTlsDestructors(); // 0x00129AC4 | fefates:bytes [tier B]
s32 ConvertSvcToLibraryPriority(s32 priority); // 0x0013E888 | nintendogs:bytes [tier A]
void FreeToSharedMemorySpace(nn::os::MemoryBlockBase*); // 0x00143224 | nintendogs:callgraph [tier A]
void FreeToMemoryBlockSpace(nn::os::MemoryBlockBase*); // 0x0034C904 | mk7dlp:callgraph [tier A]
s32 ConvertLibraryToSvcPriority(s32 priority); // 0x0034C914 | nintendogs:bytes [tier A]
uptr AllocateFromMemoryBlockSpace(nn::os::MemoryBlockBase* block, size_t size); // 0x0034C950 | mk7dlp:bytes [tier A]
uptr AllocateFromSharedMemorySpace(nn::os::MemoryBlockBase* block, size_t size); // 0x0034C96C | nintendogs:bytes [tier A]
bool IsMemoryBlockEnabled(); // 0x0034C8F4 | tier C
// to takes over the range of from (memory block space)
void Switch(nn::os::StackMemoryBlock* to, nn::os::StackMemoryBlock* from); // 0x0034C988 | tier C

// the address spaces (names are ours)
extern AddressSpaceManager s_MemoryBlockSpace;  // 0x00AE1F64, memory blocks and thread stacks
extern AddressSpaceManager s_StackSpace;        // 0x00AE1F7C, 0x0E000000 up to the main thread's stack
extern AddressSpaceManager s_SharedMemorySpace; // 0x00AF6234, 0x10000000 + 64 MB
extern bool s_IsMemoryBlockEnabled;             // 0x00975F80
extern uptr s_MainThreadLocalRegion;            // 0x00975F88
} // namespace detail
} // namespace os
} // namespace nn
