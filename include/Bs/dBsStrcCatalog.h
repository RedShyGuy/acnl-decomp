#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 13BsStrcCatalog @ 0x008CB97C
// vtable 0x008EF0EC (vptr 0x008EF0F4), offset_to_top 0, 22 entries
class BsStrcCatalog : public ::UtlBase<Base>
{
public:
    BsStrcCatalog(); // ctor candidate(s) 0x00229204 (unverified)
    virtual ~BsStrcCatalog(); // 0x002293A8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00229398 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0022904C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002278FC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00227764 slot 0x40 | virtual slot, introduced by BsStrcCatalog
    virtual void vf_0x44(); // 0x00227564 slot 0x44 | virtual slot, introduced by BsStrcCatalog
    virtual void vf_0x48(); // 0x002270E0 slot 0x48 | virtual slot, introduced by BsStrcCatalog
    virtual void vf_0x4C(); // 0x002278E4 slot 0x4C | virtual slot, introduced by BsStrcCatalog
    virtual void vf_0x50(); // 0x002278D4 slot 0x50 | virtual slot, introduced by BsStrcCatalog
    virtual void vf_0x54(); // 0x002277FC slot 0x54 | virtual slot, introduced by BsStrcCatalog
};
