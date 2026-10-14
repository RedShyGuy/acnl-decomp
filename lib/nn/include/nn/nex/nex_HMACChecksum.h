#pragma once

#include "decomp.h"
#include "nn/nex/nex_KeyedChecksumAlgorithm.h"

namespace nn {
namespace nex {
class MD5;

// HMAC-MD5 (16 bytes). A key over 64 bytes is replaced by its MD5. The member names are ours.
// RTTI N2nn3nex12HMACChecksumE @ 0x008CE0C8
// vtable 0x008FC4CC (vptr 0x008FC4D4), offset_to_top 0, 9 entries
class HMACChecksum : public ::nn::nex::KeyedChecksumAlgorithm
{
public:
    static const u32 PAD_SIZE = 64;

    HMACChecksum(); // 0x0035CDB8 | mk7dlp:callseq-callee [tier A]
    virtual ~HMACChecksum(); // 0x0035CE48 slot 0x00 | mk7dlp:callseq-callee
    // 0x0035CE14 slot 0x04 (deleting dtor)
    virtual bool ComputeChecksum(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x0035CBCC slot 0x08 | fefates:callseq
    virtual bool ComputeChecksum(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::SignatureBytes& checksum); // 0x0035CB5C slot 0x0C
    virtual u32 ComputeChecksumForTransportArray(const unsigned char** ppData, const unsigned int* pSizes, int count); // 0x0035CD5C slot 0x18
    virtual u32 GetChecksumLength(); // 0x0035CC50 slot 0x1C
    // the pads of the key
    virtual void KeyHasChanged(); // 0x0035CA68 slot 0x20 | fefates:bytes
    // the HMAC of the arrays goes to outer (finished)
    void ChecksumComputeHelper(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::MD5& outer); // 0x0035CC58 | fefates:bytes [tier B]

    u8 m_InnerPad[PAD_SIZE + 1]; // 0x2C (the key XOR 0x36)
    u8 m_OuterPad[PAD_SIZE + 1]; // 0x6D (the key XOR 0x5C)
};
ASSERT_OFFSET(HMACChecksum, m_OuterPad, 0x6D);
ASSERT_SIZE(HMACChecksum, 0xB0);
} // namespace nex
} // namespace nn
