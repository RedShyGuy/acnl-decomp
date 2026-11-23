#pragma once

// Custom designs ("My Design" patterns).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvPersonalId.h"

struct TextureImage16;
namespace script {
class IWord;
} // namespace script

// The functions of the designs (dSvMyDesign.cpp; names are ours). The pattern is four parts of 32x32 color indices (four
// bits each, 15 = transparent) in words: part * 128 + y * 4 + x / 8, the nibble x % 8. The color indices point into the
// palette of 15 colors of the design, those into the color table s_Colors (RGBA4, MyDesign/plt/picker.bin).
struct SvMyDesign {
    // the region of a part of the pattern for a kind of design (name is ours)
    struct PartRegion
    {
        u32 row;           // 0x00 the first row
        u32 part;          // 0x04
        u32 width;         // 0x08
        u32 height;        // 0x0C
        bool isRotated;    // 0x10 (the parts 2 and 3 of the kinds below 6: the sleeves)
        bool hasExtra;     // 0x11 a second region of 16 columns in part 3
        u32 extraRow;      // 0x14
    };
    ASSERT_SIZE(PartRegion, 0x18);

    // patternType
    static const u8 PATTERN_TYPE_SPECIAL = 8; // (the shape of s_SpecialMask)
    static const u8 PATTERN_TYPE_NORMAL = 9;  // (the type of a new design)
    // mainColors: no second color
    static const u8 MAIN_COLOR_NONE = 12;

    // (the destructor is empty, armlink shares its bx lr with other empty functions)
    SvMyDesign(); // 0x001B4F3C
    ~SvMyDesign(); // 0x0013AB74
    // the design is cleared (game code, not decompiled yet; name is ours)
    void Clear(); // 0x00305B40

    // the two most frequent kinds of colors of the pattern (s_ColorKinds) become mainColors
    void UpdateMainColors(); // 0x001B3238
    // the palette of the first palette, the pattern filled with color 12
    void ResetPattern(); // 0x001B36E8
    // color (an index of s_Colors) for palette index (only opaque colors; transparent ones become 0)
    void SetPaletteColor(u32 index, u8 color); // 0x001B3730
    // the header and the first part of src, the rest of the pattern filled with color 12
    void CopyFirstPart(const SvMyDesign* src); // 0x001B3884
    // the palette of the palettes s_Palettes
    void SetPalette(u32 palette); // 0x001B4028
    // the color index at x, y of part
    void SetPixel(u32 colorIndex, u32 x, u32 y, u32 part); // 0x001B4E48
    // a copy of src (the flag by FindDesignId, main colors)
    void CopyFrom(const SvMyDesign* src); // 0x001B4EC8
    // a copy of src for this design in the save data (textures, network packet 109)
    void Assign(const SvMyDesign* src); // 0x001B4F6C

