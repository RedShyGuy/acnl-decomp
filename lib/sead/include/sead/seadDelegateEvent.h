#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::DelegateEvent<char const*>  typeinfo 0x008D162C  vtable 0x009054C0
//   sead::DelegateEvent<sead::TaskBase*>  typeinfo 0x008D1640  vtable 0x009054E0
//   sead::DelegateEvent<sead::TaskBase*>::Slot  typeinfo 0x008D1634  vtable 0x009054D0
//   sead::DelegateEvent<void*>  typeinfo 0x008D1648  vtable 0x009054F0
template <typename T0>
class DelegateEvent
{
public:
    // TODO: members unknown
};
} // namespace sead
