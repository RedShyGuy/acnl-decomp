#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace fnd {
class DateTime;
}

namespace ubl {
// A list of up to 1000 ids with the time they were added, kept in the shared extra save data
// 0xF000000B (file "ubll.lst"). Only IsExist has a name in the symbols; the other names are ours.

nn::Result Initialize(); // 0x00467460 (name is ours)
void Finalize(); // 0x0012A9B4 (name is ours)
// adds id (or updates its time); the oldest entry makes room if the list is full; then saves
nn::Result Add(u64 id, const nn::fnd::DateTime& date); // 0x00467950 (name is ours)
// whether id is in the list (the other arguments are not used)
bool IsExist(u64 id, u32 unknown0, u64 unknown1); // 0x00467B80 | nintendogs:bytes [tier B]
} // namespace ubl
} // namespace nn
