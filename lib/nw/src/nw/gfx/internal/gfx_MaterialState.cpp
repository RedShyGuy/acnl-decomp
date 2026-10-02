#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/internal/gfx_MaterialState.h"

namespace nw {
namespace gfx {
namespace internal {
// 0x004B0A94 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::internal::MaterialState::SetupTextureMatrix(nn::math::MTX44*, nw::gfx::res::ResTextureCoordinator::MappingMatrixMode, float, float, float, float, float)
{
}

// 0x004B0D2C | nintendogs:bytes [tier A]
void nw::gfx::internal::MaterialState::ActivateMaterialColor(const nw::gfx::SceneEnvironment&, const nw::gfx::ShaderProgram*, nw::gfx::res::ResMaterialColor, bool)
{
}

// 0x004B2134 | nintendogs:bytes [tier A]
void nw::gfx::internal::MaterialState::ActivateParticleTextureCoordinators(nw::gfx::RenderContext*, const nw::gfx::ShaderProgram*, nw::gfx::res::ResMaterial)
{
}

} // namespace internal
} // namespace gfx
} // namespace nw
