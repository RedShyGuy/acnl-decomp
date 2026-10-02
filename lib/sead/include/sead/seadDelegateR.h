#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::DelegateR<AcNpc, bool>  typeinfo 0x008D2350  vtable 0x00907028
//   sead::DelegateR<AcNpcSp, bool>  typeinfo 0x008D235C  vtable 0x00907038
//   sead::DelegateR<AcNpcSpMaiko, bool>  typeinfo 0x008D2338  vtable 0x00907008
//   sead::DelegateR<MoveFromTalkReceptBase, bool>  typeinfo 0x008D2344  vtable 0x00907018
//   sead::DelegateR<SeaDemoCtrl, bool>  typeinfo 0x008D232C  vtable 0x00906FF8
template <typename T0, typename T1>
class DelegateR
{
public:
    // TODO: members unknown
};
} // namespace sead
