#pragma once

#include "decomp.h"

namespace nn {
namespace fslow {

// A path as the FS service takes it (3dbrew "FS Path"): type, data, size in bytes. The template
// arguments are the character types of the text paths (const char*, const wchar_t*). The name is
// from the symbols, the member names are ours.
template <typename T0, typename T1>
struct LowPath {
    u32 type;           // 0x0, 3dbrew: 1 empty, 2 binary, 3 ASCII, 4 UTF-16
    const u8* data;     // 0x4
    size_t size;        // 0x8
};

} // namespace fslow
} // namespace nn
