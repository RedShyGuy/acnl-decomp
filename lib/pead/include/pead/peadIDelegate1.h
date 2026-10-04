#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::IDelegate1<pead::PrintConfig::PrintEventArg const&>  typeinfo 0x008D1134
//
// A callback object with one argument; the one slot calls it (the slot name is ours).
template <typename T0>
class IDelegate1
{
public:
    virtual void invoke(T0 arg) = 0;
};
} // namespace pead
