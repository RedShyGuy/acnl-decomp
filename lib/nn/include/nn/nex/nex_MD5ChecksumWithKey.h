#pragma once

#include "decomp.h"
#include "nn/nex/nex_KeyedChecksumAlgorithm.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18MD5ChecksumWithKeyE @ 0x008CE718
// vtable 0x008FD4F0 (vptr 0x008FD4F8), offset_to_top 0, 9 entries
class MD5ChecksumWithKey : public ::nn::nex::KeyedChecksumAlgorithm
{
public:
    MD5ChecksumWithKey(); // ctor candidate(s) 0x003D32A8 (unverified)
    virtual ~MD5ChecksumWithKey(); // 0x0038C5CC slot 0x00 | slot vf_0x00 of nn::nex::KeyedChecksumAlgorithm
    virtual void vf_0x04(); // 0x0038C598 slot 0x04 | virtual slot, introduced by nn::nex::KeyedChecksumAlgorithm
    virtual void vf_0x08(); // 0x0038C408 slot 0x08 | fefates:callseq
    virtual void ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&); // 0x0038C398 slot 0x0C | slot vf_0x0C of nn::nex::KeyedChecksumAlgorithm
    virtual void IsReady() const; // 0x0072C1F8 slot 0x10 | slot vf_0x10 of nn::nex::KeyedChecksumAlgorithm
    virtual void ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int); // 0x0038C53C slot 0x18 | slot vf_0x18 of nn::nex::KeyedChecksumAlgorithm
    virtual void GetChecksumLength(); // 0x0038C48C slot 0x1C | slot vf_0x1C of nn::nex::KeyedChecksumAlgorithm
    void ChecksumComputeHelper(const unsigned char**, const unsigned int*, int, nn::nex::MD5&); // 0x0038C494 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
