#pragma once

#include "decomp.h"
#include "nw/font/font_CharStrmReader.h"
#include "nw/font/font_ResourceFormat.h"

namespace nw {
namespace font {
class Glyph;

// RTTI N2nw4font4FontE @ 0x008D0C10
// vtable 0x00903758 (vptr 0x00903760), offset_to_top 0, 26 entries
//
// The interface of the fonts. The names of the virtual functions are ours except for
// GetCharWidth, GetCharWidths and GetGlyph (reference symbols).
class Font
{
public:
    // the kinds of font (GetType)
    enum Type
    {
        TYPE_NULL,
        TYPE_ROM,
        TYPE_RESOURCE, // ResFont
        TYPE_PAIR      // PairFont
    };
    // the encodings (FontInformation::characterCode)
    enum CharacterCode
    {
        CHARACTER_CODE_UTF8,
        CHARACTER_CODE_UTF16,
        CHARACTER_CODE_SJIS,
        CHARACTER_CODE_CP1252
    };

    Font(); // 0x004D932C (name is ours)
    virtual ~Font(); // 0x0013F638 slot 0x00
    // 0x004D9358 slot 0x04 (deleting dtor)

    virtual int GetWidth() const = 0; // slot 0x08
    virtual int GetHeight() const = 0; // slot 0x0C
    virtual int GetAscent() const = 0; // slot 0x10
    virtual int GetDescent() const = 0; // slot 0x14
    virtual int GetMaxCharWidth() const = 0; // slot 0x18
    virtual Type GetType() const = 0; // slot 0x1C
    virtual int GetTextureFormat() const = 0; // slot 0x20
    virtual int GetLineFeed() const = 0; // slot 0x24
    virtual const CharWidths GetDefaultCharWidths() const = 0; // slot 0x28
    virtual void SetLineFeed(int linefeed) = 0; // slot 0x2C
    virtual void SetDefaultCharWidths(const CharWidths& widths) = 0; // slot 0x30
    // false: the font has no glyph for the character
    virtual bool SetAlternateChar(u16 c) = 0; // slot 0x34
    virtual int GetCharWidth(u16 c) const = 0; // slot 0x38
    virtual const CharWidths GetCharWidths(u16 c) const = 0; // slot 0x3C
    virtual void GetGlyph(nw::font::Glyph* pGlyph, u16 c) const = 0; // slot 0x40
    virtual bool HasGlyph(u16 c) const = 0; // slot 0x44
    virtual CharacterCode GetCharacterCode() const = 0; // slot 0x48
    virtual int GetBaselinePos() const = 0; // slot 0x4C
    virtual int GetCellHeight() const = 0; // slot 0x50
    virtual int GetCellWidth() const = 0; // slot 0x54
    virtual void SetLinearFilter(bool atSmall, bool atLarge) = 0; // slot 0x58
    virtual bool IsLinearFilterEnableAtSmall() const = 0; // slot 0x5C
    virtual bool IsLinearFilterEnableAtLarge() const = 0; // slot 0x60
    virtual u32 GetTextureWrapFilterValue() const = 0; // slot 0x64

    // the reader of wide strings (only UTF-16 fonts can read them)
    CharStrmReader GetCharStrmReader(wchar_t dummy) const; // 0x007452F0 | nintendogs:bytes [tier A]
};
} // namespace font
} // namespace nw
