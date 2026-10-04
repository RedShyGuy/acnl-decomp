#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common15HashContextBaseE @ 0x008CFE50
//
// Interface of a hash function (Hmac uses it). The slots are the ones Hmac calls; the names of
// GetHashSize / Update / GetHash are from Md5Context, the others are ours.
class HashContextBase : public ::nn::pia::common::RootObject
{
public:
    virtual void Initialize() = 0;                              // slot 0x00
    virtual void Update(const void* pData, unsigned int size) = 0; // slot 0x04
    virtual unsigned int GetHashSize() const = 0;               // slot 0x08
    virtual unsigned int GetBlockSize() const = 0;              // slot 0x0C
    virtual void GetHash(void* pOutput) = 0;                    // slot 0x10
};
} // namespace common
} // namespace pia
} // namespace nn
