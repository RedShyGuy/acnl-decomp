#pragma once

#include "decomp.h"

namespace sead {
class Heap;

// Instantiations found in the binary:
//   sead::IDelegateR<bool>  typeinfo 0x008D13B8
//
// A callback object that returns a value (slots as IDelegate; names are ours).
template <typename T0>
class IDelegateR
{
public:
    virtual T0 invoke() = 0;
    virtual IDelegateR* clone(Heap*) const { return nullptr; }
};
} // namespace sead