    // the pixels of part of an image (RGBA4 color indices in the low nibble; the special shape: all parts) become the pattern
    // and are drawn into the textures of the design
    void ReadFromTexture(const void* resTexture, u32 part); // 0x001B38BC
    void ReadFromImage(const TextureImage16* image, u32 part); // 0x001B38FC
    // color at x, y of a texture of the design (the normal kinds: in all four rows of parts of a 128 high texture)
    void DrawPixel(const void* resTexture, u16 color, u32 x, u32 y); // 0x001B3B6C
    // a new design of the current player / of the player of its design id (only in a network game): palette 0, pattern
    // color 12, the title "STR_Mydesign" 30, network packet 109 with bit 14
    void InitNew(); // 0x001B3C40
    void InitNewForNetwork(); // 0x001B4310
    // the template file "MyDesign/tex/myD_%02d.bin" of kind (22: 20)
    void LoadTemplate(u32 kind); // 0x001B3E48
    // the title from word
    void SetTitle(const script::IWord& word); // 0x001B3F90
    // the parts that the pattern type does not use are filled with color 12
    void FillUnusedParts(); // 0x001B406C
    // the current player becomes the creator
    void SetCreatorToCurrentPlayer(); // 0x001B42E8
    // the template of the design id (with the creator "Name" of the town of the region; kind: not read, the town flag
    // passes 22)
    void ResetToTemplate(u32 kind); // 0x001B44E0
    // the two designs change places (textures, network packet 109 with bit 15)
    void Swap(SvMyDesign* other); // 0x001B4920
    // the pattern (packed nibbles, high nibble first), the palette, the title, the town, the creator name (as message
    // indices of "STR_Mydesign") and the kind from a template file
    void LoadFile(const char* path); // 0x001B4AB8
    // the town of the creator becomes town if the creator is player
    void UpdateCreatorTown(const SvPersonalId* player, const SvTownId* town); // 0x001B4E98
    // (copies of inline functions for the game, in the area 0x70C2xx)
    // the pattern into an image texture (isSmall: one row of 32, else the four parts in rows; the kinds of 9 and more draw
    // part 0 in every row); the second copy draws two pixels per step
    void DrawToTexture(const void* resTexture, bool isSmall); // 0x0070C44C
    void DrawToTextureByPairs(const void* resTexture, bool isSmall); // 0x0070C8C4
    // part of the pattern into an image texture with the color index in the low nibble (the special shape: all parts; see
    // ReadFromImage)
    void WriteToTexture(const void* resTexture, u32 part); // 0x0070C55C
    // the RGBA4 value of the color index with the index in the low nibble (15: transparent)
    u16 GetPixelValue(u32 index) const; // 0x0070C82C
    // the kind of the design: a hat (6, 7, else 11) / clothes (0..5, else 10) / clothes or a hat (below 9)
    u8 GetHatType() const; // 0x0070C85C
    u8 GetClothesType() const; // 0x0070C870
    bool IsWearable() const; // 0x0070C880
    // the creator is the current player
    bool IsCreatedByCurrentPlayer() const; // 0x0070C898

    // the color tables are loaded (MyDesign/plt/picker.bin, MyDesign/plt/palette.bin)
    static void LoadColorTables(); // 0x001B3760
    // the color of index of a set of the picker (0: transparent colors)
    static u8 GetPickerColor(u32 set, u32 index); // 0x001B3FDC
    // the color of index of a palette (0 out of range)
    static u8 GetPaletteColor(u32 palette, u32 index); // 0x001B4000
    // the RGBA4 value of color (opaque: alpha 15)
    static u16 GetColorValue(u32 color, bool isOpaque); // 0x001B42C4

    // (inline copies for the game) the color of palette index; the region of part for patternType (false: the part is
    // not used); the design has the special shape
    DECOMP_EMIT_COPY u8 GetColor(u32 index) const { return palette[index]; } // 0x0070C32C
    DECOMP_EMIT_COPY DECOMP_NOINLINE inline bool GetPartRegion(PartRegion* region, u32 part) const; // 0x0070C338
    DECOMP_EMIT_COPY bool IsSpecial() const { return patternType == PATTERN_TYPE_SPECIAL; } // 0x0070C438

    // the special shape: the first column and row of each part, the masked pixels (bit x of row part * 32 + y) and the
    // origin of each part
    static const u8 s_SpecialPartOffsets[4][2];
    static const u32 s_SpecialMask[128];
    static const u32 s_SpecialPartOrigins[4][2];
    // the colors (RGBA4, alpha 0) and the palettes (12 x 15 colors)
    static u16 s_Colors[256];
    static u8 s_Palettes[12][15];
    // the kind of each color (UpdateMainColors) and the pixels of each pattern type
    static const u8 s_ColorKinds[256];
    static const u16 s_PixelCounts[12];

