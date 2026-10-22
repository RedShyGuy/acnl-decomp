#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResShader.h"

namespace nw {
namespace gfx {
namespace res {
// The data of a material resource (only the fields known so far; names are ours). The hashes say
// which state the material activator sent last; an animation of a part sets its hash to 0, so the
// part is sent again.
struct ResMaterialData
{
    u8 m_Unknown0x0[0x2A8];        // 0x0
    u32 textureCoordinatorsHash;   // 0x2A8
    u32 textureSamplersHash;       // 0x2AC
    u32 textureMappersHash;        // 0x2B0
    u32 materialColorHash;         // 0x2B4
    u8 m_Unknown0x2B8[0x18];       // 0x2B8
    u32 fragmentOperationHash;     // 0x2D0
};
ASSERT_OFFSET(ResMaterialData, fragmentOperationHash, 0x2D0);

class ResMaterial
{
public:
    void Cleanup(); // 0x0013B800 | nintendogs:bytes [tier A]
    void SetupShader(nw::os::IAllocator*, nw::gfx::res::ResShader, nw::gfx::res::ResGraphicsFile); // 0x004A5CA0 | nintendogs:bytes [tier A]
    void SetupTextures(nw::os::IAllocator*, nw::gfx::res::ResMaterial, nw::gfx::res::ResGraphicsFile); // 0x004A5DF4 | nintendogs:bytes-fuzzy [tier A]
    void CacheUserUniformIndex(nw::ut::internal::ResArray<nw::gfx::res::ResShaderSymbol, nw::ut::internal::ResArrayClassTraits>); // 0x004A62B0 | nintendogs:bytes [tier A]
    void CalcFragmentLightingTableHash(); // 0x004A6380 | nintendogs:bytes [tier A]
    void Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A662C | nintendogs:bytes [tier A]
};
} // namespace res
} // namespace gfx
} // namespace nw
