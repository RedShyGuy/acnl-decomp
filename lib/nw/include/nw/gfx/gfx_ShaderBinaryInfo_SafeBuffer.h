#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_ShaderBinaryInfo.h"

// A command buffer that does not write beyond its end (it remembers the overflow instead; the class
// name and Write are from the reference symbols, the members are ours).
class nw::gfx::ShaderBinaryInfo::SafeBuffer
{
public:
    // (inline)
    SafeBuffer(u32* pBuffer, u32 size)
        : m_pBegin(reinterpret_cast<u8*>(pBuffer)), m_pCurrent(reinterpret_cast<u8*>(pBuffer)),
          m_pEnd(reinterpret_cast<u8*>(pBuffer) + size), m_IsOverflow(false)
    {
    }
    // size bytes appended; returns size (-1: nothing written, no room or nothing to write)
    int Write(const u32* pData, int size); // 0x0049C3F8 | nintendogs:bytes [tier A]
    // (inline) the bytes written
    u32 GetSize() const { return m_pCurrent - m_pBegin; }

    u8* m_pBegin;      // 0x0
    u8* m_pCurrent;    // 0x4
    u8* m_pEnd;        // 0x8
    bool m_IsOverflow; // 0xC
};
ASSERT_SIZE(nw::gfx::ShaderBinaryInfo::SafeBuffer, 0x10);
