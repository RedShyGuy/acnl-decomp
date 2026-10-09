#include "nn/gr/CTR/gr_Texture.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;

namespace {
// the registers of units 1 and 2 (border color, size, parameters, lod, address, format) from
// baseRegister on
inline u32* MakeUnitCommand(const Texture::UnitBase& unit, u32* command, u32 baseRegister)
{
    *command++ = detail::PackColor(unit.m_BorderColorR, unit.m_BorderColorG, unit.m_BorderColorB, unit.m_BorderColorA);
    *command++ = CommandHeader(baseRegister);
    *command++ = unit.m_Height | (unit.m_Width << 16);
    *command++ = CommandHeader(baseRegister + 1);

    const bool isMinLinear = (unit.m_MinFilter == 3 || unit.m_MinFilter == 4 || unit.m_MinFilter == 5);
    const bool isMipmapLinear = (unit.m_MinFilter == 2 || unit.m_MinFilter == 5);
    *command++ = (unit.m_MagFilter << 1) | (isMinLinear << 2) | ((unit.m_Format == 12 ? 2 : 0) << 4)
        | (unit.m_WrapT << 8) | (unit.m_WrapS << 12) | (isMipmapLinear << 24);
    *command++ = CommandHeader(baseRegister + 2);

    u32 lod = 0;
    if (unit.m_MinFilter != 3 && unit.m_MinFilter != 0) {
        lod = detail::Float32ToFix13Fraction8(unit.m_LodBias) | (unit.m_MaxLodLevel << 16) | (unit.m_MinLodLevel << 24);
    }
    *command++ = lod;
    *command++ = CommandHeader(baseRegister + 3);
    *command++ = unit.m_PhysicalAddr >> 3;
    *command++ = CommandHeader(baseRegister + 4);
    *command++ = unit.m_Format;
    *command++ = CommandHeader(baseRegister + 5);
    return command;
}

inline u32* MakeResetTextureConfigCommand(u32* command)
{
    *command++ = 0;
    *command++ = CommandHeader(0x80, 0x0, 2);
    *command++ = 0;
    *command++ = 0;
    return command;
}
} // namespace

// 0x0034B1BC | fefates:bytes [tier B]
u32* nn::gr::CTR::Texture::MakeDisableCommand(u32* command, bool isResetTextureConfig)
{
    if (isResetTextureConfig) {
        command = MakeResetTextureConfigCommand(command);
    }
    // all units off, clear the texture cache
    *command++ = 0x13000;
    *command++ = CommandHeader(0x80);
    return command;
}

// 0x0034B248 | fefates:bytes [tier B]
nn::gr::CTR::Texture::Texture() : m_Unit0(this), m_Unit1(this), m_Unit2(this), m_Unit3(this)
{
}

// 0x007281E0 | nintendogs:bytes-fuzzy [confirmed by fefates, mk7dlp] [tier A]
u32* nn::gr::CTR::Texture::MakeCommand(u32* command, bool isResetTextureConfig) const
{
    command = MakeFuncCommand(command, isResetTextureConfig);
    command = m_Unit0.MakeCommand(command, false);
    command = m_Unit1.MakeCommand(command, false);
    return m_Unit2.MakeCommand(command, false);
}

// 0x00728220 | fefates:bytes [tier B]
u32* nn::gr::CTR::Texture::MakeFuncCommand(u32* command, bool isResetTextureConfig) const
{
    if (isResetTextureConfig) {
        command = MakeResetTextureConfigCommand(command);
    }
    // the second write also clears the texture cache
    *command++ = (m_Unit0.m_TextureType != 5) | ((m_Unit1.m_IsEnabled != 0) << 1) | ((m_Unit2.m_IsEnabled != 0) << 2)
        | (m_Unit3.m_Texcoord << 8) | ((m_Unit3.m_IsEnabled != 0) << 10) | ((m_Unit2.m_IsTexcoord1 != 0) << 13) | 0x1000;
    *command++ = CommandHeader(0x80);
    *command++ = (m_Unit0.m_TextureType != 5) | ((m_Unit1.m_IsEnabled != 0) << 1) | ((m_Unit2.m_IsEnabled != 0) << 2)
        | (m_Unit3.m_Texcoord << 8) | ((m_Unit3.m_IsEnabled != 0) << 10) | ((m_Unit2.m_IsTexcoord1 != 0) << 13) | 0x11000;
    *command++ = CommandHeader(0x80);
    return command;
}

// 0x007285AC (name is ours)
u32* nn::gr::CTR::Texture::Unit1::MakeCommand(u32* command, bool isAddFuncCommand) const
{
    if (isAddFuncCommand) {
        command = m_Texture->MakeFuncCommand(command, true);
    }
    return MakeUnitCommand(*this, command, 0x91);
}

// 0x00728774 (name is ours)
u32* nn::gr::CTR::Texture::Unit2::MakeCommand(u32* command, bool isAddFuncCommand) const
{
    if (isAddFuncCommand) {
        command = m_Texture->MakeFuncCommand(command, true);
    }
    return MakeUnitCommand(*this, command, 0x99);
}

} // namespace CTR
} // namespace gr
} // namespace nn
