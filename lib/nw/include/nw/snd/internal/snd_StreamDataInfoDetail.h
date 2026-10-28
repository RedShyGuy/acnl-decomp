#pragma once

#include "decomp.h"
#include "nw/snd/snd_Global.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
// What a stream sound player needs of a stream (the class and SetStreamSoundInfo are from the
// reference symbols, the members are ours).
class StreamDataInfoDetail
{
public:
    static const int TRACK_MAX = 4;

    // a track: as SoundArchive::StreamTrackInfo, but without its constructor (the players do not
    // construct them; names are ours)
    struct TrackInfo
    {
        u8 volume;                // 0x00
        u8 pan;                   // 0x01
        u8 span;                  // 0x02
        u8 flags;                 // 0x03 (StreamSoundPlayer: front bypass 1, 3D surround preprocessed 2)
        u8 mainSend;              // 0x04
        u8 fxSend[2];             // 0x05
        u8 lpfFreq;               // 0x07
        u8 biquadType;            // 0x08
        u8 biquadValue;           // 0x09
        u8 channelCount;          // 0x0A
        u8 globalChannelIndex[2]; // 0x0B
    };

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
    u32 channelCount;           // 0x2C
    u32 unknown30;              // 0x30
    TrackInfo trackInfo[TRACK_MAX]; // 0x34
};
ASSERT_SIZE(StreamDataInfoDetail, 0x68);
} // namespace internal
} // namespace snd
} // namespace nw
