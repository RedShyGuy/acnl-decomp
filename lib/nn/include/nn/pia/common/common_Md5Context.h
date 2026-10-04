#pragma once

#include "decomp.h"
#include "nn/pia/common/common_HashContextBase.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common10Md5ContextE @ 0x008CFDFC
// vtable 0x0090150C (vptr 0x00901514), offset_to_top 0, 6 entries
//
// MD5 (RFC 1321). Layout from Initialize / Update; the member names are ours.
class Md5Context : public ::nn::pia::common::HashContextBase
{
public:
    static const unsigned int HASH_SIZE = 16;
    static const unsigned int BLOCK_SIZE = 64;

    // (inline: hashWithMd5 only stores the vptr)
    Md5Context() {}

    virtual void Initialize(); // 0x004261BC slot 0x00 | fefates:callgraph
    virtual void Update(const void* pData, unsigned int size); // 0x00426504 slot 0x04 | fefates:bytes
    virtual unsigned int GetHashSize() const; // 0x0073180C slot 0x08
    virtual unsigned int GetBlockSize() const; // 0x00731814 slot 0x0C (name is ours)
    virtual void GetHash(void* pOutput); // 0x004265D0 slot 0x10 | fefates:bytes
    // hashes m_Block into m_State
    virtual void ProcessBlock(); // 0x004261EC slot 0x14 | fefates:bytes

    u32 m_State[4];          // 0x04
    u32 m_Size;              // 0x14, bytes so far
    u8 m_Block[BLOCK_SIZE];  // 0x18
};
ASSERT_SIZE(Md5Context, 0x58);
} // namespace common
} // namespace pia
} // namespace nn
