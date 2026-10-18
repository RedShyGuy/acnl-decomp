#pragma once

#include "decomp.h"

#include <cstring>

namespace nn {
namespace nex {
// The bytes of a signature or checksum (at most 16). The member names are ours.
struct SignatureBytes
{
    static const u32 MAX_SIZE = 16;

    // (inline) copies the bytes of the size only
    SignatureBytes& operator=(const SignatureBytes& other)
    {
        u32 size = other.m_Size;
        memcpy(m_Data, other.m_Data, size);
        m_Size = size;
        return *this;
    }

    u8 m_Data[MAX_SIZE]; // 0x00
    u32 m_Size;          // 0x10
};
ASSERT_SIZE(SignatureBytes, 0x14);
} // namespace nex
} // namespace nn
