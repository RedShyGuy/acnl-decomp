#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt16ResourceAccessorE @ 0x008D0848
// vtable 0x00902B28 (vptr 0x00902B30), offset_to_top 0, 7 entries
class ResourceAccessor
{
public:
    ResourceAccessor(); // ctor candidate(s) 0x001322B8 (unverified)
    virtual ~ResourceAccessor(); // 0x004B5E84 slot 0x00 | slot vf_0x00 of nw::lyt::ResourceAccessor
    // 0x004B5E80 slot 0x04 | slot vf_0x04 of nw::lyt::ResourceAccessor (deleting dtor)
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void GetTexture(char const*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x004B5D38 slot 0x14 | virtual slot, introduced by nw::lyt::ResourceAccessor
    virtual void vf_0x18(); // 0x004B5DD4 slot 0x18 | virtual slot, introduced by nw::lyt::ResourceAccessor
};
} // namespace lyt
} // namespace nw
