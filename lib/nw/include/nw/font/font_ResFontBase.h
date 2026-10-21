#pragma once

#include "decomp.h"
#include "nw/font/font_Font.h"

namespace nw {
namespace font {
namespace internal {
class TextureObject;
} // namespace internal

// RTTI N2nw4font11ResFontBaseE @ 0x008D0BF4
// vtable 0x009036B4 (vptr 0x009036BC), offset_to_top 0, 27 entries
//
// A font from a font resource (font_ResourceFormat.h). Member names are ours.
class ResFontBase : public ::nw::font::Font
{
public:
    // the glyph index of characters that are not in the font
    static const u16 GLYPH_INDEX_NOT_FOUND = 0xFFFF;
    // the filter bits of m_Filter (names are ours)
    static const u32 FILTER_LINEAR_AT_LARGE = 1 << 1;
    static const u32 FILTER_LINEAR_AT_SMALL = 1 << 2;

    ResFontBase(); // 0x004D88B0 (name is ours)
    virtual ~ResFontBase(); // 0x0013F634 slot 0x00
    // 0x004D88EC slot 0x04 (deleting dtor)

    virtual int GetWidth() const; // 0x007452A0 slot 0x08
    virtual int GetHeight() const; // 0x007452D4 slot 0x0C
    virtual int GetAscent() const; // 0x007452C8 slot 0x10
    virtual int GetDescent() const; // 0x00744EF4 slot 0x14
    virtual int GetMaxCharWidth() const; // 0x007450D8 slot 0x18
    virtual Type GetType() const; // 0x00745298 slot 0x1C
    virtual int GetTextureFormat() const; // 0x007450F4 slot 0x20
    virtual int GetLineFeed() const; // 0x00744F08 slot 0x24
    virtual const CharWidths GetDefaultCharWidths() const; // 0x00745264 slot 0x28
    virtual void SetLineFeed(int linefeed); // 0x004D8754 slot 0x2C
    virtual void SetDefaultCharWidths(const CharWidths& widths); // 0x004D8898 slot 0x30
    virtual bool SetAlternateChar(u16 c); // 0x004D8760 slot 0x34
    virtual int GetCharWidth(u16 c) const; // 0x00744F24 slot 0x38 | nintendogs:bytes
    virtual const CharWidths GetCharWidths(u16 c) const; // 0x00744F50 slot 0x3C | nintendogs:bytes
    virtual void GetGlyph(nw::font::Glyph* pGlyph, u16 c) const; // 0x00745114 slot 0x40 | nintendogs:bytes
    virtual bool HasGlyph(u16 c) const; // 0x007452AC slot 0x44
    virtual CharacterCode GetCharacterCode() const; // 0x007450E8 slot 0x48
    virtual int GetBaselinePos() const; // 0x007450C8 slot 0x4C
    virtual int GetCellHeight() const; // 0x00744F40 slot 0x50
    virtual int GetCellWidth() const; // 0x00744F14 slot 0x54
    virtual void SetLinearFilter(bool atSmall, bool atLarge); // 0x004D8794 slot 0x58
    virtual bool IsLinearFilterEnableAtSmall() const; // 0x00745288 slot 0x5C
    virtual bool IsLinearFilterEnableAtLarge() const; // 0x00745278 slot 0x60
    virtual u32 GetTextureWrapFilterValue() const; // 0x00745270 slot 0x64
    // the sheets of the font
    virtual int GetNumTextureObject() const; // 0x00745104 slot 0x68

protected:
    // (name is ours)
    void SetResourceBuffer(void* pResource, FontInformation* pFontInfo); // 0x004D8788 (name is ours)
    // the textures of the sheets (the draw buffer has one TextureObject per sheet)
    void GenTextureNames(); // 0x001426DC | nintendogs:bytes-fuzzy [tier A]
    void DeleteTextureNames(); // 0x0013F5DC | nintendogs:bytes [tier A]

    // the glyph index of the character (GLYPH_INDEX_NOT_FOUND: none); the last one is cached
    u16 FindGlyphIndex(u16 c) const; // 0x00744FC8 | nintendogs:bytes [tier A]
    void GetGlyphFromIndex(nw::font::Glyph* pGlyph, u16 index) const; // 0x0074514C | nintendogs:bytes [tier A]

    // (inline; names are ours)
    u16 FindGlyphIndex(const FontCodeMap* pMap, u16 c) const;
    const CharWidths& GetCharWidthsFromIndex(u16 index) const;

public:
    void* m_pResource;                               // 0x04 (NULL: no resource)
    FontInformation* m_pFontInfo;                    // 0x08
    internal::TextureObject* m_pTextureObjects;      // 0x0C the draw buffer
    u32 m_Filter;                                    // 0x10
    mutable u16 m_LastCharCode;                      // 0x14
    mutable u16 m_LastGlyphIndex;                    // 0x16
};
ASSERT_SIZE(ResFontBase, 0x18);
} // namespace font
} // namespace nw
