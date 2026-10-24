#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a wave archive file (BCWAR; the class names are from the reference symbols, the
// members are ours): the info block lists the wave files in the file block by offset and size.
class WaveArchiveFile
{
public:
    static const u32 SIGNATURE_FILE = 0x52415743;            // 'CWAR'
    static const u32 SIGNATURE_INDIVIDUAL_LOAD_TABLE = 0x54415743; // 'CWAT' at the start of the file block
    static const u32 VERSION = 0x01000000;
    // the type ids of the blocks
    static const u16 TYPE_INFO_BLOCK = 0x6800;
    static const u16 TYPE_FILE_BLOCK = 0x6801;

    class FileHeader;

    // the wave files by index
    struct InfoBlockBody
    {
        u32 count;                          // 0x0
        BinaryReferenceWithSize items[1];   // 0x4 (offsets from the body of the file block)
    };
};
} // namespace internal
} // namespace snd
} // namespace nw
