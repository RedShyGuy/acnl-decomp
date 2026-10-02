#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResMaterial.h"

namespace nw {
namespace gfx {
namespace internal {
class MaterialState
{
public:
    void SetupTextureMatrix(nn::math::MTX44*, nw::gfx::res::ResTextureCoordinator::MappingMatrixMode, float, float, float, float, float); // 0x004B0A94 | nintendogs:bytes-fuzzy [tier A]
    void ActivateMaterialColor(const nw::gfx::SceneEnvironment&, const nw::gfx::ShaderProgram*, nw::gfx::res::ResMaterialColor, bool); // 0x004B0D2C | nintendogs:bytes [tier A]
    void ActivateParticleTextureCoordinators(nw::gfx::RenderContext*, const nw::gfx::ShaderProgram*, nw::gfx::res::ResMaterial); // 0x004B2134 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace gfx
} // namespace nw
