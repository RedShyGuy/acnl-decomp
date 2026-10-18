#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"

namespace nn {
namespace nex {
// The state machine of a class that is no SystemComponent itself (ConnectionManager +0x08).
// RTTI N2nn3nex14ComponentStateE @ 0x008CE2E4
// vtable 0x008FCAB8 (vptr 0x008FCAC0), offset_to_top 0, 16 entries
class ComponentState : public ::nn::nex::SystemComponent
{
public:
    // (inline)
    explicit ComponentState(const nn::nex::String& name) : SystemComponent(name) {}
    virtual ~ComponentState(); // 0x00370FC0 slot 0x00
    // 0x00370F68 (deleting dtor)
    // (inline in the original, with a copy out of line)
    virtual const wchar_t* GetType() const; // 0x0072AF00 slot 0x08
    virtual bool IsAKindOf(const wchar_t* pType) const; // 0x0072AF28 slot 0x0C
    virtual void vf_0x10(); // 0x00370F64 slot 0x10
};
} // namespace nex
} // namespace nn
