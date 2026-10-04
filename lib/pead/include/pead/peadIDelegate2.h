#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::IDelegate2<pead::Thread*, int>  typeinfo 0x008D113C
//
// A callback object with two arguments; the one slot calls it (the slot name is ours).
template <typename T0, typename T1>
class IDelegate2
{
public:
    virtual void invoke(T0 arg1, T1 arg2) = 0;
};
} // namespace pead
