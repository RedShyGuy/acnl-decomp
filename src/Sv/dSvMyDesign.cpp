#include "Sv/dSvMyDesign.h"

#include <cstring>
#include "netgame/dAvatarId.h"
#include "netgame/dGameFunctions.h"
#include "netgame/netgame_Api.h"
#include "Other/dRegion.h"
#include "Other/dTextureImage.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "script/dStrMgr.h"
#include "script/dWordFix.h"
#include "script/dWordPtrSv.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "ssys/ma/dFileData.h"

// 0x0095D030
u16 SvMyDesign::s_Colors[256] = {0xFFFF};
// 0x00AD63A4
u8 SvMyDesign::s_Palettes[12][15];

namespace {
// the pattern data: 32 x 32 nibbles per part in words
const u32 WORDS_PER_PART = 128;
const u32 WORDS_PER_ROW = 4;
// the color index of a transparent pixel
const u32 COLOR_INDEX_TRANSPARENT = 15;
// the color index the pattern is filled with (ResetPattern)
const u32 COLOR_INDEX_FILL = 12;
const u8 PATTERN_FILL = 0xCC;
// the network packet of a changed design (FindDesignId of the old and the new one; bit 15: swapped, bit 14: reset)
const u8 PACKET_DESIGN = 109;

// the color index at x, y of part
inline u32 GetColorIndex(const SvMyDesign* design, u32 x, u32 y, u32 part)
{
    const u32* words = reinterpret_cast<const u32*>(design->patternData1);
    const u32 word = words[part * WORDS_PER_PART + (y & 31) * WORDS_PER_ROW + ((x & 31) >> 3)];
    return (word >> ((x & 7) * 4)) & 15;
}

// the color counts of the kinds by the color at x, y of part (transparent counts as color index 0)
inline void CountColor(const SvMyDesign* design, u16* counts, u32 x, u32 y, u32 part)
{
    u32 index = GetColorIndex(design, x, y, part);
    if (index == COLOR_INDEX_TRANSPARENT) {
        index = 0;
    }
    counts[SvMyDesign::s_ColorKinds[design->palette[index]]]++;
}

// the share of total (rounded down when it is nearly a whole number)
inline f32 GetShare(u32 total, f32 factor)
{
    const f32 share = static_cast<f32>(total) * factor;
    const u32 whole = share;
    const f32 rest = share - static_cast<f32>(whole);
    return rest <= 0.0002f ? static_cast<f32>(whole) : share;
}
} // namespace

