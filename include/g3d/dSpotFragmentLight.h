#pragma once

#include "decomp.h"
#include "g3d/dFragmentLight.h"

namespace g3d {
// a fragment light of one kind (names are ours)
// RTTI N3g3d17SpotFragmentLightE @ 0x008D0D18
// vtable 0x00903AC0 (vptr 0x00903AC8), offset_to_top 0, 7 entries
class SpotFragmentLight : public ::g3d::FragmentLight
{
public:
    // (inline)
    SpotFragmentLight() {}
    virtual ~SpotFragmentLight(); // 0x004F0174 slot 0x00
    // 0x004F0150 (deleting dtor)
    // a spot light pointing down and to the side, at the origin, attenuated from 0 to 1000; always true
    bool Initialize(nw::os::IAllocator* allocator) override; // 0x004F00B0 slot 0x14
};
ASSERT_SIZE(SpotFragmentLight, 0x11C);
} // namespace g3d
