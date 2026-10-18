#pragma once

#include "decomp.h"
#include "nn/nex/nex_RendezVousOperation.h"

namespace nn {
namespace nex {
// The logout operation of BackEndServices (OnLoginStart / OnLogoutStart).
// RTTI N2nn3nex25RendezVousLogoutOperationE @ 0x008CEEE8
// vtable 0x008FEB3C (vptr 0x008FEB44), offset_to_top 0, 6 entries
class RendezVousLogoutOperation : public ::nn::nex::RendezVousOperation
{
public:
    // the type number of the operation
    static const u32 TYPE = 22;

    // (inline)
    explicit RendezVousLogoutOperation(BackEndServices* pBackEndServices) : RendezVousOperation(pBackEndServices) {}
    virtual ~RendezVousLogoutOperation(); // 0x003B85E8 slot 0x00
    // 0x003B85D8 (deleting dtor)
    virtual u32 GetTypeImpl() const; // 0x0072D9A8 slot 0x0C
    virtual const wchar_t* GetName() const; // 0x0072D97C slot 0x10
    virtual void vf_0x14(); // 0x003B85D4 slot 0x14
};
} // namespace nex
} // namespace nn