// the kind of each color (UpdateMainColors)
// 0x0088F8D0
extern const u8 SvMyDesign::s_ColorKinds[256] = {
    10, 10, 10, 10, 2,  8,  8,  2,  9,  0, 0, 0, 0, 0, 0, 6,  10, 10, 2,  2,  2,  9,  2,  2,  2,  0, 0, 0, 0, 0, 0, 6,
    11, 1,  3,  3,  3,  9,  3,  9,  9,  0, 0, 0, 0, 0, 0, 6,  14, 14, 14, 14, 14, 9,  9,  9,  9,  0, 0, 0, 0, 0, 0, 11,
    10, 8,  8,  8,  8,  11, 8,  8,  7,  0, 0, 0, 0, 0, 0, 11, 10, 10, 8,  10, 10, 11, 8,  8,  8,  0, 0, 0, 0, 0, 0, 11,
    14, 9,  9,  9,  9,  9,  9,  7,  7,  0, 0, 0, 0, 0, 0, 11, 1,  1,  1,  1,  1,  1,  9,  9,  9,  0, 0, 0, 0, 0, 0, 11,
    8,  8,  8,  8,  8,  8,  8,  8,  7,  0, 0, 0, 0, 0, 0, 11, 5,  5,  5,  5,  5,  5,  5,  5,  7,  0, 0, 0, 0, 0, 0, 11,
    4,  4,  4,  4,  4,  4,  4,  4,  7,  0, 0, 0, 0, 0, 0, 11, 4,  4,  4,  4,  4,  11, 4,  4,  4,  0, 0, 0, 0, 0, 0, 11,
    13, 13, 5,  13, 5,  5,  5,  5,  7,  0, 0, 0, 0, 0, 0, 7,  13, 13, 13, 13, 13, 13, 5,  5,  7,  0, 0, 0, 0, 0, 0, 7,
    4,  4,  4,  4,  13, 11, 4,  4,  4,  0, 0, 0, 0, 0, 0, 7,  4,  4,  4,  4,  4,  4,  4,  4,  4,  0, 0, 0, 0, 0, 0, 0,
};
// 0x0088F8C8
extern const u8 SvMyDesign::s_SpecialPartOffsets[4][2] = {{0, 0}, {0, 32}, {32, 0}, {32, 32}};
// 0x0088F9D0
extern const u32 SvMyDesign::s_SpecialMask[128] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3FC00000, 0xFFF80000, 0xFFFE0000,
    0xFFFF0000, 0xFFFF8000, 0xFFFFC000, 0xFFFFC000, 0xFFFFE000, 0xFFFFE000, 0xFFFFE000, 0xFFFFE000,
    0xFFFFE000, 0xFFFFC000, 0xFFFFC000, 0xFFFF8000, 0xFFFF0000, 0xFFFC0000, 0xFFF00000, 0x1F800000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000007,
    0x0000000F, 0x0000001F, 0x0000003F, 0x0000003F, 0x0000007F, 0x0000007F, 0x0000007F, 0x0000007F,
    0x0000007F, 0x0000003F, 0x0000001F, 0x0000001F, 0x0000000F, 0x00000003, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};
// 0x0088FBE8
extern const u32 SvMyDesign::s_SpecialPartOrigins[4][2] = {{0, 0}, {0, 32}, {32, 0}, {32, 32}};
// the pixels of each pattern type
// 0x0088FBD0
extern const u16 SvMyDesign::s_PixelCounts[12] = {4096, 3584, 3072, 3072, 2560, 2048, 1024, 1024, 2945, 1024, 1024, 1024};

// 0x001B3238
void SvMyDesign::UpdateMainColors()
{
    u16 counts[16] = {0};
    const u32 total = s_PixelCounts[patternType];
    if (patternType == PATTERN_TYPE_SPECIAL) {
        for (u32 part = 0; part < 4; part++) {
            for (u32 y = 0; y < 32; y++) {
                for (u32 x = 0; x < 32; x++) {
                    if (s_SpecialPartOffsets[part][0] + x < 52 && (s_SpecialMask[part * 32 + y] & (1 << x)) == 0) {
                        CountColor(this, counts, x, y, part);
                    }
                }
            }
        }
    } else if (patternType >= 6) {
        for (u32 y = 0; y < 32; y++) {
            for (u32 x = 0; x < 32; x++) {
                CountColor(this, counts, x, y, 0);
            }
        }
    } else {
        for (u32 part = 0; part < 4; part++) {
            PartRegion region;
            if (!GetPartRegion(&region, part)) {
                continue;
            }
            for (u32 x = 0; x < region.width; x++) {
                for (u32 y = 0; y < region.height; y++) {
                    CountColor(this, counts, x, region.row + y, region.part);
                }
            }
            if (region.hasExtra) {
                for (u32 x = 0; x < 32; x++) {
                    for (u32 y = 0; y < 16; y++) {
                        CountColor(this, counts, x, region.extraRow + y, 3);
                    }
                }
            }
        }
    }
    u8 first = 0;
    for (u32 i = 0; i < 15; i++) {
        if (counts[i] > counts[first]) {
            first = i;
        }
    }
    u8 second = 0;
    for (u32 i = 0; i < 15; i++) {
        if (i != first && counts[i] > counts[second]) {
            second = i;
        }
    }
    if (counts[first] >= GetShare(total, 0.9f)) {
        mainColors = ((mainColors & 0xF0) | first) & 0xF;
        mainColors |= (first << 4) & 0xF0;
    } else if (counts[first] >= GetShare(total, 0.35f)) {
        mainColors = (mainColors & 0xF0) | (first & 0xF);
        if (counts[second] >= GetShare(total - counts[first], 0.6f)) {
            mainColors = (mainColors & 0xF) | ((second << 4) & 0xF0);
        } else {
            mainColors = (mainColors & 0xF) | (MAIN_COLOR_NONE << 4);
        }
    } else {
        mainColors = (MAIN_COLOR_NONE << 4) | MAIN_COLOR_NONE;
    }
}

