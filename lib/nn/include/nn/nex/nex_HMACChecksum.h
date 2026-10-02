#pragma once

#include "decomp.h"
#include "nn/nex/nex_KeyedChecksumAlgorithm.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12HMACChecksumE @ 0x008CE0C8
// vtable 0x008FC4CC (vptr 0x008FC4D4), offset_to_top 0, 9 entries
class HMACChecksum : public ::nn::nex::KeyedChecksumAlgorithm
{
public:
    virtual ~HMACChecksum(); // 0x0035CE48 slot 0x00 | mk7dlp:callseq-callee
    virtual void vf_0x04(); // 0x0035CE14 slot 0x04 | virtual slot, introduced by nn::nex::KeyedChecksumAlgorithm
    virtual void vf_0x08(); // 0x0035CBCC slot 0x08 | fefates:callseq
    virtual void ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&); // 0x0035CB5C slot 0x0C | slot vf_0x0C of nn::nex::KeyedChecksumAlgorithm
    virtual void ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int); // 0x0035CD5C slot 0x18 | slot vf_0x18 of nn::nex::KeyedChecksumAlgorithm
    virtual void GetChecksumLength(); // 0x0035CC50 slot 0x1C | slot vf_0x1C of nn::nex::KeyedChecksumAlgorithm
    virtual void KeyHasChanged(); // 0x0035CA68 slot 0x20 | fefates:bytes
    void ChecksumComputeHelper(const unsigned char**, const unsigned int*, int, nn::nex::MD5&); // 0x0035CC58 | fefates:bytes [tier B]
    HMACChecksum(); // 0x0035CDB8 | mk7dlp:callseq-callee [tier A]
};
} // namespace nex
} // namespace nn
