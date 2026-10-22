#include <cstring>

#include "nw/gfx/gfx_ShaderBinaryInfo_SafeBuffer.h"

// 0x0049C3F8 | nintendogs:bytes [tier A]
int nw::gfx::ShaderBinaryInfo::SafeBuffer::Write(const u32* pData, int size)
{
    if (size <= 0) {
        return -1;
    }
    if (m_pCurrent + size > m_pEnd) {
        m_IsOverflow = true;
        return -1;
    }
    std::memcpy(m_pCurrent, pData, size);
    m_pCurrent += size;
    return size;
}
