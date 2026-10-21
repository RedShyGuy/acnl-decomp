#pragma once

#include "decomp.h"
#include "nw/font/font_Font.h"

namespace nw {
namespace font {
// RTTI N2nw4font8PairFontE @ 0x008D0C24
// vtable 0x0090383C (vptr 0x00903844), offset_to_top 0, 26 entries
//
// Two fonts as one: a character comes from the first font that has it. The metrics are the
// larger ones of both fonts. Member names are ours.
class PairFont : public ::nw::font::Font
{
public:
    PairFont(Font* pFirst, Font* pSecond); // 0x004D9764 (name is ours)
    virtual ~PairFont(); // 0x004D97A0 slot 0x00
    // 0x004D9790 slot 0x04 (deleting dtor)

    virtual int GetWidth() const; // 0x00745784 slot 0x08
    virtual int GetHeight() const; // 0x00745864 slot 0x0C
    virtual int GetAscent() const; // 0x00745808 slot 0x10
    virtual int GetDescent() const; // 0x00745330 slot 0x14
    virtual int GetMaxCharWidth() const; // 0x007455F8 slot 0x18
    virtual Type GetType() const; // 0x007456F0 slot 0x1C
    virtual int GetTextureFormat() const; // 0x00745640 slot 0x20
    virtual int GetLineFeed() const; // 0x0074538C slot 0x24
    virtual const CharWidths GetDefaultCharWidths() const; // 0x00745650 slot 0x28
    virtual void SetLineFeed(int linefeed); // 0x004D9620 slot 0x2C
    virtual void SetDefaultCharWidths(const CharWidths& widths); // 0x004D972C slot 0x30
    virtual bool SetAlternateChar(u16 c); // 0x004D9688 slot 0x34
    virtual int GetCharWidth(u16 c) const; // 0x00745420 slot 0x38 | slot vf_0x38 of nw::font::Font
    virtual const CharWidths GetCharWidths(u16 c) const; // 0x007454FC slot 0x3C | slot vf_0x3C of nw::font::Font
    virtual void GetGlyph(nw::font::Glyph* pGlyph, u16 c) const; // 0x007456F8 slot 0x40 | slot vf_0x40 of nw::font::Font
    virtual bool HasGlyph(u16 c) const; // 0x007457BC slot 0x44
    virtual CharacterCode GetCharacterCode() const; // 0x00745630 slot 0x48
    virtual int GetBaselinePos() const; // 0x0074559C slot 0x4C
    virtual int GetCellHeight() const; // 0x007454A0 slot 0x50
    virtual int GetCellWidth() const; // 0x007453C4 slot 0x54
    virtual void SetLinearFilter(bool atSmall, bool atLarge); // 0x004D96EC slot 0x58
    virtual bool IsLinearFilterEnableAtSmall() const; // 0x007456E0 slot 0x5C
    virtual bool IsLinearFilterEnableAtLarge() const; // 0x007456D0 slot 0x60
    virtual u32 GetTextureWrapFilterValue() const; // 0x007456C0 slot 0x64

private:
    // the font that draws the character: the first one that has it, else the one with the
    // alternate character (inline; name is ours)
    const Font* GetFontFor(u16 c) const;

public:
    Font* m_pFirst;            // 0x4
    Font* m_pSecond;           // 0x8
    bool m_IsAlternateFirst;   // 0xC (the alternate character is the one of the first font)
};
ASSERT_SIZE(PairFont, 0x10);
} // namespace font
} // namespace nw
