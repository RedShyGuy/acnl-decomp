#pragma once

#include "decomp.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace font {
namespace internal {
class TextureObject;
} // namespace internal

// The characters of a string for the command drawing: CharWriter collects them, RectDrawer
// builds the GPU commands from them, CharWriter::UseCommandBuffer puts the commands into a
// command list. The buffer is one block: this header, the characters (not constructed), a bit
// per character, the command buffer (CharWriter::GetDispStringBufferSize). The name is from
// CharWriter::InitDispStringBuffer; struct and member names are ours.
struct DispStringBuffer
{
    // a character
    struct CharAttribute
    {
        f32 width;                                  // 0x00
        f32 height;                                 // 0x04 (negative: downwards)
        f32 x;                                      // 0x08
        f32 y;                                      // 0x0C
        ut::Color8 color[2];                        // 0x10 the text colors
        f32 texLeft;                                // 0x18
        f32 texTop;                                 // 0x1C
        f32 texRight;                               // 0x20
        f32 texBottom;                              // 0x24
        internal::TextureObject* pTextureObject;    // 0x28
    };

    // the bits of m_Flags
    static const u8 FLAG_SUBROUTINE = 1 << 1;   // the commands end with a kick: called as a subroutine
    static const u8 FLAG_ALT_BUFFER = 1 << 2;   // the subroutine is m_pCommandBufferAlt

    explicit DispStringBuffer(u32 charCountMax); // 0x004D89DC (name is ours)

    // the characters behind the header
    CharAttribute* GetCharAttributes() { return reinterpret_cast<CharAttribute*>(this + 1); }

    // the size of the command buffer in words for the characters
    static u32 CalcCommandBufferSize(u32 charCountMax); // 0x004D89C8 (name is ours)

    u32 m_CharCountMax;            // 0x00
    u16 m_CharCount;               // 0x04
    u16 m_DrawCharCount;           // 0x06 (0xFFFF: all)
    bool m_IsCommandGenerated;     // 0x08
    u8 m_Flags;                    // 0x09
    u8* m_pCharFlags;              // 0x0C a bit per character
    u32* m_pCommandBuffer;         // 0x10
    u32* m_pCommandBufferBase;     // 0x14 (16 byte aligned)
    u32* m_pCommandBufferAlt;      // 0x18
    u32 m_CommandSize;             // 0x1C in words
    u32 m_CommandBufferSize;       // 0x20 in words
    u32 m_Unknown0x24;             // 0x24
};
ASSERT_SIZE(DispStringBuffer::CharAttribute, 0x2C);
ASSERT_SIZE(DispStringBuffer, 0x28);
} // namespace font
} // namespace nw
