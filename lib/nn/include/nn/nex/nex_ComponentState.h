#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14ComponentStateE @ 0x008CE2E4
// vtable 0x008FCAB8 (vptr 0x008FCAC0), offset_to_top 0, 16 entries
class ComponentState : public ::nn::nex::SystemComponent
{
public:
    ComponentState(); // ctor candidate(s) 0x0038678C (unverified)
    virtual ~ComponentState(); // 0x00370FC0 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x00370F68 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x08(); // 0x0072AF00 slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x0C(); // 0x0072AF28 slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x10(); // 0x00370F64 slot 0x10 | virtual slot, introduced by nn::nex::SystemComponent
};
} // namespace nex
} // namespace nn
