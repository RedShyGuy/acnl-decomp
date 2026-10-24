#pragma once

#include "decomp.h"
#include "nw/snd/snd_Global.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
// What a stream sound player needs of a stream (the class and SetStreamSoundInfo are from the
// reference symbols, the members are ours). Followed at 0x34 by the tracks (13 bytes each), whose
// count is not known yet.
class StreamDataInfoDetail
{
public:
    void SetStreamSoundInfo(const StreamSoundFile::StreamSoundInfo& info); // 0x004C9304 | fefates:bytes [tier B]

    SampleFormat sampleFormat;  // 0x00
    u32 sampleRate;             // 0x04
    bool isLoop;                // 0x08
    u32 loopStart;              // 0x0C
    u32 frameCount;             // 0x10
    u32 originalLoopStart;      // 0x14
    u32 originalLoopEnd;        // 0x18
    u32 blockSampleCount;       // 0x1C
    u32 blockSize;              // 0x20
    u32 lastBlockSize;          // 0x24 the padded size
    u32 lastBlockSampleCount;   // 0x28
    u32 unknown2C;              // 0x2C
    u32 unknown30;              // 0x30
};
} // namespace internal
} // namespace snd
} // namespace nw