// 0x001B36E8
void SvMyDesign::ResetPattern()
{
    SetPalette(0);
    std::memset(patternData1, PATTERN_FILL, sizeof(patternData1) * 4);
}

// 0x001B3730
void SvMyDesign::SetPaletteColor(u32 index, u8 color)
{
    if (index < 15) {
        palette[index] = (s_Colors[color] & 15) != 0 ? 0 : color;
    }
}

// 0x001B3760
void SvMyDesign::LoadColorTables()
{
    ssys::ma::FileData file;
    sead::Heap* heap = sead::HeapMgr::s_pInstance->getCurrentHeap();
    file.Load("MyDesign/plt/picker.bin", heap, 0);
    const u8* data = file.m_pData;
    for (u32 i = 0; i < 256; i++) {
        const u8 low = *data++;
        const u8 high = *data++;
        s_Colors[i] = low | (high << 8);
    }
    file.Unload();
    file.Load("MyDesign/plt/palette.bin", heap, 0);
    const u8* palettes = file.m_pData;
    for (u32 i = 0; i < 12; i++) {
        std::memcpy(s_Palettes[i], palettes, sizeof(s_Palettes[i]));
        palettes += sizeof(s_Palettes[i]);
    }
    file.Unload();
}

// 0x001B3884
void SvMyDesign::CopyFirstPart(const SvMyDesign* src)
{
    std::memset(patternData1, PATTERN_FILL, sizeof(patternData1) * 4);
    std::memcpy(static_cast<void*>(this), src, offsetof(SvMyDesign, patternData2));
    isGuestDesign = false;
}

// 0x001B3FDC
u8 SvMyDesign::GetPickerColor(u32 set, u32 index)
{
    const u8 color = set + index * 16;
    if ((s_Colors[color] & 15) != 0) {
        return 0;
    }
    return color;
}

// 0x001B4000
u8 SvMyDesign::GetPaletteColor(u32 palette, u32 index)
{
    if (palette >= 12 || index >= 15) {
        return 0;
    }
    return s_Palettes[palette][index];
}

// 0x001B4028
void SvMyDesign::SetPalette(u32 palette)
{
    for (u32 i = 0; i < 15; i++) {
        this->palette[i] = GetPaletteColor(palette, i);
    }
}

// 0x001B42C4
u16 SvMyDesign::GetColorValue(u32 color, bool isOpaque)
{
    return s_Colors[color] | (isOpaque ? 15 : 0);
}

// 0x001B4E48
void SvMyDesign::SetPixel(u32 colorIndex, u32 x, u32 y, u32 part)
{
    u32* words = reinterpret_cast<u32*>(patternData1);
    u32& word = words[(part & 3) * WORDS_PER_PART + (y & 31) * WORDS_PER_ROW + ((x & 31) >> 3)];
    const u32 shift = (x & 7) * 4;
    word = (word & ~(15 << shift)) | ((colorIndex & 15) << shift);
}

// 0x001B4EC8
void SvMyDesign::CopyFrom(const SvMyDesign* src)
{
    std::memcpy(static_cast<void*>(this), src, sizeof(SvMyDesign));
    isGuestDesign = false;
    if (netgame::IsMultiplePlayer() && netgame::GetMyPlayerNo() != 0 && ((FindDesignId(this) >> 8) & 15) == 0) {
        isGuestDesign = true;
    }
    UpdateMainColors();
    title[20] = 0;
}

// 0x001B4F3C
SvMyDesign::SvMyDesign()
{
    mainColors = 1;
    tenConstant = 0;
    patternType = 4;
    isGuestDesign = false;
}

