#pragma once

#include "decomp.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11MD5ChecksumE @ 0x008CE03C
// vtable 0x008FC30C (vptr 0x008FC314), offset_to_top 0, 8 entries
class MD5Checksum : public ::nn::nex::ChecksumAlgorithm
{
public:
    virtual void vf_0x00(); // 0x00358968 slot 0x00 | virtual slot, introduced by nn::nex::MD5Checksum
    virtual ~MD5Checksum(); // 0x00358950 slot 0x04 | slot vf_0x04 of nn::nex::MD5Checksum
    virtual void ComputeChecksum(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x00358804 slot 0x08 | fefates:bytes
    virtual void ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&); // 0x00358750 slot 0x0C | fefates:bytes
    virtual void IsReady() const; // 0x0072C1D4 slot 0x10 | slot vf_0x10 of nn::nex::MD5Checksum
    virtual void ComputeChecksumForTransport(const unsigned char*, unsigned int); // 0x00385EB0 slot 0x14 | fefates:bytes
    virtual void ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int); // 0x00358898 slot 0x18 | fefates:bytes
    virtual void GetChecksumLength(); // 0x00358890 slot 0x1C | slot vf_0x1C of nn::nex::MD5Checksum
    MD5Checksum(); // 0x00358934 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
