#pragma once

// Types of nn::fs that the IPC layer and the archives pass around. The type names are from the
// symbols (parameter types), the layouts and values after 3dbrew ("Filesystem services"), the
// member and enumerator names are ours.

#include "decomp.h"

namespace nn {
namespace fs {

// the first parameter of the file / directory commands of FS:USER (3dbrew: "usually 0")
enum Transaction : u8 {
    TRANSACTION_NONE = 0,
};

enum MediaType : u8 {
    MEDIA_TYPE_NAND = 0,
    MEDIA_TYPE_SDMC = 1,
    MEDIA_TYPE_GAME_CARD = 2,
};

enum SystemMediaType : u8 {
    SYSTEM_MEDIA_TYPE_CTR_NAND = 0,
    SYSTEM_MEDIA_TYPE_TWL_NAND = 1,
    SYSTEM_MEDIA_TYPE_SDMC = 2,
    SYSTEM_MEDIA_TYPE_TWL_PHOTO = 3,
};

// passed by value (one word)
struct Attributes {
    bool isDirectory;   // 0x0
    bool isHidden;      // 0x1
    bool isArchive;     // 0x2
    bool isReadOnly;    // 0x3
};
ASSERT_SIZE(Attributes, 0x4);

// passed by value (one word). 3dbrew: byte 0 flush, byte 1 "update time stamp", 2 and 3 reserved;
// FileServerArchive::File sets 5 fields (byte 2 to the flush flag as well, byte 1 and byte 3
// to 0, byte 3 as a 1 bit and a 7 bit field), so only isFlush is named.
struct WriteOption {
    bool isFlush;       // 0x0
    bool unknown1;      // 0x1
    bool unknown2;      // 0x2, set like isFlush
    u8 unknown3 : 1;    // 0x3
    u8 reserved : 7;
};
ASSERT_SIZE(WriteOption, 0x4);

struct DirectoryEntry {
    wchar_t name[0x106];        // 0x000, UTF-16 (wchar_t is 16 bit, -fshort-wchar)
    char shortName[0xA];        // 0x20C, 8.3 name
    char shortExtension[0x4];   // 0x216
    u8 unknown21A;              // 0x21A, 1 from the FS service, 0 from RomFsArchive
    u8 reserved21B;             // 0x21B
    Attributes attributes;      // 0x21C
    s64 size;                   // 0x220
};
ASSERT_SIZE(DirectoryEntry, 0x228);

struct ArchiveResource {
    u32 sectorSize;             // 0x0, bytes
    u32 clusterSize;            // 0x4, bytes
    u32 partitionClusters;      // 0x8
    u32 freeClusters;           // 0xC
};
ASSERT_SIZE(ArchiveResource, 0x10);

// what a seek counts from (FileBase::TrySeek; the names are ours)
enum PositionBase : u8 {
    POSITION_BASE_BEGIN = 0,
    POSITION_BASE_CURRENT = 1,
    POSITION_BASE_END = 2,
};

// how a file is opened (bits; the names are ours)
const u32 OPEN_MODE_READ = 1 << 0;
const u32 OPEN_MODE_WRITE = 1 << 1;

// ARMCC lays out some s64 members on 4 bytes (detail::FileBase: +0x04, +0x0C); a typedef may
// lower the alignment with GCC (name is ours)
typedef s64 s64_align4 __attribute__((aligned(4)));

} // namespace fs
} // namespace nn
