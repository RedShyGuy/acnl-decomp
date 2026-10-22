#include "nw/lyt/lyt_TexMap.h"

namespace nw {
namespace lyt {
// 0x004BAD50 | nintendogs:bytes [tier A]
nw::lyt::TexMap::TexMap() : m_TexObject(0), m_PhysicalAddress(0), m_Size(), m_RealSize()
{
    m_Bits.wrapS = WRAP_CLAMP;
    m_Bits.wrapT = WRAP_CLAMP;
    m_Bits.minFilter = FILTER_LINEAR;
    m_Bits.magFilter = FILTER_LINEAR;
    m_Bits.format = TEXFORMAT_MAX;
    Update();
}

// 0x004BACDC | libgarden [tier A]
nw::lyt::TexMap::TexMap(const TextureInfo& textureInfo)
{
    m_Bits.wrapS = WRAP_CLAMP;
    m_Bits.wrapT = WRAP_CLAMP;
    m_Bits.minFilter = FILTER_LINEAR;
    m_Bits.magFilter = FILTER_LINEAR;
    Set(textureInfo);
}

// 0x004BAB50 | libgarden [tier A]
void nw::lyt::TexMap::Update()
{
    // the PICA texture formats (after 3dbrew "GPU/Internal Registers", TEXTURE0_TYPE)
    switch (m_Bits.format) {
    case TEXFORMAT_L8:
        m_FormatReg = 7;
        break;
    case TEXFORMAT_A8:
        m_FormatReg = 8;
        break;
    case TEXFORMAT_LA4:
        m_FormatReg = 9;
        break;
    case TEXFORMAT_LA8:
        m_FormatReg = 5;
        break;
    case TEXFORMAT_HILO8:
        m_FormatReg = 6;
        break;
    case TEXFORMAT_RGB565:
        m_FormatReg = 3;
        break;
    case TEXFORMAT_RGB8:
        m_FormatReg = 1;
        break;
    case TEXFORMAT_RGB5A1:
        m_FormatReg = 2;
        break;
    case TEXFORMAT_RGBA4:
        m_FormatReg = 4;
        break;
    case TEXFORMAT_RGBA8:
        m_FormatReg = 0;
        break;
    case TEXFORMAT_ETC1:
        m_FormatReg = 12;
        break;
    case TEXFORMAT_ETC1A4:
        m_FormatReg = 13;
        break;
    case TEXFORMAT_L4:
        m_FormatReg = 10;
        break;
    case TEXFORMAT_A4:
        m_FormatReg = 11;
        break;
    }

    m_SizeReg = (m_RealSize.width << 16) | m_RealSize.height;

    // the filters (0: nearest, 3: linear; 1, 2, 4 and 5 are the mipmap modes) and the wrap modes
    // of the register (0: clamp to edge, 2: repeat, 3: mirrored repeat)
    const u32 filterModes[] = {0, 3};
    const u32 wrapModes[] = {0, 2, 3};

    const u32 minFilter = filterModes[m_Bits.minFilter];
    const u32 isMinLinear = (minFilter == 3 || minFilter == 4 || minFilter == 5) ? 1 : 0;
    const u32 isEtc1 = (m_FormatReg == 12) ? 2 : 0;
    const u32 isMipLinear = (minFilter == 2 || minFilter == 5) ? 1 : 0;
    m_ParamReg = (m_Bits.magFilter << 1) | (isMinLinear << 2) | (isEtc1 << 4) | (wrapModes[m_Bits.wrapT] << 8) |
                 (wrapModes[m_Bits.wrapS] << 12) | (isMipLinear << 24);
}

} // namespace lyt
} // namespace nw
