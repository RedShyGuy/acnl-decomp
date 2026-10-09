#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
class IAllocator;
} // namespace fnd

namespace cec {
namespace CTR {
// the memory of the library (from the allocator of SetAllocFunc; NULL without one)
void* os_malloc(size_t size, s32 alignment); // 0x00144BF0 | nintendogs:callgraph [tier A]
void os_free(void* p); // 0x001409BC | nintendogs:callgraph [tier A]
void SetAllocFunc(nn::fnd::IAllocator& allocator); // 0x0034F3F8 | nintendogs:callseq-callee [tier C]
void FinalizeAllocFunc(); // 0x0034F408 | fefates:bytes [tier B]
// the title id from its name in the message box list (8 hexadecimal digits, despite the name)
u32 Base64Str2CecTitleId(const u8* str); // 0x0034F468 | nintendogs:bytes [tier A]
// the title id of StreetPass from the unique id of a program and a variation (name is ours)
bit32 MakeCecTitleId(bit32 uniqueId, u8 variation); // 0x0034D568 (name is ours)
} // namespace CTR
} // namespace cec
} // namespace nn
