#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResShader.h"

namespace nw {
namespace gfx {
namespace res {
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
