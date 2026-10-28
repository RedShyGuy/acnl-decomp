#include "nw/gfx/gfx_SceneContext.h"

namespace nw {
namespace gfx {
// 0x0049108C slot 0x00 | nintendogs:bytes
// 0x00491080 (deleting dtor)
nw::gfx::SceneContext::~SceneContext()
{
    // (only the members are destroyed)
}

} // namespace gfx
} // namespace nw
