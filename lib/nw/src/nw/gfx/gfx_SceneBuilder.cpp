#include "nw/gfx/gfx_SceneBuilder.h"

namespace nw {
namespace gfx {
// 0x0048F670 | nintendogs:bytes [tier B]
void nw::gfx::SceneBuilder::CreateObject(nw::os::IAllocator*, nw::os::IAllocator*)
{
}

// 0x0073789C | nintendogs:bytes [tier A]
void nw::gfx::SceneBuilder::BuildChildren(nw::os::MemorySizeCalculator*, nw::os::MemorySizeCalculator*, nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, nw::os::IAllocator*, nw::os::IAllocator*, bool) const
{
}

// 0x007379B8 | nintendogs:callgraph [tier A]
void nw::gfx::SceneBuilder::BuildSceneObject(nw::os::MemorySizeCalculator*, nw::os::MemorySizeCalculator*, nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, nw::os::IAllocator*, nw::os::IAllocator*, bool, bool) const
{
}

} // namespace gfx
} // namespace nw
