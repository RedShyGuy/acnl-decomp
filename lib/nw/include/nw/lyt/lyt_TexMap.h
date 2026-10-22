#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Types.h"

namespace nw {
namespace lyt {
// A texture of a material with its wrap modes and filters, and the PICA register values made
// from them (Update). Member names are ours.
class TexMap
{
public:
    // the wrap modes (GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT)
    enum WrapMode
    {
        WRAP_CLAMP,
        WRAP_REPEAT,
        WRAP_MIRROR
    };
    // the filters (GL_NEAREST, GL_LINEAR)
    enum FilterMode
    {
        FILTER_NEAREST,
        FILTER_LINEAR
    };

    TexMap(); // 0x004BAD50 | nintendogs:bytes [tier A]
    explicit TexMap(const TextureInfo& textureInfo); // 0x004BACDC | libgarden [tier A]

    // the register values from the texture and its settings (libgarden also names the address
    // ResetU32Info)
    void Update(); // 0x004BAB50 | libgarden [tier A]

    // (inline)
    void Set(const TextureInfo& textureInfo)
    {
        m_TexObject = textureInfo.texObject;
        m_PhysicalAddress = textureInfo.physicalAddress;
        m_Size = textureInfo.size;
        m_RealSize = textureInfo.realSize;
        m_Bits.format = textureInfo.format;
        Update();
    }
    void SetWrapMode(u8 wrapS, u8 wrapT)
    {
        m_Bits.wrapS = wrapS;
        m_Bits.wrapT = wrapT;
        Update();
    }
    void SetFilter(u8 minFlt, u8 magFlt)
    {
        m_Bits.minFilter = minFlt;
        m_Bits.magFilter = magFlt;
        Update();
    }
    u8 GetWrapModeS() const { return m_Bits.wrapS; }
    u8 GetWrapModeT() const { return m_Bits.wrapT; }
    u8 GetMinFilter() const { return m_Bits.minFilter; }
    u8 GetMagFilter() const { return m_Bits.magFilter; }

    struct Bits
    {
        u32 wrapS : 2;      // WrapMode
        u32 wrapT : 2;
        u32 minFilter : 3;  // FilterMode
        u32 magFilter : 1;
        u32 format : 4;     // TexFormat
    };

    u32 m_TexObject;        // 0x00
    u32 m_PhysicalAddress;  // 0x04
    TexSize m_Size;         // 0x08
    TexSize m_RealSize;     // 0x0C
    Bits m_Bits;            // 0x10
    u32 m_ParamReg;         // 0x14 the texture parameters (PICA register 0x83: filters, wrap, ETC1)
    u32 m_SizeReg;          // 0x18 width << 16 | height of the texture (register 0x82)
    u32 m_FormatReg;        // 0x1C the PICA texture format (register 0x8E)
};
ASSERT_SIZE(TexMap, 0x20);
} // namespace lyt
} // namespace nw
