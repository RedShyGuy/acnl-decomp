#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 21BsLightAmbientCatalog @ 0x008CCC50
// vtable 0x008F5EDC (vptr 0x008F5EE4), offset_to_top 0, 20 entries
class BsLightAmbientCatalog : public ::BsLightBase
{
public:
    class AmbientCatalogLightHioNode;
    BsLightAmbientCatalog(); // ctor address unknown
    virtual ~BsLightAmbientCatalog(); // 0x00325398 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00325374 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0032525C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00325338 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x003252E0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x003251F0 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0072582C slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x00325358 slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x48(); // 0x00325328 slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x003251EC slot 0x4C | virtual slot, introduced by BsLightAmbientCatalog
};
