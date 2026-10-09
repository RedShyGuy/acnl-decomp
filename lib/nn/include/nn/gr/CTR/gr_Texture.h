#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// The four texture units. Unit0 and UnitBase are named in the symbols; Unit1 to Unit3 are named
// after them (names are ours), as are all members. Values follow the PICA registers on 3dbrew:
// format 12 = ETC1; filters 0 nearest, 1/2 nearest with nearest/linear mipmap, 3 linear,
// 4/5 linear with nearest/linear mipmap; texture type 0 2D, 1 cube, 2 shadow 2D, 3 projection,
// 4 shadow cube, 5 disabled.
class Texture
{
public:
    class UnitBase
    {
    public:
        UnitBase(); // 0x0034B1F4 | fefates:bytes [tier B]

        u32 m_PhysicalAddr; // 0x00
        u16 m_Width;        // 0x04
        u16 m_Height;       // 0x06
        u8 m_Format;        // 0x08
        u8 m_WrapT;         // 0x09
        u8 m_WrapS;         // 0x0A
        u8 m_MagFilter;     // 0x0B
        u8 m_MinFilter;     // 0x0C
        f32 m_LodBias;      // 0x10
        u8 m_MinLodLevel;   // 0x14
        u8 m_MaxLodLevel;   // 0x15
        u8 m_BorderColorR;  // 0x16
        u8 m_BorderColorG;  // 0x17
        u8 m_BorderColorB;  // 0x18
        u8 m_BorderColorA;  // 0x19
        u8 m_Padding[2];    // 0x1A, (explicit, the derived units start at 0x1C)
    };

    class Unit0 : public UnitBase
    {
    public:
        explicit Unit0(Texture* texture)
            : m_TextureType(5), m_CubeMapAddr0(0), m_CubeMapAddr1(0), m_CubeMapAddr2(0), m_CubeMapAddr3(0),
              m_CubeMapAddr4(0), m_CubeMapAddr5(0), m_Texture(texture)
        {
        }

        u32* MakeCommand(u32* command, bool isAddFuncCommand) const; // 0x00728314 | fefates:bytes [tier B]

        u8 m_TextureType;   // 0x1C
        u32 m_CubeMapAddr0; // 0x20
        u32 m_CubeMapAddr1; // 0x24
        u32 m_CubeMapAddr2; // 0x28
        u32 m_CubeMapAddr3; // 0x2C
        u32 m_CubeMapAddr4; // 0x30
        u32 m_CubeMapAddr5; // 0x34
        Texture* m_Texture; // 0x38
    };

    class Unit1 : public UnitBase
    {
    public:
        explicit Unit1(Texture* texture) : m_IsEnabled(0), m_Texture(texture) {}

        DECOMP_NOINLINE u32* MakeCommand(u32* command, bool isAddFuncCommand) const; // 0x007285AC (name is ours)

        u8 m_IsEnabled;     // 0x1C
        Texture* m_Texture; // 0x20
    };

    class Unit2 : public UnitBase
    {
    public:
        explicit Unit2(Texture* texture) : m_IsEnabled(0), m_IsTexcoord1(0), m_Texture(texture) {}

        DECOMP_NOINLINE u32* MakeCommand(u32* command, bool isAddFuncCommand) const; // 0x00728774 (name is ours)

        u8 m_IsEnabled;     // 0x1C
        u8 m_IsTexcoord1;   // 0x1D, reads texture coordinate 1 instead of 2
        Texture* m_Texture; // 0x20
    };

    // the procedural texture unit
    class Unit3 : public UnitBase
    {
    public:
        explicit Unit3(Texture* texture) : m_IsEnabled(0), m_Texcoord(0), m_Texture(texture) {}

        u8 m_IsEnabled;     // 0x1C
        u8 m_Texcoord;      // 0x1D
        Texture* m_Texture; // 0x20
    };

    Texture(); // 0x0034B248 | fefates:bytes [tier B]

    static u32* MakeDisableCommand(u32* command, bool isResetTextureConfig); // 0x0034B1BC | fefates:bytes [tier B]
    u32* MakeCommand(u32* command, bool isResetTextureConfig) const; // 0x007281E0 | nintendogs:bytes-fuzzy [confirmed by fefates, mk7dlp] [tier A]
    DECOMP_NOINLINE u32* MakeFuncCommand(u32* command, bool isResetTextureConfig) const; // 0x00728220 | fefates:bytes [tier B]

    Unit0 m_Unit0; // 0x00
    Unit1 m_Unit1; // 0x3C
    Unit2 m_Unit2; // 0x60
    Unit3 m_Unit3; // 0x84
};
ASSERT_SIZE(Texture::UnitBase, 0x1C);
ASSERT_SIZE(Texture::Unit0, 0x3C);
ASSERT_SIZE(Texture::Unit1, 0x24);
ASSERT_SIZE(Texture, 0xA8);
} // namespace CTR
} // namespace gr
} // namespace nn
