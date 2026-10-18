#pragma once

#include "decomp.h"
#include "nn/nex/nex_CompressionAlgorithm.h"
#include "nn/nex/nex_ZLib.h"

namespace nn {
namespace nex {
// The zlib compression: the first byte of a compressed payload is the ratio of the sizes (rounded
// up; 0: not compressed, 255: at least 255). The names of the members and constants are ours.
// RTTI N2nn3nex15ZLibCompressionE @ 0x008CE538
// vtable 0x008FD06C (vptr 0x008FD074), offset_to_top 0, 4 entries
class ZLibCompression : public ::nn::nex::CompressionAlgorithm
{
public:
    // the defaults of the parameters (-1)
    static const int DEFAULT_WINDOW_BITS = 15;
    static const int DEFAULT_MEMORY_LEVEL = 8;
    // the ratio byte of an uncompressed payload / of a ratio of 255 or more
    static const u8 RATIO_UNCOMPRESSED = 0;
    static const u8 RATIO_MAX = 255;
    // the size of a payload with the ratio 255 is at most this many times the compressed size
    static const u32 MAX_RATIO_FACTOR = 0x41A;

    // the streams of zlib
    struct Streams
    {
        z_stream m_Inflate; // 0x00
        z_stream m_Deflate; // 0x38
    };

    ZLibCompression(); // 0x0037EC3C (name is ours)
    ZLibCompression(int windowBits, int memoryLevel); // 0x0037EB10 | fefates:bytes-fuzzy [tier B]
    virtual ~ZLibCompression(); // 0x0037EE08 slot 0x00
    // 0x0037ED54 slot 0x04 (deleting dtor)
    virtual int CompressImpl(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x0037E804 slot 0x08 | slot vf_0x08 of nn::nex::CompressionAlgorithm
    virtual int DecompressImpl(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x0037E974 slot 0x0C | fefates:bytes-fuzzy

    // (inline; the name is ours)
    DECOMP_ALWAYS_INLINE void InitializeStreams(int windowBits, int memoryLevel)
    {
        m_pStreams = new (MemoryManager::Allocate(sizeof(Streams))) Streams();
        m_pStreams->m_Inflate.zalloc = zlib::Allocate;
        m_pStreams->m_Inflate.zfree = zlib::Free;
        m_pStreams->m_Inflate.opaque = nullptr;
        zlib::inflateInit_(&m_pStreams->m_Inflate, "1.2.5", sizeof(z_stream));
        m_pStreams->m_Deflate.zalloc = zlib::Allocate;
        m_pStreams->m_Deflate.zfree = zlib::Free;
        m_pStreams->m_Deflate.opaque = nullptr;
        zlib::deflateInit2_(&m_pStreams->m_Deflate, -1, Z_DEFLATED, windowBits, memoryLevel, Z_DEFAULT_STRATEGY, "1.2.5", sizeof(z_stream));
    }

    Streams* m_pStreams; // 0x118
};
ASSERT_SIZE(ZLibCompression::Streams, 0x70);
ASSERT_SIZE(ZLibCompression, 0x120);
} // namespace nex
} // namespace nn
