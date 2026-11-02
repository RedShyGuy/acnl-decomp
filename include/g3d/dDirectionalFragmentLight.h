#pragma once

#include "decomp.h"
#include "g3d/dFragmentLight.h"

namespace g3d {
// a fragment light of one kind (names are ours)
// RTTI N3g3d24DirectionalFragmentLightE @ 0x008D0D3C
// vtable 0x00903B34 (vptr 0x00903B3C), offset_to_top 0, 7 entries
class DirectionalFragmentLight : public ::g3d::FragmentLight
{
public:
    // (inline)
    DirectionalFragmentLight() {}
    virtual ~DirectionalFragmentLight(); // 0x004F0F28 slot 0x00
    // 0x004F0F04 (deleting dtor)
    // a directional light pointing down and to the side; always true
    bool Initialize(nw::os::IAllocator* allocator) override; // 0x004F0EA0 slot 0x14
};
ASSERT_SIZE(DirectionalFragmentLight, 0x11C);
} // namespace g3d
