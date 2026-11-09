#include "script/dRenderBase.h"
#include <algorithm>
#include <new>

namespace script {
// 0x005D6BA4 (name is ours)
void RenderBase::Clear()
{
    std::fill(m_pText, m_pText + m_Len, 0);
    for (u32 i = 0; i < m_Count; i++) {
        m_pBuf[i].pText = m_pText;
        m_pBuf2[i] = 0;
    }
    m_pBuf2[m_Count - 1] = 1;
}

// 0x005D6C38 (name is ours)
RenderBase::RenderBase(u32 length, u32 tagNum)
    : m_TextColors{nw::ut::Color8(0, 0, 0, 0xFF), nw::ut::Color8(0, 0, 0, 0xFF)}, m_FontSize(-1.0f, -1.0f)
{
    m_pText = new (std::nothrow) char16[length];
    m_Len = length;
    m_Count = tagNum + 1;
    m_pBuf = new (std::nothrow) Position[m_Count];
    m_pBuf2 = new (std::nothrow) u8[m_Count];
    m_pEnd = m_pText + length;
    Clear();
}

// 0x005D6D30 slot 0x00
// 0x005D6CF0 (deleting dtor)
RenderBase::~RenderBase()
{
    delete[] m_pBuf2;
    delete[] m_pBuf;
    delete[] m_pText;
}
} // namespace script
