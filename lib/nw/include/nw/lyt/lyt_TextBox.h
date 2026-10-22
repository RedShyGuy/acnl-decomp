#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector2.h"
#include "nw/font/font_TagProcessorBase.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace font {
class Font;
struct DispStringBuffer;
template <typename T>
class TextWriterBase;
} // namespace font

namespace lyt {
namespace res {
struct TextBox;
} // namespace res

// RTTI N2nw3lyt7TextBoxE @ 0x008D08A8
// vtable 0x00902D98 (vptr 0x00902DA0), offset_to_top 0, 32 entries
//
// A pane that draws a string with a font: with GL through a TextWriter, or with GPU commands
// from a display string buffer (Drawer).
class TextBox : public ::nw::lyt::Pane
{
public:
    typedef font::TagProcessorBase<wchar_t> TagProcessor;

    // the flags of AllocStringBuffer
    static const u32 ALLOC_FLAG_COMMAND_BUFFER = 1 << 3; // the commands go to device memory

    // the position of the text: h + v * 3 (0: left / top, 1: center, 2: right / bottom)
    static const u8 POSITION_CENTER = 1;

    TextBox(const res::TextBox* pBlock, const ResBlockSet& resBlockSet, u32 allocFlags = 0); // 0x004BC5C4 | nintendogs:callseq-callee [tier A]
    virtual ~TextBox(); // 0x004BC824 slot 0x00 | libgarden
    // 0x004BC774 (deleting dtor)
    virtual const PaneTypeInfo* GetRuntimeTypeInfo() const; // 0x0073E784 slot 0x08 (name is ours)
    virtual ut::Color8 GetVtxColor(u32 idx) const; // 0x0073E5E0 slot 0x0C | nintendogs:bytes
    virtual void SetVtxColor(u32 idx, ut::Color8 value); // 0x004BBF98 slot 0x10
    virtual u8 GetVtxColorElement(u32 idx) const; // 0x0073E790 slot 0x1C | mk7dlp:bytes
    virtual void SetVtxColorElement(u32 idx, u8 value); // 0x004BC38C slot 0x20 | nintendogs:bytes
    virtual u8 GetMaterialNum() const; // 0x0073E6B4 slot 0x24
    virtual Material* GetMaterial(u32 idx); // 0x0073E5D0 slot 0x28
    virtual void DrawSelf(DrawInfo& drawInfo); // 0x004BC508 slot 0x64 (name is ours)
    virtual u32* MakeUniformDataSelf(u32* pCommand, DrawInfo& drawInfo, Drawer& drawer); // 0x0073E7A8 slot 0x68 | libgarden
    virtual void LoadMtx(DrawInfo& drawInfo); // 0x004BC430 slot 0x6C (name is ours)
    // room for minLen characters (the flags as in the display string buffer)
    virtual void AllocStringBuffer(u16 minLen, u32 flags); // 0x0013B9BC slot 0x70 | libgarden
    virtual void FreeStringBuffer(); // 0x004BC210 slot 0x74 (name is ours)
    // the string from dstIdx on; the characters set
    virtual u16 SetString(const wchar_t* str, u16 dstIdx); // 0x004BBFAC slot 0x78 (name is ours)
    virtual u16 SetString(const wchar_t* str, u16 dstIdx, u16 strLen); // 0x004BC5C0 slot 0x7C | libgarden

    // the writer gets the font, colors and layout of the text box
    void SetupTextWriter(font::TextWriterBase<wchar_t>* pWriter); // 0x004BC098 | nintendogs:callseq [tier A]
    // the characters into the display string buffer, then the commands (with a drawer)
    void SetupDrawCharData(Drawer* pDrawer); // 0x004BC2D0 | nintendogs:callseq [tier A]
    // the left top of the text in the box of the size
    nn::math::VEC2 AdjustTextPos(const Size& size, bool isCeil) const; // 0x0073E5F4 | nintendogs:bytes [tier A]
    // the global matrix moved to the text
    void GetTextGlobalMtx(nn::math::MTX34* pMtx) const; // 0x0073E6C4 | nintendogs:bytes [tier A]

    // (inline, a copy out of line for the game; name is ours)
    void SetTextPositionH(u8 pos); // 0x004BC270 (name is ours)
    // (inline)
    void SetTagProcessor(TagProcessor* pTagProcessor)
    {
        const bool isChanged = m_pTagProcessor != pTagProcessor;
        m_IsTextChanged |= isChanged;
        if (isChanged) {
            m_pTagProcessor = pTagProcessor;
        }
    }

private:
    void Init(u16 allocStrLen, u32 allocFlags); // 0x004BC3A4 (name is ours)
    u16 SetStringImpl(const wchar_t* str, u16 dstIdx, u16 strLen); // 0x004BBFDC (name is ours)

public:
    static const PaneTypeInfo s_TypeInfo; // (name is ours)

    wchar_t* m_TextBuf;                         // 0x0D4
    ut::Color8 m_TextColors[2];                 // 0x0D8 top, bottom
    const font::Font* m_pFont;                  // 0x0E0
    Size m_FontSize;                            // 0x0E4
    f32 m_LineSpace;                            // 0x0EC
    f32 m_CharSpace;                            // 0x0F0
    TagProcessor* m_pTagProcessor;              // 0x0F4
    u16 m_TextBufBytes;                         // 0x0F8
    u16 m_TextLen;                              // 0x0FA
    u8 m_TextPosition;                          // 0x0FC
    u8 m_TextAlignment;                         // 0x0FD 0: by m_TextPosition, 1: left, 2: center, 3: right
    bool m_IsTextChanged;                       // 0x0FE the display string buffer is old
    Material* m_pMaterial;                      // 0x100
    font::DispStringBuffer* m_pDispStringBuf;   // 0x104
};
ASSERT_OFFSET(TextBox, m_pMaterial, 0x100);
ASSERT_SIZE(TextBox, 0x108);
} // namespace lyt
} // namespace nw
