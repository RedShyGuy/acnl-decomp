#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto15HashContextBaseE @ 0x008D04C0
//
// Interface of a hash (Md5Context, Sha1Context, Sha256Context, imgdb::Crc16Context). Initialize,
// Update, GetHash and ProcessBlock are named in the symbols; the other slot names are ours, after
// what the implementations do. The defaults of GetHashSize and ProcessBlock are only used by
// imgdb::Crc16Context.
class HashContextBase
{
public:
    virtual void Initialize() = 0;                           // slot 0x00
    // (Crc16Context clears its "initialized" flag; the others do nothing)
    virtual void Finalize() = 0;                             // slot 0x04 (name is ours)
    virtual void Update(const void* pData, size_t size) = 0; // slot 0x08
    virtual size_t GetHashSize() const { return 0; }         // 0x0048349C slot 0x0C (name is ours)
    virtual void GetHash(void* pOutput) = 0;                 // slot 0x10
    virtual ~HashContextBase() {}                            // slots 0x14, 0x18
    virtual void ProcessBlock() {}                           // 0x004834A4 slot 0x1C
};
} // namespace crypto
} // namespace nn
