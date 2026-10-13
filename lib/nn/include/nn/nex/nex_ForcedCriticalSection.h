#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"

namespace nn {
namespace nex {
// A CriticalSection that also locks while locking is disabled. The names of the functions are
// ours.
// RTTI N2nn3nex21ForcedCriticalSectionE @ 0x008CEA90
// vtable 0x008FDCFC (vptr 0x008FDD04), offset_to_top 0, 5 entries
class ForcedCriticalSection : public ::nn::nex::CriticalSection
{
public:
    explicit ForcedCriticalSection(unsigned int level) : CriticalSection(level) {} // (inline)
    virtual ~ForcedCriticalSection(); // 0x003986AC slot 0x00
    // 0x00398680 slot 0x04 (deleting dtor)
    virtual void Enter(); // 0x00398670 slot 0x08
    virtual void Leave(); // 0x00398678 slot 0x0C
    // (inline in the original, with a copy out of line)
    virtual bool IsForced(); // 0x0072CE04 slot 0x10
};
} // namespace nex
} // namespace nn
