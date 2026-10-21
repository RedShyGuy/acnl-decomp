#include "nw/font/font_DispStringBuffer.h"

namespace nw {
namespace font {
// 0x004D89C8 (name is ours)
DECOMP_NOINLINE u32 nw::font::DispStringBuffer::CalcCommandBufferSize(u32 charCountMax)
{
    return (charCountMax * 78 + 57) & ~3;
}

// 0x004D89DC (name is ours)
nw::font::DispStringBuffer::DispStringBuffer(u32 charCountMax)
    : m_CharCountMax(charCountMax), m_CharCount(0), m_DrawCharCount(0xFFFF), m_IsCommandGenerated(false), m_Flags(0), m_pCommandBufferAlt(nullptr),
      m_CommandSize(0), m_Unknown0x24(0)
{
    // behind the characters: a bit per character, then the command buffer (16 byte aligned)
    m_pCharFlags = reinterpret_cast<u8*>(GetCharAttributes() + charCountMax);
    m_pCommandBufferBase = reinterpret_cast<u32*>((reinterpret_cast<uptr>(m_pCharFlags) + (charCountMax + 7) / 8 + 15) & ~15);
    m_CommandBufferSize = CalcCommandBufferSize(charCountMax);
    m_pCommandBuffer = m_pCommandBufferBase;
}

} // namespace font
} // namespace nw
