#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/ut/ut_Api.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
// The blocks of the layout files: BCLYT ('CLYT', a block per pane, group and resource list),
// BCLAN ('CLAN', the animations) and the image blocks of BCLIM ('CLIM'). The block kinds are
// from the binary (Layout::Build, AnimResource::Set, TexResource::Set); struct and member names
// are ours. Offsets in the blocks are relative to the place named at the member.
namespace res {
// the block kinds (four characters, the first in the lowest byte)
#define NW_LYT_BLOCK_KIND(a, b, c, d) (static_cast<u32>(a) | static_cast<u32>(b) << 8 | static_cast<u32>(c) << 16 | static_cast<u32>(d) << 24)
const u32 FILE_SIGNATURE_LAYOUT = NW_LYT_BLOCK_KIND('C', 'L', 'Y', 'T');
const u32 FILE_SIGNATURE_ANIMATION = NW_LYT_BLOCK_KIND('C', 'L', 'A', 'N');
const u32 FILE_SIGNATURE_IMAGE = NW_LYT_BLOCK_KIND('C', 'L', 'I', 'M');
const u32 FILE_VERSION = 0x02020000;
const u32 BLOCK_KIND_LAYOUT = NW_LYT_BLOCK_KIND('l', 'y', 't', '1');
const u32 BLOCK_KIND_TEXTURE_LIST = NW_LYT_BLOCK_KIND('t', 'x', 'l', '1');
const u32 BLOCK_KIND_FONT_LIST = NW_LYT_BLOCK_KIND('f', 'n', 'l', '1');
const u32 BLOCK_KIND_MATERIAL_LIST = NW_LYT_BLOCK_KIND('m', 'a', 't', '1');
const u32 BLOCK_KIND_PANE = NW_LYT_BLOCK_KIND('p', 'a', 'n', '1');
const u32 BLOCK_KIND_PICTURE = NW_LYT_BLOCK_KIND('p', 'i', 'c', '1');
const u32 BLOCK_KIND_TEXTBOX = NW_LYT_BLOCK_KIND('t', 'x', 't', '1');
const u32 BLOCK_KIND_WINDOW = NW_LYT_BLOCK_KIND('w', 'n', 'd', '1');
const u32 BLOCK_KIND_BOUNDING = NW_LYT_BLOCK_KIND('b', 'n', 'd', '1');
const u32 BLOCK_KIND_PANE_CHILD_START = NW_LYT_BLOCK_KIND('p', 'a', 's', '1');
const u32 BLOCK_KIND_PANE_CHILD_END = NW_LYT_BLOCK_KIND('p', 'a', 'e', '1');
const u32 BLOCK_KIND_GROUP = NW_LYT_BLOCK_KIND('g', 'r', 'p', '1');
const u32 BLOCK_KIND_GROUP_CHILD_START = NW_LYT_BLOCK_KIND('g', 'r', 's', '1');
const u32 BLOCK_KIND_GROUP_CHILD_END = NW_LYT_BLOCK_KIND('g', 'r', 'e', '1');
const u32 BLOCK_KIND_USER_DATA_LIST = NW_LYT_BLOCK_KIND('u', 's', 'd', '1');
const u32 BLOCK_KIND_ANIM_TAG = NW_LYT_BLOCK_KIND('p', 'a', 't', '1');
const u32 BLOCK_KIND_ANIM_SHARE = NW_LYT_BLOCK_KIND('p', 'a', 'h', '1');
const u32 BLOCK_KIND_ANIM_INFO = NW_LYT_BLOCK_KIND('p', 'a', 'i', '1');
const u32 BLOCK_KIND_IMAGE = NW_LYT_BLOCK_KIND('i', 'm', 'a', 'g');
// the kinds of the animation infos
const u32 ANIMKIND_PANE_SRT = NW_LYT_BLOCK_KIND('C', 'L', 'P', 'A');
const u32 ANIMKIND_VISIBILITY = NW_LYT_BLOCK_KIND('C', 'L', 'V', 'I');
const u32 ANIMKIND_VERTEX_COLOR = NW_LYT_BLOCK_KIND('C', 'L', 'V', 'C');
const u32 ANIMKIND_MATERIAL_COLOR = NW_LYT_BLOCK_KIND('C', 'L', 'M', 'C');
const u32 ANIMKIND_TEXTURE_SRT = NW_LYT_BLOCK_KIND('C', 'L', 'T', 'S');
const u32 ANIMKIND_TEXTURE_PATTERN = NW_LYT_BLOCK_KIND('C', 'L', 'T', 'P');
#undef NW_LYT_BLOCK_KIND

// the length of the names
const int RESOURCE_NAME_LENGTH = 16;    // panes, groups
const int MATERIAL_NAME_LENGTH = 20;
const int USER_DATA_STRING_LENGTH = 8;

// 'lyt1'
struct Layout
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u8 originType;                      // 0x08
    u8 padding[3];                      // 0x09
    Size layoutSize;                    // 0x0C
};
ASSERT_SIZE(Layout, 0x14);

