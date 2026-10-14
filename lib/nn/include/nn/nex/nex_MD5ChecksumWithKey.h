#pragma once

#include "decomp.h"
#include "nn/nex/nex_KeyedChecksumAlgorithm.h"

namespace nn {
namespace nex {
class MD5;

// The MD5 of the data followed by the key (16 bytes).
// RTTI N2nn3nex18MD5ChecksumWithKeyE @ 0x008CE718
// vtable 0x008FD4F0 (vptr 0x008FD4F8), offset_to_top 0, 9 entries
class MD5ChecksumWithKey : public ::nn::nex::KeyedChecksumAlgorithm
{
public:
    // (inline, e.g. in Network::Network)
    MD5ChecksumWithKey() {}
    virtual ~MD5ChecksumWithKey(); // 0x0038C5CC slot 0x00
    // 0x0038C598 slot 0x04 (deleting dtor)
    virtual bool ComputeChecksum(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x0038C408 slot 0x08 | fefates:callseq
    virtual bool ComputeChecksum(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::SignatureBytes& checksum); // 0x0038C398 slot 0x0C
    // (inline, with a copy out of line)
    virtual bool IsReady() const { return true; } // 0x0072C1F8 slot 0x10
    virtual u32 ComputeChecksumForTransportArray(const unsigned char** ppData, const unsigned int* pSizes, int count); // 0x0038C53C slot 0x18
    virtual u32 GetChecksumLength(); // 0x0038C48C slot 0x1C
    // the MD5 of the arrays and the key goes to md5 (finished)
    void ChecksumComputeHelper(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::MD5& md5); // 0x0038C494 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
