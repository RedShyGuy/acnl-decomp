#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
class StreamSoundFileReader;

// Reads the parts of a stream sound file from a stream (the class and function names are from the
// reference symbols, the members are ours).
class StreamSoundFileLoader
{
public:
    // the bytes read for the check of the header
    static const u32 HEADER_READ_SIZE = 96; // (name is ours)

    // reads the header and the info block into pBuffer and initializes the reader with it
    bool LoadFileHeader(StreamSoundFileReader* pReader, void* pBuffer, u32 bufferSize); // 0x004C9534 | fefates:bytes [tier B]
    // the ADPCM history of the channels at the start of a block
    bool ReadSeekBlockData(u16* pYn1, u16* pYn2, int blockIndex, int channelCount); // 0x004C96D8 | fefates:bytes [tier B]
    bool ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, u32 index) const; // 0x007409E8 | fefates:bytes [tier B]

    nw::io::FileStream* m_pStream; // 0x0
    u32 m_SeekBlockOffset;         // 0x4 (0: none)
    u32 m_RegionDataOffset;        // 0x8 (0: none)
    u16 m_RegionInfoSize;          // 0xC
};
ASSERT_SIZE(StreamSoundFileLoader, 0x10);
} // namespace internal
} // namespace snd
} // namespace nw
