#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dArcResAcc.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt15ArcResAccReaderE @ 0x008D2470
// vtable 0x009072DC (vptr 0x009072E4), offset_to_top 0, 2 entries
class ArcResAccReader : public ::ssys::ma::lyt::ArcResAcc
{
public:
    virtual ~ArcResAccReader(); // 0x005689D8 slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x005689A4 slot 0x04 | virtual slot, introduced by ssys::ma::lyt::ArcResAcc
    ArcResAccReader(); // 0x00120D68 | libgarden [tier A]
    void Clear(); // 0x00133A20 | libgarden [tier A]
    void ReadArc(char const*); // 0x0056890C | libgarden [tier A]
};
} // namespace lyt
} // namespace ma
} // namespace ssys