// 0x001B4F6C
void SvMyDesign::Assign(const SvMyDesign* src)
{
    std::memcpy(static_cast<void*>(this), src, sizeof(SvMyDesign));
    isGuestDesign = false;
    if (netgame::IsMultiplePlayer() && netgame::GetMyPlayerNo() != 0 && ((FindDesignId(this) >> 8) & 15) == 0) {
        isGuestDesign = true;
    }
    UpdateDesignTextures(this);
    const u16 id = FindDesignId(this);
    OnDesignChanged(id, false);
    if (netgame::IsMultiplePlayer()) {
        const u16 srcId = FindDesignId(src);
        if (id != 0xFFFF && srcId != 0xFFFF) {
            const u16 packet[2] = {id, srcId};
            netgame::EnqueuePacket(PACKET_DESIGN, netgame::PLAYER_NO_TARGET_ALL, packet, sizeof(packet));
        }
    }
    UpdateMainColors();
    title[20] = 0;
}

namespace {
// the creator of the templates (in the town s_TemplateTown of the region)
// 0x0095D02C
const char16* s_TemplateName = u"Name";
// 0x00AD6458
SvTownId s_TemplateTown;

// the message of the title of a new design in "STR_Mydesign"
const u32 MESSAGE_NEW_TITLE = 30;
// the template kinds: 0..9 the designs of a player by index, 10.. the displays of the Able Sisters by index, the town flag,
// the museum exhibits and the design stands (22, the town flag of ResetToTemplate, loads the template of the stands)
const u32 TEMPLATE_DISPLAY_BASE = 10;
const u32 TEMPLATE_TOWN_FLAG = 18;
const u32 TEMPLATE_EXHIBIT = 19;
const u32 TEMPLATE_STAND = 20;
const u32 TEMPLATE_TOWN_FLAG_SAVE = 22;

// the flag of a design of a player changed on this machine as a guest
inline void UpdateGuestFlag(SvMyDesign* design)
{
    design->isGuestDesign = false;
    if (netgame::IsMultiplePlayer() && netgame::GetMyPlayerNo() != 0 && ((FindDesignId(design) >> 8) & 15) == 0) {
        design->isGuestDesign = true;
    }
}

// the color index of a pixel of an image (15 becomes 12)
inline u32 GetImageColorIndex(const TextureImage16* image, u32 x, u32 y)
{
    const u32 index = image->GetPixel(x, y) & 15;
    return index >= COLOR_INDEX_TRANSPARENT ? COLOR_INDEX_FILL : index;
}
} // namespace

// 0x001B38BC
void SvMyDesign::ReadFromTexture(const void* resTexture, u32 part)
{
    if (resTexture == NULL) {
        return;
    }
    TextureImage16 image(resTexture);
    ReadFromImage(&image, part);
}

// 0x001B38FC
void SvMyDesign::ReadFromImage(const TextureImage16* image, u32 part)
{
    if (patternType == PATTERN_TYPE_SPECIAL) {
        for (u32 p = 0; p < 4; p++) {
            for (u32 y = 0; y < 32; y++) {
                for (u32 x = 0; x < 32; x++) {
                    if (s_SpecialPartOrigins[p][0] + x >= 52) {
                        continue;
                    }
                    const u32 index =
                        GetImageColorIndex(image, s_SpecialPartOrigins[p][0] + x, s_SpecialPartOrigins[p][1] + y);
                    SetPixel(index, x & 31, y & 31, p & 3);
                    DrawDesignPixel(this, s_Colors[palette[index]], x & 31, (y & 31) + (p & 3) * 32);
                }
            }
        }
        return;
    }
    PartRegion region;
    GetPartRegion(&region, part);
    for (u32 x = 0; x < region.width; x++) {
        for (u32 y = 0; y < region.height; y++) {
            u32 index;
            if (region.isRotated) {
                index = GetImageColorIndex(image, region.height - y - 1, x);
            } else {
                index = GetImageColorIndex(image, x, y);
            }
            const u32 row = (region.row + y) & 31;
            SetPixel(index, x & 31, row, region.part & 3);
            DrawDesignPixel(this, s_Colors[palette[index]], x & 31, row + (region.part & 3) * 32);
        }
    }
    if (region.hasExtra) {
        for (u32 x = 0; x < 32; x++) {
            for (u32 y = 0; y < 16; y++) {
                const u32 index = GetImageColorIndex(image, x, y + 32);
                const u32 row = (region.extraRow + y) & 31;
                SetPixel(index, x & 31, row, 3);
                DrawDesignPixel(this, s_Colors[palette[index]], x & 31, row + 96);
            }
        }
    }
}

