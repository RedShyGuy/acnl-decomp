#pragma once

#include "decomp.h"
#include "nn/nex/nex_CompressionAlgorithm.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15ZLibCompressionE @ 0x008CE538
// vtable 0x008FD06C (vptr 0x008FD074), offset_to_top 0, 4 entries
class ZLibCompression : public ::nn::nex::CompressionAlgorithm
{
public:
    ZLibCompression(); // ctor candidate(s) 0x0037EB10, 0x0037EC3C (unverified)
    virtual void vf_0x00(); // 0x0037EE08 slot 0x00 | virtual slot, introduced by nn::nex::CompressionAlgorithm
    virtual void vf_0x04(); // 0x0037ED54 slot 0x04 | fefates:callseq
    virtual void CompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0037E804 slot 0x08 | slot vf_0x08 of nn::nex::CompressionAlgorithm
    virtual void DecompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0037E974 slot 0x0C | fefates:bytes-fuzzy
    ZLibCompression(int, int); // 0x0037EB10 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