// 'txl1' and 'fnl1': the names of the files
struct TextureList
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u16 texNum;                         // 0x08
    u8 padding[2];                      // 0x0A
    u32 nameOffsets[1];                 // 0x0C (relative to nameOffsets)
};
struct FontList
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u16 fontNum;                        // 0x08
    u8 padding[2];                      // 0x0A
    u32 nameOffsets[1];                 // 0x0C (relative to nameOffsets)
};

// 'mat1'
struct MaterialList
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u16 materialNum;                    // 0x08
    u8 padding[2];                      // 0x0A
    u32 materialOffsets[1];             // 0x0C (relative to the block)
};

// the texture of a material
struct TexMap
{
    u16 texIdx;     // 0x0 in the texture list
    u8 wrapSFlag;   // 0x2 bits 0-1 the wrap mode of S, 2-3 the minifying filter
    u8 wrapTFlag;   // 0x3 bits 0-1 the wrap mode of T, 2-3 the magnifying filter
};
ASSERT_SIZE(TexMap, 0x4);

// A material: its name, colors and flags, then the parts (TexMap, TexSRT, TexCoordGen,
// TevStage, AlphaCompare, BlendMode) as the counts in flags say.
struct Material
{
    char name[MATERIAL_NAME_LENGTH];    // 0x00
    ut::Color8 colors[7];               // 0x14
    u32 flags;                          // 0x30 the counts as in Material::m_MemNum; bit 11: no
                                        //      texture environment of its own
    // the parts follow
};
ASSERT_SIZE(Material, 0x34);

// The base of the pane blocks.
struct Pane
{
    ut::BinaryBlockHeader blockHeader;          // 0x00
    u8 flag;                                    // 0x08 Pane::m_Flag
    u8 basePosition;                            // 0x09
    u8 alpha;                                   // 0x0A
    u8 padding;                                 // 0x0B
    char name[RESOURCE_NAME_LENGTH];            // 0x0C
    char userData[USER_DATA_STRING_LENGTH];     // 0x1C
    nn::math::VEC3 translate;                   // 0x24
    nn::math::VEC3 rotate;                      // 0x30
    nn::math::VEC2 scale;                       // 0x3C
    Size size;                                  // 0x44
};
ASSERT_SIZE(Pane, 0x4C);

// 'bnd1'
struct Bounding : public Pane
{
};

// 'pic1'
struct Picture : public Pane
{
    ut::Color8 vtxCols[4];      // 0x4C
    u16 materialIdx;            // 0x5C
    u8 texCoordNum;             // 0x5E
    u8 padding2;                // 0x5F
    // TexCoordQuad texCoords[texCoordNum]; 0x60
};
ASSERT_SIZE(Picture, 0x60);

// 'txt1'
struct TextBox : public Pane
{
    u16 textBufBytes;           // 0x4C
    u16 textStrBytes;           // 0x4E
    u16 materialIdx;            // 0x50
    u16 fontIdx;                // 0x52
    u8 textPosition;            // 0x54
    u8 textAlignment;           // 0x55
    u8 padding2[2];             // 0x56
    u32 textStrOffset;          // 0x58 (relative to the block)
    ut::Color8 textCols[2];     // 0x5C
    Size fontSize;              // 0x64
    f32 charSpace;              // 0x6C
    f32 lineSpace;              // 0x70
};
ASSERT_SIZE(TextBox, 0x74);

// the inflation of the content of a window
struct InflationLRTB
{
    f32 l;  // 0x0
    f32 r;  // 0x4
    f32 t;  // 0x8
    f32 b;  // 0xC
};
ASSERT_SIZE(InflationLRTB, 0x10);

// the content of a window
struct WindowContent
{
    ut::Color8 vtxCols[4];      // 0x00
    u16 materialIdx;            // 0x10
    u8 texCoordNum;             // 0x12
    u8 padding;                 // 0x13
    // TexCoordQuad texCoords[texCoordNum]; 0x14
};
ASSERT_SIZE(WindowContent, 0x14);

// a frame of a window
struct WindowFrame
{
    u16 materialIdx;    // 0x0
    u8 textureFlip;     // 0x2
    u8 padding;         // 0x3
};
ASSERT_SIZE(WindowFrame, 0x4);

// 'wnd1'
struct Window : public Pane
{
    InflationLRTB inflation;    // 0x4C
    u8 frameNum;                // 0x5C
    u8 padding2[3];             // 0x5D
    u32 contentOffset;          // 0x60 (relative to the block)
    u32 frameOffsetTableOffset; // 0x64 (relative to the block; the frames relative to the block)
};
ASSERT_SIZE(Window, 0x68);

