#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_PluginObject.h"
#include "nn/nex/nex_ProfilingUnit.h"

namespace nn {
namespace nex {
// The compression of the payload of packets: the implementation is in CompressImpl /
// DecompressImpl, the inline wrappers count the sizes under the lock. The names of the members and
// of the wrappers are ours.
// RTTI N2nn3nex20CompressionAlgorithmE @ 0x008CE85C
// vtable 0x008FD7DC (vptr 0x008FD7E4), offset_to_top 0, 4 entries
class CompressionAlgorithm : public ::nn::nex::PluginObject
{
public:
    // (inline)
    CompressionAlgorithm()
        : m_Lock(0x20000000), m_CompressionProfiling(L"Compression", 0), m_DecompressionProfiling(L"Decompression", 0)
    {
        m_TotalInputSize = 0;
        m_TotalOutputSize = 0;
    }
    virtual ~CompressionAlgorithm(); // 0x00396270 slot 0x00
    // 0x00396218 slot 0x04 | fefates:callseq (deleting dtor)
    // 0: success
    virtual int CompressImpl(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput) = 0; // slot 0x08
    virtual int DecompressImpl(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput) = 0; // slot 0x0C

    // (inline)
    void Compress(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
    {
        ScopedCS lock(m_Lock);
        CompressImpl(input, pOutput);
        m_TotalInputSize += input.m_ContentSize;
        m_TotalOutputSize += pOutput->m_ContentSize;
    }

    u32 m_TotalInputSize;                   // 0x08
    u32 m_TotalOutputSize;                  // 0x0C
    CriticalSection m_Lock;                 // 0x10
    ProfilingUnit m_CompressionProfiling;   // 0x38
    ProfilingUnit m_DecompressionProfiling; // 0xA8
};
ASSERT_OFFSET(CompressionAlgorithm, m_CompressionProfiling, 0x38);
ASSERT_SIZE(CompressionAlgorithm, 0x118);
} // namespace nex
} // namespace nn
