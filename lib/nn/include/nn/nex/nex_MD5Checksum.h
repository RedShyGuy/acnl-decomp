#pragma once

#include "decomp.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"

namespace nn {
namespace nex {
// The MD5 of the data (16 bytes).
// RTTI N2nn3nex11MD5ChecksumE @ 0x008CE03C
// vtable 0x008FC30C (vptr 0x008FC314), offset_to_top 0, 8 entries
class MD5Checksum : public ::nn::nex::ChecksumAlgorithm
{
public:
    MD5Checksum(); // 0x00358934 | fefates:bytes [tier B]
    virtual ~MD5Checksum(); // 0x00358968 slot 0x00
    // 0x00358950 slot 0x04 (deleting dtor)
    virtual bool ComputeChecksum(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x00358804 slot 0x08 | fefates:bytes
    virtual bool ComputeChecksum(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::SignatureBytes& checksum); // 0x00358750 slot 0x0C | fefates:bytes
    // (inline, with a copy out of line)
    virtual bool IsReady() const { return true; } // 0x0072C1D4 slot 0x10
    virtual u32 ComputeChecksumForTransportArray(const unsigned char** ppData, const unsigned int* pSizes, int count); // 0x00358898 slot 0x18 | fefates:bytes
    virtual u32 GetChecksumLength(); // 0x00358890 slot 0x1C
};
} // namespace nex
} // namespace nn
