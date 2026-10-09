#include "nn/gr/CTR/gr_Texture_Unit0.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;

// 0x00728314 | fefates:bytes [tier B]
u32* nn::gr::CTR::Texture::Unit0::MakeCommand(u32* command, bool isAddFuncCommand) const
{
    if (isAddFuncCommand) {
        command = m_Texture->MakeFuncCommand(command, true);
    }
    *command++ = detail::PackColor(m_BorderColorR, m_BorderColorG, m_BorderColorB, m_BorderColorA);
    *command++ = CommandHeader(0x81);
    *command++ = m_Height | (m_Width << 16);
    *command++ = CommandHeader(0x82);

    const bool isShadow = (m_TextureType == 2 || m_TextureType == 4);
    const bool isMinLinear = (m_MinFilter == 3 || m_MinFilter == 4 || m_MinFilter == 5);
    const bool isMipmapLinear = (m_MinFilter == 2 || m_MinFilter == 5);
    *command++ = (m_MagFilter << 1) | (isMinLinear << 2) | ((m_Format == 12 ? 2 : 0) << 4) | (m_WrapT << 8)
        | (m_WrapS << 12) | (isShadow << 20) | (isMipmapLinear << 24) | (m_TextureType << 28);
    *command++ = CommandHeader(0x83);

    // without mipmaps the lod register stays 0
    u32 lod = 0;
    if (m_MinFilter != 3 && m_MinFilter != 0) {
        lod = detail::Float32ToFix13Fraction8(m_LodBias) | (m_MaxLodLevel << 16) | (m_MinLodLevel << 24);
    }
    *command++ = lod;
    *command++ = CommandHeader(0x84);

    switch (m_TextureType) {
    case 0:
    case 3:
    case 2:
        *command++ = m_PhysicalAddr >> 3;
        *command++ = CommandHeader(0x85);
        break;
    case 1:
    case 4:
        // the other faces share the upper address bits of the first
        *command++ = m_CubeMapAddr0 >> 3;
        *command++ = CommandHeader(0x85);
        *command++ = (m_CubeMapAddr1 >> 3) & 0x3FFFFF;
        *command++ = CommandHeader(0x86);
        *command++ = (m_CubeMapAddr2 >> 3) & 0x3FFFFF;
        *command++ = CommandHeader(0x87);
        *command++ = (m_CubeMapAddr3 >> 3) & 0x3FFFFF;
        *command++ = CommandHeader(0x88);
        *command++ = (m_CubeMapAddr4 >> 3) & 0x3FFFFF;
        *command++ = CommandHeader(0x89);
        *command++ = (m_CubeMapAddr5 >> 3) & 0x3FFFFF;
        *command++ = CommandHeader(0x8A);
        break;
    }
    *command++ = m_Format;
    *command++ = CommandHeader(0x8E);
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
