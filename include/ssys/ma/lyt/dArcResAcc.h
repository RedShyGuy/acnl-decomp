#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dResAccInterface.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt9ArcResAccE @ 0x008D24B4
// vtable 0x00907348 (vptr 0x00907350), offset_to_top 0, 2 entries
class ArcResAcc : public ::ssys::ma::lyt::ResAccInterface
{
public:
    ArcResAcc(); // ctor address unknown
    virtual ~ArcResAcc(); // 0x0056A610 slot 0x00 | slot vf_0x00 of ssys::ma::lyt::ArcResAcc
    virtual void vf_0x04(); // 0x0056A5F0 slot 0x04 | virtual slot, introduced by ssys::ma::lyt::ArcResAcc
    void SetData(void const*, char const*); // 0x0056A580 | libgarden [tier A]
};
} // namespace lyt
} // namespace ma
} // namespace ssys
