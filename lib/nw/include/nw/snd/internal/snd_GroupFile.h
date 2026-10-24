#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a group file (BCGRP; the class names are from the reference symbols, the members are
// ours): the files of the group embedded in the file block, and optionally the items of the group.
class GroupFile
{
public:
    static const u32 SIGNATURE_FILE = 0x50524743; // 'CGRP'
    static const u32 VERSION_MIN = 0x01000000;
    static const u32 VERSION_MAX = 0x01010000;
    static const u16 TYPE_INFO_BLOCK = 0x7800;
    static const u16 TYPE_FILE_BLOCK = 0x7801;
    static const u16 TYPE_INFO_EX_BLOCK = 0x7802;

    class FileHeader;

    // the info block: a BinaryReferenceTable of these
    struct GroupItemInfo
    {
        u32 fileId;                           // 0x0
        BinaryReferenceWithSize embeddedItem; // 0x4 the file in the file block (offset -1: not embedded)
    };
    // the info ex block: a BinaryReferenceTable of these (names are ours)
    struct GroupItemInfoEx
    {
        u32 itemId;   // 0x0
        u32 loadFlag; // 0x4
    };
    ASSERT_SIZE(GroupItemInfoEx, 0x8);
};

// where a file of a group is (the struct name is from the reference symbols, the members are ours)
struct GroupItemLocationInfo
{
    u32 fileId;           // 0x0
    const void* address;  // 0x4 (NULL: not embedded)
};
ASSERT_SIZE(GroupItemLocationInfo, 0x8);
} // namespace internal
} // namespace snd
} // namespace nw
