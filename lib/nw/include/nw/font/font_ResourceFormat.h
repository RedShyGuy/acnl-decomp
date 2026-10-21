#pragma once

#include "decomp.h"

// The blocks of a font resource (BCFNT, after 3dbrew "BCFNT"): FINF, TGLP, CWDH and CMAP. After
// ResFont::SetResource the offsets in the blocks are pointers. The structure and member names
// are ours.
namespace nw {
namespace font {
struct FontTextureGlyph;
struct FontWidth;
struct FontCodeMap;

// the widths of a character
struct CharWidths
{
    s8 left;        // 0x0 the space left of the glyph
    u8 glyphWidth;  // 0x1
    s8 charWidth;   // 0x2 the advance
};
ASSERT_SIZE(CharWidths, 0x3);

// FINF: the information about the font
struct FontInformation
{
    u8 fontType;                 // 0x00
    s8 linefeed;                 // 0x01
    u16 alterCharIndex;          // 0x02 the glyph of characters that are not in the font
    CharWidths defaultWidth;     // 0x04
    u8 characterCode;            // 0x07 (Font::CharacterCode)
    FontTextureGlyph* pGlyph;    // 0x08
    FontWidth* pWidth;           // 0x0C
    FontCodeMap* pMap;           // 0x10
    u8 height;                   // 0x14
    u8 width;                    // 0x15
    u8 ascent;                   // 0x16
    u8 padding;                  // 0x17
};
ASSERT_SIZE(FontInformation, 0x18);

// TGLP: the glyph images, cells on one or more sheets
struct FontTextureGlyph
{
    u8 cellWidth;        // 0x00
    u8 cellHeight;       // 0x01
    s8 baselinePos;      // 0x02
    u8 maxCharWidth;     // 0x03
    u32 sheetSize;       // 0x04
    u16 sheetNum;        // 0x08
    u16 sheetFormat;     // 0x0A
    u16 sheetRow;        // 0x0C the cells in a row
    u16 sheetLine;       // 0x0E the rows
    u16 sheetWidth;      // 0x10
    u16 sheetHeight;     // 0x12
    u8* sheetImage;      // 0x14
};
ASSERT_SIZE(FontTextureGlyph, 0x18);

// CWDH: the widths of a range of glyph indices
struct FontWidth
{
    u16 indexBegin;          // 0x0
    u16 indexEnd;            // 0x2
    FontWidth* pNext;        // 0x4
    CharWidths widthTable[1]; // 0x8
};

// CMAP: the glyph indices of a range of character codes
struct FontCodeMap
{
    // the kinds of mapping
    enum MappingMethod
    {
        METHOD_DIRECT, // mapInfo[0] is the index of ccodeBegin
        METHOD_TABLE,  // mapInfo[c - ccodeBegin]
        METHOD_SCAN    // mapInfo[0] pairs of (code, index) follow, sorted by code
    };

    u16 ccodeBegin;      // 0x0
    u16 ccodeEnd;        // 0x2
    u16 mappingMethod;   // 0x4
    u16 reserved;        // 0x6
    FontCodeMap* pNext;  // 0x8
    u16 mapInfo[1];      // 0xC
};

// the sheet formats of TGLP (names are ours)
enum FontSheetFormat
{
    FONT_SHEET_FORMAT_RGBA8,
    FONT_SHEET_FORMAT_RGB8,
    FONT_SHEET_FORMAT_RGBA5551,
    FONT_SHEET_FORMAT_RGB565,
    FONT_SHEET_FORMAT_RGBA4,
    FONT_SHEET_FORMAT_LA8,
    FONT_SHEET_FORMAT_HILO8,
    FONT_SHEET_FORMAT_L8,
    FONT_SHEET_FORMAT_A8,
    FONT_SHEET_FORMAT_LA4,
    FONT_SHEET_FORMAT_L4,
    FONT_SHEET_FORMAT_A4
};
} // namespace font
} // namespace nw