// 0x001B3B6C
void SvMyDesign::DrawPixel(const void* resTexture, u16 color, u32 x, u32 y)
{
    TextureImage16 image(resTexture);
    if (image.m_Height == 32) {
        if (y < 32) {
            image.SetColor(x, y, color);
        }
    } else if (patternType < PATTERN_TYPE_NORMAL) {
        image.SetColor(x, y, color);
    } else if (y < 32) {
        image.SetColor(x, y, color);
        image.SetColor(x, y + 32, color);
        image.SetColor(x, y + 64, color);
        image.SetColor(x, y + 96, color);
    }
}

// 0x001B3C40
void SvMyDesign::InitNew()
{
    OnDesignChanged(FindDesignId(this), false);
    patternType = PATTERN_TYPE_NORMAL;
    ResetPattern();
    {
        script::WordFix<21> word;
        script::StrMgr::s_pInstance->GetString(&word, "STR_Mydesign", MESSAGE_NEW_TITLE);
        script::WordPtrSv buffer(title, 21);
        static_cast<script::IWord&>(buffer).Copy(word);
    }
    if (netgame::IsMultiplePlayer()) {
        const u16 id = FindDesignId(this);
        if (id != 0xFFFF) {
            const u16 packet[2] = {0xFFFF, static_cast<u16>(id | 0x4000)};
            netgame::EnqueuePacket(PACKET_DESIGN, netgame::PLAYER_NO_TARGET_ALL, packet, sizeof(packet));
        }
    }
    netgame::AssignPersonalId(&creatorData, &SvGetCurrentPlayer()->playerInfo);
    tenConstant = 0;
    UpdateMainColors();
    UpdateDesignTextures(this);
    UpdateGuestFlag(this);
}

// 0x001B3E48
void SvMyDesign::LoadTemplate(u32 kind)
{
    sead::FixedSafeString<30> path;
    {
        sead::StringFormatter formatter(&path);
        formatter.append("MyDesign/tex/myD_%02d.bin");
        if (kind == TEMPLATE_TOWN_FLAG_SAVE) {
            formatter.appendValue(static_cast<s32>(TEMPLATE_STAND));
        } else {
            formatter.appendValue(static_cast<s32>(kind));
        }
        formatter.flush();
    }
    LoadFile(path.getCString());
}

// 0x001B3F90
void SvMyDesign::SetTitle(const script::IWord& word)
{
    script::WordPtrSv buffer(title, 21);
    static_cast<script::IWord&>(buffer).Copy(word);
}

// 0x001B406C
void SvMyDesign::FillUnusedParts()
{
    const u8 type = patternType;
    if (type == PATTERN_TYPE_SPECIAL) {
        for (u32 part = 0; part < 4; part++) {
            for (u32 y = 0; y < 32; y++) {
                for (u32 x = 0; x < 32; x++) {
                    if (s_SpecialPartOffsets[part][0] + x >= 52 || (s_SpecialMask[part * 32 + y] & (1 << x)) != 0) {
                        SetPixel(COLOR_INDEX_FILL, x, y, part);
                    }
                }
            }
        }
        return;
    }
    if (static_cast<u32>(type - 6) <= 1 || type >= PATTERN_TYPE_NORMAL) {
        for (u32 part = 1; part < 4; part++) {
            for (u32 y = 0; y < 32; y++) {
                for (u32 x = 0; x < 32; x++) {
                    SetPixel(COLOR_INDEX_FILL, x, y, part);
                }
            }
        }
        return;
    }
    if (type == 4 || type == 3 || type == 5) {
        for (u32 y = 0; y < 32; y++) {
            for (u32 x = 0; x < 32; x++) {
                SetPixel(COLOR_INDEX_FILL, x, y, 3);
            }
        }
    }
    u32 first;
    if (patternType == 2 || patternType == 5) {
        first = 0;
    } else if (patternType == 1 || patternType == 4) {
        first = 16;
    } else {
        return;
    }
    for (u32 y = 0; y < 32; y++) {
        for (u32 x = first; x < 32; x++) {
            SetPixel(COLOR_INDEX_FILL, x, y, 2);
        }
    }
}

