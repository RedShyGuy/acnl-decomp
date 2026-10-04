#pragma once

#include "decomp.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// Text output collected in a ring buffer and printed later (by Flush). Layout from Clear / Flush;
// the member names and the names marked so are ours.
class CachedPrint : public RootObject
{
public:
    static void DestroyInstance(); // 0x004266E0 | fefates:bytes [tier B]

    // empties the buffer (name is ours)
    void Clear(); // 0x00426734
    // prints and empties the buffer (name is ours)
    void Flush(); // 0x00426764

    char* m_pBuffer;                   // 0x00
    u32 m_BufferSize;                  // 0x04
    u32 m_ReadOffset;                  // 0x08
    u32 m_WrapOffset;                  // 0x0C, end of the text at the start of the buffer
    u32 m_WriteOffset;                 // 0x10
    CriticalSection m_CriticalSection; // 0x14

    static CachedPrint* s_pInstance; // 0x00975A34
};
ASSERT_SIZE(CachedPrint, 0x20);
} // namespace common
} // namespace pia
} // namespace nn
