#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"
#include "nw/snd/internal/snd_StreamSoundFile_FileHeader.h"
#include "nw/snd/internal/snd_StreamSoundFile_InfoBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
// Reads the info block of a stream sound file (the class and function names are from the reference
// symbols, the members are ours).
class StreamSoundFileReader
{
public:
    // a track (names are ours)
    struct TrackInfo
    {
        static const u8 CHANNEL_COUNT_MAX = 2;

        u8 volume;                                  // 0x0
        u8 pan;                                     // 0x1
        u8 unknown2;                                // 0x2
        u8 unknown3;                                // 0x3
        u8 channelCount;                            // 0x4
        u8 globalChannelIndex[CHANNEL_COUNT_MAX];   // 0x5
    };
    ASSERT_SIZE(TrackInfo, 0x7);

    StreamSoundFileReader(); // 0x004C97FC | fefates:bytes [tier B]

    void Initialize(const void* pStreamSoundFile); // 0x004C97A8 | fefates:bytes [tier B]
    // the signature and the version
    bool IsValidFileHeader(const void* pStreamSoundFile) const; // 0x00740A60 | fefates:bytes [tier B]
    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* pInfo) const; // 0x00740A98 | fefates:bytes [tier B]
    // false if there is no track table or index is beyond it
    bool ReadStreamTrackInfo(TrackInfo* pInfo, int index) const; // 0x00740B80 | fefates:bytes [tier B]
    bool IsTrackInfoAvailable() const; // 0x00740C24 | fefates:bytes [tier B]
    // false if the channel has no DSP ADPCM state
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam, int channelIndex) const; // 0x00740C44 | fefates:bytes [tier B]
    // (inline; name is ours) the offset of the samples in the file (0: no file)
    u32 GetSampleDataOffset() const
    {
        if (m_pHeader == nullptr) {
            return 0;
        }
        return m_pInfoBlockBody->GetStreamSoundInfo()->sampleDataReference.offset + (m_pHeader->GetDataBlockOffset() + 8);
    }

    const StreamSoundFile::FileHeader* m_pHeader;          // 0x0
    const StreamSoundFile::InfoBlockBody* m_pInfoBlockBody; // 0x4
};
ASSERT_SIZE(StreamSoundFileReader, 0x8);
} // namespace internal
} // namespace snd
} // namespace nw