// 'grp1'
struct Group
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    char name[RESOURCE_NAME_LENGTH];    // 0x08
    u16 paneNum;                        // 0x18
    u8 padding[2];                      // 0x1A
    // char paneNames[paneNum][RESOURCE_NAME_LENGTH]; 0x1C
};
ASSERT_SIZE(Group, 0x1C);

// 'pai1': the animations
struct AnimationBlock
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u16 frameSize;                      // 0x08
    u8 loop;                            // 0x0A
    u8 padding;                         // 0x0B
    u16 fileNum;                        // 0x0C the textures of the texture patterns
    u16 animContNum;                    // 0x0E
    u32 animContOffsetsOffset;          // 0x10 (relative to the block; the contents as well)
    // u32 fileNameOffsets[fileNum]; 0x14 (relative to fileNameOffsets)
};
ASSERT_SIZE(AnimationBlock, 0x14);

// the target of an animation content
enum AnimContentType : u8 {
    ANIMCONTENTTYPE_PANE,
    ANIMCONTENTTYPE_MATERIAL
};

// the animations of a pane or material
struct AnimationContent
{
    char name[MATERIAL_NAME_LENGTH];    // 0x00
    u8 num;                             // 0x14 the infos
    u8 type;                            // 0x15 AnimContentType
    u8 padding[2];                      // 0x16
    // u32 animInfoOffsets[num]; 0x18 (relative to the content)
};
ASSERT_SIZE(AnimationContent, 0x18);

// the animations of a kind (ANIMKIND_*)
struct AnimationInfo
{
    u32 kind;           // 0x0
    u8 num;             // 0x4 the targets
    u8 padding[3];      // 0x5
    // u32 animTargetOffsets[num]; 0x8 (relative to the info)
};
ASSERT_SIZE(AnimationInfo, 0x8);

// an animated value and its curve
struct AnimationTarget
{
    u8 id;              // 0x0 the texture / texture SRT of a material
    u8 target;          // 0x1 the value
    u8 curveType;       // 0x2
    u8 padding;         // 0x3
    u16 keyNum;         // 0x4
    u8 padding2[2];     // 0x6
    u32 keysOffset;     // 0x8 (relative to the target)
};
ASSERT_SIZE(AnimationTarget, 0xC);

// a key of a Hermite curve
struct HermiteKey
{
    f32 frame;  // 0x0
    f32 value;  // 0x4
    f32 slope;  // 0x8
};
ASSERT_SIZE(HermiteKey, 0xC);

// a key of a step curve
struct StepKey
{
    f32 frame;      // 0x0
    u16 value;      // 0x4
    u16 padding;    // 0x6
};
ASSERT_SIZE(StepKey, 0x8);

// 'pat1': a tag of an animation and the groups it binds to
struct AnimationTagBlock
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u16 tagOrder;                       // 0x08
    u16 groupNum;                       // 0x0A
    u32 nameOffset;                     // 0x0C (relative to the block)
    u32 groupsOffset;                   // 0x10 (relative to the block)
    s16 startFrame;                     // 0x14
    s16 endFrame;                       // 0x16
    u8 flag;                            // 0x18 bit 0: the groups bind with their descendants
    u8 padding[3];                      // 0x19
};
ASSERT_SIZE(AnimationTagBlock, 0x1C);

// a group of an animation tag
struct AnimationGroupRef
{
    char name[RESOURCE_NAME_LENGTH + 1];    // 0x00
    u8 flag;                                // 0x11
    u8 padding[2];                          // 0x12
};
ASSERT_SIZE(AnimationGroupRef, 0x14);

// 'pah1': panes whose animations the panes of a group share
struct AnimationShareBlock
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u32 animShareInfoOffset;            // 0x08 (relative to the block)
    u16 shareNum;                       // 0x0C
    u8 padding[2];                      // 0x0E
};
ASSERT_SIZE(AnimationShareBlock, 0x10);

struct AnimationShareInfo
{
    char srcPaneName[RESOURCE_NAME_LENGTH + 1];     // 0x00
    char targetGroupName[RESOURCE_NAME_LENGTH + 1]; // 0x11
    u8 padding[2];                                  // 0x22
};
ASSERT_SIZE(AnimationShareInfo, 0x24);

// 'imag': the image block at the end of a BCLIM file
struct Image
{
    ut::BinaryBlockHeader blockHeader;  // 0x00
    u16 width;                          // 0x08
    u16 height;                         // 0x0A
    u8 format;                          // 0x0C TexFormat
    u8 flag;                            // 0x0D bits 0-1 the rotation of the texture
    u8 padding[2];                      // 0x0E
};
ASSERT_SIZE(Image, 0x10);
} // namespace res
} // namespace lyt
} // namespace nw
