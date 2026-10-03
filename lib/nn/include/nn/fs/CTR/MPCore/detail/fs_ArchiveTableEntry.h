#pragma once

#include "decomp.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
class IArchive;

// A mounted archive (fs_UserFileSystem.cpp has a table of 32). The key is the mount name before
// ':' (up to 8 characters, one per byte). Member names are ours.
class ArchiveTableEntry
{
public:
    ArchiveTableEntry(); // 0x00347F54 | fefates:bytes [tier B]

    u64 key;            // 0x00, 0 while free
    IArchive* archive;  // 0x08
    bool unknown0C;     // 0x0C, RegisterArchive's third argument
    bool isNotOwned;    // 0x0D, Unmount does not delete the archive
};
ASSERT_SIZE(ArchiveTableEntry, 0x10);
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
