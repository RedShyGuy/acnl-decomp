#pragma once

#include "decomp.h"
#include "nn/nex/nex_PluginObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20CompressionAlgorithmE @ 0x008CE85C
// vtable 0x008FD7DC (vptr 0x008FD7E4), offset_to_top 0, 4 entries
class CompressionAlgorithm : public ::nn::nex::PluginObject
{
public:
    CompressionAlgorithm(); // ctor address unknown
    virtual void vf_0x00(); // 0x00396270 slot 0x00 | virtual slot, introduced by nn::nex::CompressionAlgorithm
    virtual void vf_0x04(); // 0x00396218 slot 0x04 | fefates:callseq
    virtual void CompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void DecompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
};
} // namespace nex
} // namespace nn
