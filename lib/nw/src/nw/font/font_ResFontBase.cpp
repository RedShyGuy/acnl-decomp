#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nw/font/font_Glyph.h"
#include "nw/font/font_ResFontBase.h"
#include "nw/font/internal/font_TextureObject.h"

namespace nw {
namespace font {
// 0x004D88B0 (name is ours)
nw::font::ResFontBase::ResFontBase()
    : m_pResource(nullptr), m_pFontInfo(nullptr), m_pTextureObjects(nullptr), m_Filter(FILTER_LINEAR_AT_SMALL | FILTER_LINEAR_AT_LARGE),
      m_LastCharCode(0), m_LastGlyphIndex(GLYPH_INDEX_NOT_FOUND)
{
}

// 0x0013F634 slot 0x00
// 0x004D88EC (deleting dtor)
nw::font::ResFontBase::~ResFontBase()
{
}

// 0x004D8788 (name is ours)
void nw::font::ResFontBase::SetResourceBuffer(void* pResource, FontInformation* pFontInfo)
{
    m_pResource = pResource;
    m_pFontInfo = pFontInfo;
}

// 0x001426DC | nintendogs:bytes-fuzzy [tier A]
void nw::font::ResFontBase::GenTextureNames()
{
    const FontTextureGlyph* pGlyph = m_pFontInfo->pGlyph;
    internal::TextureObject* pObjects = m_pTextureObjects;
    const int sheetNum = GetNumTextureObject();
    u32 offset = 0;
    for (int i = 0; i < sheetNum; i++) {
        pObjects[i].Set(0, this, pGlyph->sheetImage + offset, pGlyph->sheetFormat, pGlyph->sheetWidth, pGlyph->sheetHeight);
        offset += pGlyph->sheetSize;
    }
    // the sheets are written back to the memory the GPU reads
    nngxlowStoreDataCache(reinterpret_cast<uptr>(pGlyph->sheetImage), pGlyph->sheetSize * sheetNum);
}

// 0x0013F5DC | nintendogs:bytes [tier A]
void nw::font::ResFontBase::DeleteTextureNames()
{
    internal::TextureObject* pObjects = m_pTextureObjects;
    const int sheetNum = GetNumTextureObject();
    for (int i = 0; i < sheetNum; i++) {
        GLuint name = pObjects[i].m_Name;
        if (name != 0) {
            glDeleteTextures(1, &name);
            pObjects[i].m_Name = 0;
        }
    }
}

// 0x007452A0 slot 0x08
int nw::font::ResFontBase::GetWidth() const
{
    return m_pFontInfo->width;
}

// 0x007452D4 slot 0x0C
int nw::font::ResFontBase::GetHeight() const
{
    return m_pFontInfo->height;
}

// 0x007452C8 slot 0x10
int nw::font::ResFontBase::GetAscent() const
{
    return m_pFontInfo->ascent;
}

// 0x00744EF4 slot 0x14
int nw::font::ResFontBase::GetDescent() const
{
    return m_pFontInfo->height - m_pFontInfo->ascent;
}

// 0x007450D8 slot 0x18
int nw::font::ResFontBase::GetMaxCharWidth() const
{
    return m_pFontInfo->pGlyph->maxCharWidth;
}

// 0x00745298 slot 0x1C
nw::font::Font::Type nw::font::ResFontBase::GetType() const
{
    return TYPE_RESOURCE;
}

// 0x007450F4 slot 0x20
int nw::font::ResFontBase::GetTextureFormat() const
{
    return m_pFontInfo->pGlyph->sheetFormat;
}

// 0x00744F08 slot 0x24
int nw::font::ResFontBase::GetLineFeed() const
{
    return m_pFontInfo->linefeed;
}

// 0x00745264 slot 0x28
const nw::font::CharWidths nw::font::ResFontBase::GetDefaultCharWidths() const
{
    return m_pFontInfo->defaultWidth;
}

// 0x004D8754 slot 0x2C
void nw::font::ResFontBase::SetLineFeed(int linefeed)
{
    m_pFontInfo->linefeed = static_cast<s8>(linefeed);
}

// 0x004D8898 slot 0x30
void nw::font::ResFontBase::SetDefaultCharWidths(const CharWidths& widths)
{
    m_pFontInfo->defaultWidth = widths;
}

// 0x004D8760 slot 0x34
bool nw::font::ResFontBase::SetAlternateChar(u16 c)
{
    u16 index = FindGlyphIndex(c);
    if (index != GLYPH_INDEX_NOT_FOUND) {
        m_pFontInfo->alterCharIndex = index;
        return true;
    }
    return false;
}

// 0x00744F24 slot 0x38 | nintendogs:bytes
int nw::font::ResFontBase::GetCharWidth(u16 c) const
{
    return GetCharWidths(c).charWidth;
}

// (inline)
DECOMP_ALWAYS_INLINE const nw::font::CharWidths& nw::font::ResFontBase::GetCharWidthsFromIndex(u16 index) const
{
    for (const FontWidth* pWidth = m_pFontInfo->pWidth; pWidth != nullptr; pWidth = pWidth->pNext) {
        if (pWidth->indexBegin <= index && index <= pWidth->indexEnd) {
            return pWidth->widthTable[index - pWidth->indexBegin];
        }
    }
    return m_pFontInfo->defaultWidth;
}

// 0x00744F50 slot 0x3C | nintendogs:bytes
const nw::font::CharWidths nw::font::ResFontBase::GetCharWidths(u16 c) const
{
    u16 index = FindGlyphIndex(c);
    if (index == GLYPH_INDEX_NOT_FOUND) {
        index = m_pFontInfo->alterCharIndex;
    }
    return GetCharWidthsFromIndex(index);
}

// 0x00745114 slot 0x40 | nintendogs:bytes
void nw::font::ResFontBase::GetGlyph(nw::font::Glyph* pGlyph, u16 c) const
{
    u16 index = FindGlyphIndex(c);
    if (index == GLYPH_INDEX_NOT_FOUND) {
        index = m_pFontInfo->alterCharIndex;
    }
    GetGlyphFromIndex(pGlyph, index);
}

// 0x007452AC slot 0x44
bool nw::font::ResFontBase::HasGlyph(u16 c) const
{
    return FindGlyphIndex(c) != GLYPH_INDEX_NOT_FOUND;
}

// 0x007450E8 slot 0x48
nw::font::Font::CharacterCode nw::font::ResFontBase::GetCharacterCode() const
{
    return static_cast<CharacterCode>(m_pFontInfo->characterCode);
}

// 0x007450C8 slot 0x4C
int nw::font::ResFontBase::GetBaselinePos() const
{
    return m_pFontInfo->pGlyph->baselinePos;
}

// 0x00744F40 slot 0x50
int nw::font::ResFontBase::GetCellHeight() const
{
    return m_pFontInfo->pGlyph->cellHeight;
}

// 0x00744F14 slot 0x54
int nw::font::ResFontBase::GetCellWidth() const
{
    return m_pFontInfo->pGlyph->cellWidth;
}

// 0x004D8794 slot 0x58
void nw::font::ResFontBase::SetLinearFilter(bool atSmall, bool atLarge)
{
    m_Filter = (atSmall ? FILTER_LINEAR_AT_SMALL : 0) | (atLarge ? FILTER_LINEAR_AT_LARGE : 0);
    if (m_pResource != nullptr) {
        // the textures are made again with the new filter
        DeleteTextureNames();
        GenTextureNames();
    }
}

// 0x00745288 slot 0x5C
bool nw::font::ResFontBase::IsLinearFilterEnableAtSmall() const
{
    return (m_Filter & FILTER_LINEAR_AT_SMALL) != 0;
}

// 0x00745278 slot 0x60
bool nw::font::ResFontBase::IsLinearFilterEnableAtLarge() const
{
    return (m_Filter & FILTER_LINEAR_AT_LARGE) != 0;
}

// 0x00745270 slot 0x64
u32 nw::font::ResFontBase::GetTextureWrapFilterValue() const
{
    return m_Filter;
}

// 0x00745104 slot 0x68
int nw::font::ResFontBase::GetNumTextureObject() const
{
    return m_pFontInfo->pGlyph->sheetNum;
}

// (inline)
DECOMP_ALWAYS_INLINE u16 nw::font::ResFontBase::FindGlyphIndex(const FontCodeMap* pMap, u16 c) const
{
    u16 index = GLYPH_INDEX_NOT_FOUND;
    switch (pMap->mappingMethod) {
    case FontCodeMap::METHOD_DIRECT:
        index = static_cast<u16>(pMap->mapInfo[0] + (c - pMap->ccodeBegin));
        break;
    case FontCodeMap::METHOD_TABLE:
        index = pMap->mapInfo[c - pMap->ccodeBegin];
        break;
    case FontCodeMap::METHOD_SCAN: {
        // binary search in the pairs (code, index)
        struct CMapScanEntry
        {
            u16 ccode;
            u16 index;
        };
        const CMapScanEntry* pFirst = reinterpret_cast<const CMapScanEntry*>(&pMap->mapInfo[1]);
        const CMapScanEntry* pLast = pFirst + pMap->mapInfo[0] - 1;
        while (pFirst <= pLast) {
            const CMapScanEntry* pMiddle = pFirst + (pLast - pFirst) / 2;
            if (pMiddle->ccode < c) {
                pFirst = pMiddle + 1;
            } else if (c < pMiddle->ccode) {
                pLast = pMiddle - 1;
            } else {
                index = pMiddle->index;
                break;
            }
        }
        break;
    }
    default:
        break;
    }
    return index;
}

// 0x00744FC8 | nintendogs:bytes [tier A]
u16 nw::font::ResFontBase::FindGlyphIndex(u16 c) const
{
    if (c == m_LastCharCode) {
        return m_LastGlyphIndex;
    }
    m_LastCharCode = c;
    u16 index = GLYPH_INDEX_NOT_FOUND;
    for (const FontCodeMap* pMap = m_pFontInfo->pMap; pMap != nullptr; pMap = pMap->pNext) {
        if (pMap->ccodeBegin <= c && c <= pMap->ccodeEnd) {
            index = FindGlyphIndex(pMap, c);
            break;
        }
    }
    m_LastGlyphIndex = index;
    return index;
}

// 0x0074514C | nintendogs:bytes [tier A]
void nw::font::ResFontBase::GetGlyphFromIndex(nw::font::Glyph* pGlyph, u16 index) const
{
    const FontTextureGlyph* pTexGlyph = m_pFontInfo->pGlyph;
    const u32 cellsPerSheet = pTexGlyph->sheetRow * pTexGlyph->sheetLine;
    const u32 sheetNo = index / cellsPerSheet;
    pGlyph->m_pTexture = pTexGlyph->sheetImage + pTexGlyph->sheetSize * sheetNo;
    pGlyph->m_Widths = GetCharWidthsFromIndex(index);
    pGlyph->m_pTextureObject = m_pTextureObjects != nullptr ? &m_pTextureObjects[sheetNo] : nullptr;

    const u32 cellNo = index % cellsPerSheet;
    const u32 cellUnitY = cellNo / pTexGlyph->sheetRow;
    const u32 cellUnitX = cellNo % pTexGlyph->sheetRow;
    pGlyph->m_Height = pTexGlyph->cellHeight;
    pGlyph->m_TexFormat = pTexGlyph->sheetFormat;
    pGlyph->m_TexWidth = pTexGlyph->sheetWidth;
    pGlyph->m_TexHeight = pTexGlyph->sheetHeight;
    // the cells have a border of one pixel
    pGlyph->m_CellX = static_cast<u16>(cellUnitX * (pTexGlyph->cellWidth + 1) + 1);
    pGlyph->m_CellY = static_cast<u16>(cellUnitY * (pTexGlyph->cellHeight + 1) + 1);
}

} // namespace font
} // namespace nw
