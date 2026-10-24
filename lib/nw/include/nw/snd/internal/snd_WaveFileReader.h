#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveFile.h"
#include "nw/snd/internal/snd_WaveFile_FileHeader.h"
#include "nw/snd/snd_Global.h"

namespace nw {
namespace snd {
namespace internal {
// what a wave file describes (the struct name is from the reference symbols, the members are ours)
struct WaveInfo
{
    static const int CHANNEL_MAX = 2;

    struct ChannelParam
    {
        const void* pDataAddress;          // 0x00
        DspAdpcmParam adpcmParam;          // 0x04
        DspAdpcmLoopParam adpcmLoopParam;  // 0x2A
    };

    SampleFormat sampleFormat;          // 0x00
    bool isLoop;                        // 0x01
    u32 channelCount;                   // 0x04
    u32 sampleRate;                     // 0x08
    u32 loopStartFrame;                 // 0x0C
    u32 loopEndFrame;                   // 0x10
    u32 originalLoopStartFrame;         // 0x14
    ChannelParam channelParam[CHANNEL_MAX]; // 0x18
};
ASSERT_SIZE(WaveInfo, 0x78);

// Reads the info of a wave file (the class and function names are from the reference symbols, the
// members are ours).
class WaveFileReader
{
public:
    // a reader of a wave file (another wave type than 0: nothing is read)
    WaveFileReader(const void* pWaveFile, s8 waveType); // 0x004C8200 | fefates:bytes [tier B]

    // the sample format of an encoding of the file
    static SampleFormat GetSampleFormat(u8 encoding); // 0x004C81EC | nintendogs:bytes [tier A]
    bool ReadWaveInfo(WaveInfo* pInfo, const void* pWaveDataAddress) const; // 0x0073F86C | nintendogs:callseq [tier A]

    const WaveFile::FileHeader* m_pHeader;          // 0x0
    const WaveFile::InfoBlockBody* m_pInfoBlockBody; // 0x4
    const void* m_pDataBlockBody;                   // 0x8
    s8 m_WaveType;                                  // 0xC
};
ASSERT_SIZE(WaveFileReader, 0x10);
} // namespace internal
} // namespace snd
} // namespace nw