// 0x001B42E8
void SvMyDesign::SetCreatorToCurrentPlayer()
{
    if (g_pSvCurrentPlayer != NULL) {
        netgame::AssignPersonalId(&creatorData, &g_pSvCurrentPlayer->playerInfo);
    }
}

// 0x001B4310
void SvMyDesign::InitNewForNetwork()
{
    if (!netgame::IsMultiplePlayer()) {
        return;
    }
    const u16 id = FindDesignId(this);
    OnDesignChanged(id, true);
    patternType = PATTERN_TYPE_NORMAL;
    ResetPattern();
    {
        script::WordFix<21> word;
        script::StrMgr::s_pInstance->GetString(&word, "STR_Mydesign", MESSAGE_NEW_TITLE);
        script::WordPtrSv buffer(title, 21);
        static_cast<script::IWord&>(buffer).Copy(word);
    }
    netgame::AssignPersonalId(&creatorData, &netgame::Func3038EC((id >> 4) & 15)->playerInfo);
    tenConstant = 0;
    UpdateMainColors();
    UpdateDesignTextures(this);
    UpdateGuestFlag(this);
}

// 0x001B44E0
void SvMyDesign::ResetToTemplate(u32 /* kind */)
{
    const u16 id = FindDesignId(this);
    const u16 index = id & 15;
    const u32 sub = (id >> 4) & 15;
    const u32 kind = (id >> 8) & 15;
    MakeTownId(&s_TemplateTown, s_TemplateName, GetRegion());
    netgame::MakePersonalId(&creatorData, s_TemplateName, netgame::UNKNOWN_PLAYER_ID, 0, NULL);
    AssignTownId(&creatorData.townData, &s_TemplateTown);
    switch (kind) {
    case 0: {
        sead::FixedSafeString<30> path;
        {
            sead::StringFormatter formatter(&path);
            formatter.append("MyDesign/tex/myD_%02d.bin").appendValue(index).flush();
        }
        LoadFile(path.getCString());
        netgame::AssignPersonalId(&creatorData, &SvGetPlayer(static_cast<PlayerSaveNumber>(sub))->playerInfo);
        break;
    }
    case 2: {
        sead::FixedSafeString<30> path;
        {
            sead::StringFormatter formatter(&path);
            formatter.append("MyDesign/tex/myD_%02d.bin")
                .appendValue(static_cast<s32>(index + TEMPLATE_DISPLAY_BASE))
                .flush();
        }
        LoadFile(path.getCString());
        break;
    }
    case 4: {
        sead::FixedSafeString<30> path;
        {
            sead::StringFormatter formatter(&path);
            formatter.append("MyDesign/tex/myD_%02d.bin").appendValue(static_cast<s32>(TEMPLATE_EXHIBIT)).flush();
        }
        LoadFile(path.getCString());
        break;
    }
    case 5: {
        sead::FixedSafeString<30> path;
        {
            sead::StringFormatter formatter(&path);
            formatter.append("MyDesign/tex/myD_%02d.bin").appendValue(static_cast<s32>(TEMPLATE_TOWN_FLAG)).flush();
        }
        LoadFile(path.getCString());
        break;
    }
    case 6: {
        sead::FixedSafeString<30> path;
        {
            sead::StringFormatter formatter(&path);
            formatter.append("MyDesign/tex/myD_%02d.bin").appendValue(static_cast<s32>(TEMPLATE_STAND)).flush();
        }
        LoadFile(path.getCString());
        break;
    }
    }
    UpdateMainColors();
}

