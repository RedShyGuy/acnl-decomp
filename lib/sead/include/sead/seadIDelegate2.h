#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::IDelegate2<sead::Thread*, int>  typeinfo 0x008D13B0
//
// A callback object with two arguments; the one slot calls it (as pead::IDelegate2; the slot name
// is ours).
template <typename T0, typename T1>
class IDelegate2
{
public:
    virtual void invoke(T0 arg1, T1 arg2) = 0;
};
} // namespace sead
