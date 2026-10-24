#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"
#include "nw/snd/internal/snd_StreamSoundFile_InfoBlockBody.h"
#include "nw/snd/internal/snd_StreamSoundPrefetchFile.h"
#include "nw/snd/internal/snd_StreamSoundPrefetchFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// Reads a stream prefetch file (the class and function names are from the reference symbols, the
// members are ours).
class StreamSoundPrefetchFileReader
{
public:
    // a prefetched region (names are ours)
    struct PrefetchDataInfo
    {
        u32 startFrame;              // 0x0
        u32 prefetchSize;            // 0x4
        const void* pPrefetchSample; // 0x8
    };
    ASSERT_SIZE(PrefetchDataInfo, 0xC);

    StreamSoundPrefetchFileReader(); // 0x004C9A90 | fefates:bytes [tier B]

    void Initialize(const void* pPrefetchFile); // 0x004C9A08 | fefates:bytes [tier B]
    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* pInfo) const; // 0x0074135C | fefates:bytes [tier B]
    bool ReadPrefetchDataInfo(PrefetchDataInfo* pInfo, int index) const; // 0x00741384 | fefates:bytes [tier B]
    bool ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam, int channelIndex) const; // 0x007413C0 | fefates:bytes [tier B]

    const StreamSoundPrefetchFile::FileHeader* m_pHeader;          // 0x0
    const StreamSoundFile::InfoBlockBody* m_pInfoBlockBody;         // 0x4
    const BinaryTable<StreamSoundPrefetchFile::PrefetchData>* m_pPrefetchDataBlockBody; // 0x8
};
ASSERT_SIZE(StreamSoundPrefetchFileReader, 0xC);
} // namespace internal
} // namespace snd
} // namespace nw
