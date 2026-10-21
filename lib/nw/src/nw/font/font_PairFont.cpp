#include "nw/font/font_PairFont.h"

namespace nw {
namespace font {
// 0x004D9764 (name is ours)
nw::font::PairFont::PairFont(Font* pFirst, Font* pSecond) : m_pFirst(pFirst), m_pSecond(pSecond), m_IsAlternateFirst(true)
{
}

// 0x004D97A0 slot 0x00
// 0x004D9790 (deleting dtor)
nw::font::PairFont::~PairFont()
{
}

// (inline)
DECOMP_ALWAYS_INLINE const nw::font::Font* nw::font::PairFont::GetFontFor(u16 c) const
{
    if (m_pFirst->HasGlyph(c)) {
        return m_pFirst;
    }
    if (m_pSecond->HasGlyph(c)) {
        return m_pSecond;
    }
    return m_IsAlternateFirst ? m_pFirst : m_pSecond;
}

// 0x00745784 slot 0x08
int nw::font::PairFont::GetWidth() const
{
    int second = m_pSecond->GetWidth();
    int first = m_pFirst->GetWidth();
    return first < second ? second : first;
}

// 0x00745864 slot 0x0C
int nw::font::PairFont::GetHeight() const
{
    int second = m_pSecond->GetHeight();
    int first = m_pFirst->GetHeight();
    return first < second ? second : first;
}

// 0x00745808 slot 0x10
int nw::font::PairFont::GetAscent() const
{
    const Font* pFont = m_pFirst->GetHeight() >= m_pSecond->GetHeight() ? m_pFirst : m_pSecond;
    return pFont->GetAscent();
}

// 0x00745330 slot 0x14
int nw::font::PairFont::GetDescent() const
{
    const Font* pFont = m_pFirst->GetHeight() >= m_pSecond->GetHeight() ? m_pFirst : m_pSecond;
    return pFont->GetDescent();
}

// 0x007455F8 slot 0x18
int nw::font::PairFont::GetMaxCharWidth() const
{
    int second = m_pSecond->GetMaxCharWidth();
    int first = m_pFirst->GetMaxCharWidth();
    return first < second ? second : first;
}

// 0x007456F0 slot 0x1C
nw::font::Font::Type nw::font::PairFont::GetType() const
{
    return TYPE_PAIR;
}

// 0x00745640 slot 0x20
int nw::font::PairFont::GetTextureFormat() const
{
    return m_pFirst->GetTextureFormat();
}

// 0x0074538C slot 0x24
int nw::font::PairFont::GetLineFeed() const
{
    int second = m_pSecond->GetLineFeed();
    int first = m_pFirst->GetLineFeed();
    return first < second ? second : first;
}

// 0x00745650 slot 0x28
const nw::font::CharWidths nw::font::PairFont::GetDefaultCharWidths() const
{
    const Font* pFont = m_pFirst->GetWidth() >= m_pSecond->GetWidth() ? m_pFirst : m_pSecond;
    return pFont->GetDefaultCharWidths();
}

// 0x004D9620 slot 0x2C
void nw::font::PairFont::SetLineFeed(int linefeed)
{
    Font* pFont = m_pFirst->GetHeight() >= m_pSecond->GetHeight() ? m_pFirst : m_pSecond;
    pFont->SetLineFeed(linefeed);
}

// 0x004D972C slot 0x30
void nw::font::PairFont::SetDefaultCharWidths(const CharWidths& widths)
{
    m_pFirst->SetDefaultCharWidths(widths);
    m_pSecond->SetDefaultCharWidths(widths);
}

// 0x004D9688 slot 0x34
bool nw::font::PairFont::SetAlternateChar(u16 c)
{
    bool isFirst = m_pFirst->SetAlternateChar(c);
    bool isSecond = m_pSecond->SetAlternateChar(c);
    if (isFirst) {
        m_IsAlternateFirst = true;
        return true;
    }
    if (isSecond) {
        m_IsAlternateFirst = false;
        return true;
    }
    return false;
}

// 0x00745420 slot 0x38 | slot vf_0x38 of nw::font::Font
int nw::font::PairFont::GetCharWidth(u16 c) const
{
    return GetFontFor(c)->GetCharWidth(c);
}

// 0x007454FC slot 0x3C | slot vf_0x3C of nw::font::Font
const nw::font::CharWidths nw::font::PairFont::GetCharWidths(u16 c) const
{
    return GetFontFor(c)->GetCharWidths(c);
}

// 0x007456F8 slot 0x40 | slot vf_0x40 of nw::font::Font
void nw::font::PairFont::GetGlyph(nw::font::Glyph* pGlyph, u16 c) const
{
    GetFontFor(c)->GetGlyph(pGlyph, c);
}

// 0x007457BC slot 0x44
bool nw::font::PairFont::HasGlyph(u16 c) const
{
    return m_pFirst->HasGlyph(c) || m_pSecond->HasGlyph(c);
}

// 0x00745630 slot 0x48
nw::font::Font::CharacterCode nw::font::PairFont::GetCharacterCode() const
{
    return m_pFirst->GetCharacterCode();
}

// 0x0074559C slot 0x4C
int nw::font::PairFont::GetBaselinePos() const
{
    const Font* pFont = m_pFirst->GetHeight() >= m_pSecond->GetHeight() ? m_pFirst : m_pSecond;
    return pFont->GetBaselinePos();
}

// 0x007454A0 slot 0x50
int nw::font::PairFont::GetCellHeight() const
{
    const Font* pFont = m_pFirst->GetHeight() >= m_pSecond->GetHeight() ? m_pFirst : m_pSecond;
    return pFont->GetCellHeight();
}

// 0x007453C4 slot 0x54
int nw::font::PairFont::GetCellWidth() const
{
    const Font* pFont = m_pFirst->GetWidth() >= m_pSecond->GetWidth() ? m_pFirst : m_pSecond;
    return pFont->GetCellWidth();
}

// 0x004D96EC slot 0x58
void nw::font::PairFont::SetLinearFilter(bool atSmall, bool atLarge)
{
    m_pFirst->SetLinearFilter(atSmall, atLarge);
    m_pSecond->SetLinearFilter(atSmall, atLarge);
}

// 0x007456E0 slot 0x5C
bool nw::font::PairFont::IsLinearFilterEnableAtSmall() const
{
    return m_pFirst->IsLinearFilterEnableAtSmall();
}

// 0x007456D0 slot 0x60
bool nw::font::PairFont::IsLinearFilterEnableAtLarge() const
{
    return m_pFirst->IsLinearFilterEnableAtLarge();
}

// 0x007456C0 slot 0x64
u32 nw::font::PairFont::GetTextureWrapFilterValue() const
{
    return m_pFirst->GetTextureWrapFilterValue();
}

} // namespace font
} // namespace nw
