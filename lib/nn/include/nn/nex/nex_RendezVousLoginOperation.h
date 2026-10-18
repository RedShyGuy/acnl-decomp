#pragma once

#include "decomp.h"
#include "nn/nex/nex_RendezVousOperation.h"

namespace nn {
namespace nex {
// The login operation of BackEndServices (OnLoginStart / OnLogoutStart).
// RTTI N2nn3nex24RendezVousLoginOperationE @ 0x008CEE2C
// vtable 0x008FE938 (vptr 0x008FE940), offset_to_top 0, 6 entries
class RendezVousLoginOperation : public ::nn::nex::RendezVousOperation
{
public:
    // the type number of the operation
    static const u32 TYPE = 21;

    // (inline)
    explicit RendezVousLoginOperation(BackEndServices* pBackEndServices) : RendezVousOperation(pBackEndServices) {}
    virtual ~RendezVousLoginOperation(); // 0x003B5770 slot 0x00
    // 0x003B5760 (deleting dtor)
    virtual u32 GetTypeImpl() const; // 0x0072D598 slot 0x0C
    virtual const wchar_t* GetName() const; // 0x0072D570 slot 0x10
    virtual void vf_0x14(); // 0x003B575C slot 0x14
};
} // namespace nex
} // namespace nn
