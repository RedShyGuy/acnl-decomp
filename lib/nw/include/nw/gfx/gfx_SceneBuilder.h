#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class SceneBuilder
{
public:
    void CreateObject(nw::os::IAllocator*, nw::os::IAllocator*); // 0x0048F670 | nintendogs:bytes [tier B]
    void BuildChildren(nw::os::MemorySizeCalculator*, nw::os::MemorySizeCalculator*, nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, nw::os::IAllocator*, nw::os::IAllocator*, bool) const; // 0x0073789C | nintendogs:bytes [tier A]
    void BuildSceneObject(nw::os::MemorySizeCalculator*, nw::os::MemorySizeCalculator*, nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, nw::os::IAllocator*, nw::os::IAllocator*, bool, bool) const; // 0x007379B8 | nintendogs:callgraph [tier A]
};
} // namespace gfx
} // namespace nw