// 0x001B4920
void SvMyDesign::Swap(SvMyDesign* other)
{
    const u16 id = FindDesignId(this);
    const u16 otherId = FindDesignId(other);
    OnDesignChanged(id, false);
    OnDesignChanged(otherId, false);
    if (netgame::IsMultiplePlayer() && id != 0xFFFF && otherId != 0xFFFF) {
        const u16 packet[2] = {id, static_cast<u16>(otherId | 0x8000)};
        netgame::EnqueuePacket(PACKET_DESIGN, netgame::PLAYER_NO_TARGET_ALL, packet, sizeof(packet));
    }
    SvMyDesign tmp;
    tmp.CopyFrom(this);
    std::memcpy(static_cast<void*>(this), other, sizeof(SvMyDesign));
    std::memcpy(static_cast<void*>(other), &tmp, sizeof(SvMyDesign));
    UpdateGuestFlag(this);
    UpdateGuestFlag(other);
    title[20] = 0;
    UpdateDesignTextures(this);
    UpdateDesignTextures(other);
}

// 0x001B4AB8
void SvMyDesign::LoadFile(const char* path)
{
    ssys::ma::FileData file;
    file.Load(path, sead::HeapMgr::s_pInstance->getCurrentHeap(), 0);
    const u8* data = file.m_pData;
    for (u32 part = 0; part < 4; part++) {
        for (u32 y = 0; y < 32; y++) {
            for (u32 x = 0; x < 16; x++) {
                SetPixel(*data >> 4, x * 2, y, part);
                SetPixel(*data & 15, x * 2 + 1, y, part);
                data++;
            }
        }
    }
    for (u32 i = 0; i < 15; i++) {
        palette[i] = data[i];
    }
    {
        script::WordFix<21> word;
        script::StrMgr::s_pInstance->GetString(&word, "STR_Mydesign", data[15]);
        script::WordPtrSv buffer(title, 21);
        static_cast<script::IWord&>(buffer).Copy(word);
    }
    {
        script::WordFix<9> word;
        script::StrMgr::s_pInstance->GetString(&word, "STR_Mydesign", data[16]);
        script::WordPtrSv buffer(creatorData.townData.dataTownName, 9);
        static_cast<script::IWord&>(buffer).Copy(word);
    }
    {
        script::WordFix<9> word;
        script::StrMgr::s_pInstance->GetString(&word, "STR_Mydesign", data[17]);
        script::WordPtrSv buffer(creatorData.playerData.playerName, 9);
        static_cast<script::IWord&>(buffer).Copy(word);
    }
    tenConstant = data[18];
    patternType = data[19];
    file.Unload();
    UpdateMainColors();
}

// 0x001B4E98
void SvMyDesign::UpdateCreatorTown(const SvPersonalId* player, const SvTownId* town)
{
    if (IsSamePersonalId(&creatorData, player)) {
        AssignTownId(&creatorData.townData, town);
    }
}

// 0x0070C44C
void SvMyDesign::DrawToTexture(const void* resTexture, bool isSmall)
{
    if (resTexture == NULL) {
        return;
    }
    const u32 isParts = patternType < PATTERN_TYPE_NORMAL;
    const u32 rows = isSmall ? 1 : 4;
    TextureImage16 image(resTexture);
    for (u32 row = 0; row < rows; row++) {
        const u32 part = row * isParts;
        for (u32 y = 0; y < 32; y++) {
            for (u32 x = 0; x < 32; x++) {
                u32 index = GetColorIndex(this, x, y, part);
                if (index == COLOR_INDEX_TRANSPARENT) {
                    index = 0;
                }
                image.SetColor(x, y + row * 32, s_Colors[palette[index]]);
            }
        }
    }
}

