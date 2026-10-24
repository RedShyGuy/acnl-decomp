#pragma once

#include "decomp.h"
#include "nw/snd/snd_Global.h"
#include "nw/snd/internal/snd_BinaryTypes.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a stream sound file (BCSTM after 3dbrew "BCSTM"; the class names are from the reference
// symbols, the members are ours): the info block with the stream, track and channel infos, the seek
// block with the ADPCM history per block and channel, the data block and the region block, which
// 3dbrew does not list.
class StreamSoundFile
{
public:
    static const u32 SIGNATURE_FILE = 0x4D545343; // 'CSTM'
    static const u32 VERSION_MIN = 0x02000000;
    static const u32 VERSION_MAX = 0x02030100;
    // up to this version the files have the track infos
    static const u32 VERSION_TRACK_INFO = 0x02010000;
    // from this version on the stream info has the original loop
    static const u32 VERSION_ORIGINAL_LOOP = 0x02030000;

    static const u16 TYPE_INFO_BLOCK = 0x4000;
    static const u16 TYPE_SEEK_BLOCK = 0x4001;
    static const u16 TYPE_DATA_BLOCK = 0x4002;
    static const u16 TYPE_REGION_BLOCK = 0x4003;
    static const u16 TYPE_STREAM_SOUND_INFO = 0x4100;
    static const u16 TYPE_TRACK_INFO = 0x4101;
    static const u16 TYPE_CHANNEL_INFO = 0x4102;
    static const u16 TYPE_REFERENCE_TABLE = 0x0101;
    static const u16 TYPE_DSP_ADPCM_INFO = 0x0300;

    class FileHeader;
    class InfoBlockBody;
    class TrackInfoTable;
    class ChannelInfoTable;
    class ChannelInfo;

    // the stream (3dbrew "BCSTM": Stream Info)
    struct StreamSoundInfo
    {
        u8 encoding;                    // 0x00
        u8 isLoop;                      // 0x01 (0 or 1)
        u8 channelCount;                // 0x02
        u8 unknown3;                    // 0x03 (3dbrew: padding; copied)
        u32 sampleRate;                 // 0x04
        u32 loopStart;                  // 0x08
        u32 frameCount;                 // 0x0C the loop end
        u32 blockCount;                 // 0x10
        u32 blockSize;                  // 0x14
        u32 blockSampleCount;           // 0x18
        u32 lastBlockSize;              // 0x1C
        u32 lastBlockSampleCount;       // 0x20
        u32 lastBlockPaddedSize;        // 0x24
        u32 seekSize;                   // 0x28
        u32 seekInterval;               // 0x2C
        BinaryReference sampleDataReference; // 0x30
        u16 regionInfoSize;             // 0x38 the size of a region info
        u16 padding;                    // 0x3A
        BinaryReference regionDataReference; // 0x3C the region infos in the region block
        u32 originalLoopStart;          // 0x44 (from VERSION_ORIGINAL_LOOP on)
        u32 originalLoopEnd;            // 0x48
    };
    ASSERT_SIZE(StreamSoundInfo, 0x4C);

    // a track (3dbrew "BCSTM": Track Info)
    struct TrackInfo
    {
        u8 volume;                          // 0x0
        u8 pan;                             // 0x1
        u8 unknown2;                        // 0x2 (3dbrew: padding; copied)
        u8 unknown3;                        // 0x3
        BinaryReference globalChannelIndexTable; // 0x4 BinaryTable<u8>
    };

    // the DSP ADPCM state of a channel (3dbrew "BCSTM": DSP ADPCM Info)
    struct DspAdpcmChannelInfo
    {
        DspAdpcmParam param;            // 0x00
        DspAdpcmLoopParam loopParam;    // 0x26
    };
    ASSERT_SIZE(DspAdpcmChannelInfo, 0x2C);

    // a region of the stream: the frames and the ADPCM state at its start (0x100 bytes; the channel
    // count of the array is ours)
    struct RegionInfo
    {
        u32 start;                          // 0x00
        u32 end;                            // 0x04
        DspAdpcmLoopParam adpcmContext[16]; // 0x08
        u8 padding[0x98];                   // 0x68
    };
    ASSERT_SIZE(RegionInfo, 0x100);
};
} // namespace internal
} // namespace snd
} // namespace nw
