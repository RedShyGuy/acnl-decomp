#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::Delegate<AcNpcSpResetsan>  typeinfo 0x008D2190  vtable 0x00906CC4
//   sead::Delegate<nfp::Framework>  typeinfo 0x008D219C  vtable 0x00906CD4
//   sead::Delegate<sead::CalculateTask>  typeinfo 0x008D21A8  vtable 0x00906CE4
//   sead::Delegate<sead::DualScreenTask>  typeinfo 0x008D21C0  vtable 0x00906D04
//   sead::Delegate<sead::FaderTaskBase>  typeinfo 0x008D21B4  vtable 0x00906CF4
//   sead::Delegate<sead::MethodTreeNode>  typeinfo 0x008D21CC  vtable 0x00906D14
//   sead::Delegate<sead::UlcdTask>  typeinfo 0x008D21D8  vtable 0x00906D24
template <typename T0>
class Delegate
{
public:
    // TODO: members unknown
};
} // namespace sead