// 0x0070C55C
void SvMyDesign::WriteToTexture(const void* resTexture, u32 part)
{
    if (resTexture == NULL) {
        return;
    }
    TextureImage16 image(resTexture);
    if (patternType == PATTERN_TYPE_SPECIAL) {
        for (u32 p = 0; p < 4; p++) {
            for (u32 y = 0; y < 32; y++) {
                for (u32 x = 0; x < 32; x++) {
                    if (static_cast<s32>(s_SpecialPartOffsets[p][0] + x) >= 52 ||
                        (s_SpecialMask[p * 32 + y] & (1 << x)) != 0) {
                        continue;
                    }
                    u32 index = GetColorIndex(this, x, y, p);
                    if (index == COLOR_INDEX_TRANSPARENT) {
                        index = 0;
                    }
                    image.SetPixel(s_SpecialPartOffsets[p][0] + x, s_SpecialPartOffsets[p][1] + y, GetPixelValue(index));
                }
            }
        }
        return;
    }
    PartRegion region;
    GetPartRegion(&region, part);
    for (u32 x = 0; x < region.width; x++) {
        for (u32 y = 0; y < region.height; y++) {
            u32 index = GetColorIndex(this, x, region.row + y, region.part);
            if (index == COLOR_INDEX_TRANSPARENT) {
                index = 0;
            }
            if (index == COLOR_INDEX_TRANSPARENT) {
                continue;
            }
            const u16 value = (s_Colors[palette[index]] & ~15) | index;
            if (region.isRotated) {
                image.SetPixel(region.height - y - 1, x, value);
            } else {
                image.SetPixel(x, y, value);
            }
        }
    }
    if (region.hasExtra) {
        for (u32 x = 0; x < 32; x++) {
            for (u32 y = 0; y < 16; y++) {
                u32 index = GetColorIndex(this, x, region.extraRow + y, 3);
                if (index == COLOR_INDEX_TRANSPARENT) {
                    index = 0;
                }
                if (index == COLOR_INDEX_TRANSPARENT) {
                    continue;
                }
                image.SetPixel(x, y + 32, (s_Colors[palette[index]] & ~15) | index);
            }
        }
    }
}

// 0x0070C82C
u16 SvMyDesign::GetPixelValue(u32 index) const
{
    if (index == COLOR_INDEX_TRANSPARENT) {
        return COLOR_INDEX_TRANSPARENT;
    }
    return (s_Colors[palette[index]] & ~15) | index;
}

// 0x0070C85C
u8 SvMyDesign::GetHatType() const
{
    return static_cast<u32>(patternType - 6) < 2 ? patternType : 11;
}

// 0x0070C870
u8 SvMyDesign::GetClothesType() const
{
    return patternType < 6 ? patternType : 10;
}

// 0x0070C880
bool SvMyDesign::IsWearable() const
{
    return patternType < PATTERN_TYPE_NORMAL;
}

// 0x0070C898
bool SvMyDesign::IsCreatedByCurrentPlayer() const
{
    if (g_pSvCurrentPlayer == NULL) {
        return false;
    }
    return IsSamePersonalId(&g_pSvCurrentPlayer->playerInfo, &creatorData);
}

// 0x0070C8C4
void SvMyDesign::DrawToTextureByPairs(const void* resTexture, bool isSmall)
{
    if (resTexture == NULL) {
        return;
    }
    const u32 isParts = patternType < PATTERN_TYPE_NORMAL;
    const u32 rows = isSmall ? 1 : 4;
    TextureImage16 image(resTexture);
    for (u32 row = 0; row < rows; row++) {
        const u32 part = row * isParts;
        for (u32 y = 0; y < 32; y++) {
            for (u32 i = 0; i < 16; i++) {
                const u8 x = i * 2;
                u32 index = GetColorIndex(this, x, y, part);
                if (index == COLOR_INDEX_TRANSPARENT) {
                    index = 0;
                }
                image.SetColor(x, y + row * 32, s_Colors[palette[index]]);
                index = GetColorIndex(this, x + 1, y, part);
                if (index == COLOR_INDEX_TRANSPARENT) {
                    index = 0;
                }
                image.SetColor(x + 1, y + row * 32, s_Colors[palette[index]]);
            }
        }
    }
}
