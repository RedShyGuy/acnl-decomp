#pragma once

#include "decomp.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22KeyedChecksumAlgorithmE @ 0x008CEC44
// vtable 0x008FE154 (vptr 0x008FE15C), offset_to_top 0, 9 entries
class KeyedChecksumAlgorithm : public ::nn::nex::ChecksumAlgorithm
{
public:
    KeyedChecksumAlgorithm(); // ctor address unknown
    virtual ~KeyedChecksumAlgorithm(); // 0x0039DAF4 slot 0x00 | slot vf_0x00 of nn::nex::KeyedChecksumAlgorithm
    virtual void vf_0x04(); // 0x0039DAC0 slot 0x04 | virtual slot, introduced by nn::nex::KeyedChecksumAlgorithm
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void IsReady() const; // 0x0072D088 slot 0x10 | fefates:bytes
    virtual void ComputeChecksumForTransport(const unsigned char*, unsigned int); // 0x00385EB0 slot 0x14 | fefates:bytes
    virtual void ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void GetChecksumLength(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void KeyHasChanged(); // 0x0039DA70 slot 0x20 | slot vf_0x20 of nn::nex::KeyedChecksumAlgorithm
    void SetKey(const nn::nex::Key&); // 0x0039DA74 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