    /* 0x0000 */ char16 title[0x15];
    /* 0x002A */ SvPersonalId creatorData;
    /* 0x0058 */ u8 palette[0xF];
    /* 0x0067 */ u8 mainColors; // the most frequent kinds of colors (UpdateMainColors: low nibble first, high second, 12 none; research name: UnusedChecksum, default 1)
    /* 0x0068 */ u8 tenConstant; // seems to always be 0x0A
    /* 0x0069 */ u8 patternType;
    /* 0x006A */ u16 pad_0x6A; // Zero Padding; Always 0x0000
    /* 0x006C */ u8 patternData1[0x200]; // mandatory
    /* 0x026C */ u8 patternData2[0x200]; // optional
    /* 0x046C */ u8 patternData3[0x200]; // optional
    /* 0x066C */ u8 patternData4[0x200]; // optional
    /* 0x086C */ u8 isGuestDesign; // a design of a player changed on this machine as a guest of a network game (kind 0 of FindDesignId)
    /* 0x086D */ u8 pad_0x86D[3];
};
ASSERT_SIZE(SvMyDesign, 0x870);

inline bool SvMyDesign::GetPartRegion(PartRegion* region, u32 part) const
{
    if (region == NULL) {
        return false;
    }
    region->width = 32;
    if (patternType >= 6) {
        region->row = 0;
        region->part = 0;
        region->height = 32;
        region->isRotated = false;
        region->hasExtra = false;
        return true;
    }
    if (part == 2 || part == 3) {
        if (patternType == 2 || patternType == 5) {
            return false;
        }
        region->height = 16;
        region->isRotated = true;
        region->part = 2;
        region->row = part == 2 ? 16 : 0;
        region->hasExtra = false;
        if (patternType == 1 || patternType == 4) {
            region->width = 16;
        }
        return true;
    }
    region->isRotated = false;
    region->row = 0;
    region->height = 32;
    if (patternType <= 2) {
        region->hasExtra = true;
        region->extraRow = part != 0 ? 16 : 0;
        region->part = part != 0 ? 1 : 0;
    } else {
        region->hasExtra = false;
        region->part = part != 0 ? 1 : 0;
    }
    return true;
}

// The designs in the save data by an id and the textures that show them (the file after MenuBase; not decompiled yet, names
// are ours). The id: kind << 8 | sub << 4 | index (0xFFFF: none); kinds 0 the designs of player sub, 1 the design of
// villager sub, 2 the displays of the Able Sisters, 3 the designs of Re-Tail, 4 the design of museum exhibit sub, 5 the town
// flag, 6 the design stands.
u16 FindDesignId(const SvMyDesign* design); // 0x006AB738
// the textures of the item loaders that show design are updated
void UpdateDesignTextures(SvMyDesign* design); // 0x006ABB88
// (only kind 0) the design index of player sub changed
void OnDesignChanged(u16 id, bool isChanged); // 0x006ABA70
// the pixel x, y (y: row of parts * 32 + row) of the textures of the item loaders that show design
void DrawDesignPixel(SvMyDesign* design, u16 color, u32 x, u32 y); // 0x006AB530
namespace item {
class ResLoader;
} // namespace item
// (names are ours) an item loader shows the design of its m_pDesign (until it is unregistered); the design of an id; a copy /
// a swap of designs with their textures (network packet 109)
void RegisterDesignLoader(item::ResLoader* loader); // 0x006AB518
void UnregisterDesignLoader(item::ResLoader* loader); // 0x006AB6F0
SvMyDesign* GetDesign(u16 id); // 0x006AB5C0
void AssignDesign(SvMyDesign* design, const SvMyDesign* src); // 0x006AB940
void SwapDesigns(SvMyDesign* a, SvMyDesign* b); // 0x006AB95C
// (the designs of the current player changed as a guest: their bits to Func4E514C, the flags cleared; false: none)
bool Func6ABAE4(); // 0x006ABAE4

// (game code, not decompiled yet; names after the addresses) the designs of a player: the design of an index (two copies),
// the flags isGuestDesign cleared, one of them is set
void* Func20C50C(SvMyDesign* designs, u32 index); // 0x0020C50C
void Func20C524(SvMyDesign* designs); // 0x0020C524
bool Func7133CC(const SvMyDesign* designs); // 0x007133CC
// (OnDesignChanged: the design index of player sub changed)
void Func24F5CC(u32 sub, u32 index, bool isValidPlayer); // 0x0024F5CC
void Func4E638C(u32 sub, u32 index, bool isLocal); // 0x004E638C
void Func4E514C(const u8* bits); // 0x004E514C
