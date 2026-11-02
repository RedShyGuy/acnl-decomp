#pragma once

#include "decomp.h"
#include "g3d/dFragmentLight.h"

namespace g3d {
// a fragment light of one kind (names are ours)
// RTTI N3g3d18PointFragmentLightE @ 0x008D0D30
// vtable 0x00903B10 (vptr 0x00903B18), offset_to_top 0, 7 entries
class PointFragmentLight : public ::g3d::FragmentLight
{
public:
    // (inline)
    PointFragmentLight() {}
    virtual ~PointFragmentLight(); // 0x004F0978 slot 0x00
    // 0x004F0954 (deleting dtor)
    // a point light at the origin; always true
    bool Initialize(nw::os::IAllocator* allocator) override; // 0x004F0904 slot 0x14
};
ASSERT_SIZE(PointFragmentLight, 0x11C);
} // namespace g3d
