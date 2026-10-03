#pragma once

#include "decomp.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
namespace CTR {

// The file path (binary, 12 bytes) of the program's own image in archive 3 (3dbrew "Filesystem
// services"). The struct name is from the symbols, the members are ours: type 0 is the RomFS,
// "patch:" opens type 5.
struct ProgramDataPath
{
    u32 type;
    u32 reserved[2];
};
ASSERT_SIZE(ProgramDataPath, 0xC);

// A content of a title, opened through archive 0x2345678A. The struct name is from the symbols,
// the members are ours.
struct DataContentArchivePath
{
    u64 programId;          // 0x0
    u32 mediaType;          // 0x8 a MediaType (stored as a word)
    u32 contentIndex;       // 0xC
};
ASSERT_SIZE(DataContentArchivePath, 0x10);

} // namespace CTR
} // namespace fs
} // namespace nn
