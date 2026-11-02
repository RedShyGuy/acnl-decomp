#pragma once

#include "decomp.h"
#include "g3d/dBaseLight.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/ut/ut_FloatColor.h"

namespace nw {
namespace gfx {
class AmbientLight;
} // namespace gfx
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace g3d {
// an ambient light (names are ours)
// RTTI N3g3d12AmbientLightE @ 0x008D0C94
// vtable 0x00903974 (vptr 0x0090397C), offset_to_top 0, 5 entries
class AmbientLight : public ::g3d::BaseLight
{
public:
    AmbientLight(); // 0x004ECF44
    virtual ~AmbientLight(); // 0x004EEB98 slot 0x00 (falls into the one of TransformNode)
    // 0x004ECF78 (deleting dtor)
    void Update() override; // 0x004EEB3C slot 0x08 (falls into the one of TransformNode)
    // a gray light (0.2) in the memory of allocator (NULL: the one of the engine); always true
    virtual bool Initialize(nw::os::IAllocator* allocator); // 0x004ECEA8 slot 0x10

    // always true
    bool SetColor(const nw::ut::FloatColor& color); // 0x004ECE74

    nw::gfx::res::ResAmbientLight m_ResLight;   // 0x0C
    nw::gfx::AmbientLight* m_pLight;            // 0x10
};
ASSERT_SIZE(AmbientLight, 0x14);
} // namespace g3d
