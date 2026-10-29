#pragma once

#include "decomp.h"

namespace pead {
namespace hostio {
// RTTI N4pead6hostio13LifeCheckableE @ 0x008D12C0
//
// The hostio classes of pead are empty and have no vtable (pead::Heap starts its own members right behind
// INamable at 0x18); only their RTTI is in the binary.
class LifeCheckable
{
public:
    LifeCheckable() {}
};
} // namespace hostio
} // namespace pead
