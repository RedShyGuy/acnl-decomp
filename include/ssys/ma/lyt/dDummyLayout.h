#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_AnimResource.h"
#include "nw/lyt/lyt_Layout.h"

namespace ssys {
namespace ma {
namespace lyt {
// the nw layout of ssys: the animation transforms it makes do not stay in its list (the owner deletes them), and an
// animation with groups gets one transform per group
// RTTI N4ssys2ma3lyt11DummyLayoutE @ 0x008D2458
// vtable 0x00907280 (vptr 0x00907288), offset_to_top 0, 17 entries
class DummyLayout : public ::nw::lyt::Layout
{
public:
    DummyLayout() {} // (inline)
    virtual ~DummyLayout(); // 0x004BAA64 slot 0x00 (falls into the destructor of nw::lyt::Layout)

    using nw::lyt::Layout::CreateAnimTransform;
    nw::lyt::AnimTransform* CreateAnimTransform(const void* animResBuf, nw::lyt::ResourceAccessor* pResAcsr) override; // 0x00568770 slot 0x10
    bool BindAnimationAuto(const nw::lyt::AnimResource& animRes, nw::lyt::ResourceAccessor* pResAcsr) override; // 0x00568674 slot 0x28
};
ASSERT_SIZE(DummyLayout, 0x20);
} // namespace lyt
} // namespace ma
} // namespace ssys
