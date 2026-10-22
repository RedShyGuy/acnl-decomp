#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx11SceneObjectE @ 0x008D0580
// A gfx object made from a scene object resource (members are ours).
class SceneObject : public ::nw::gfx::GfxObject
{
public:
    // (inline)
    SceneObject(os::IAllocator* pAllocator, res::ResSceneObject resObject) : GfxObject(pAllocator), m_ResSceneObject(resObject) {}
    SceneObject(); // ctor address unknown (for the derived classes not written yet)

    res::ResSceneObject GetResSceneObject() const { return m_ResSceneObject; }

    static const TypeInfo s_TypeInfo;

    res::ResSceneObject m_ResSceneObject; // 0x8
};
ASSERT_SIZE(SceneObject, 0xC);
} // namespace gfx
} // namespace nw
