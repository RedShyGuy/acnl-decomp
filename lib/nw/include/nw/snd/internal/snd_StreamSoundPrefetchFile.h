#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a stream prefetch file (BCSTP; the class names are from the reference symbols, the
// members are ours): the info block of the stream sound file and the first samples of the regions to
// start from.
class StreamSoundPrefetchFile
{
public:
    static const u32 SIGNATURE_FILE = 0x50545343; // 'CSTP'
    static const u32 VERSION_MIN = 0x00010000;
    static const u32 VERSION_MAX = 0x00020200;
    static const u32 SIGNATURE_INFO_BLOCK = 0x4F464E49; // 'INFO'
    static const u32 SIGNATURE_PREFETCH_DATA_BLOCK = 0x54414450; // 'PDAT'
    static const u16 TYPE_INFO_BLOCK = 0x4000;
    static const u16 TYPE_PREFETCH_DATA_BLOCK = 0x4004;

    class FileHeader;
    class PrefetchData;
};

class StreamSoundPrefetchFile::PrefetchData
{
public:
    const void* GetPrefetchSample() const; // 0x00741350 | fefates:bytes [tier B]

    u32 startFrame;                 // 0x00
    u32 prefetchSize;               // 0x04
    u32 unknown8;                   // 0x08
    BinaryReference sampleReference; // 0x0C
};
ASSERT_SIZE(StreamSoundPrefetchFile::PrefetchData, 0x14);
} // namespace internal
} // namespace snd
} // namespace nw
