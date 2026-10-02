#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 14BsLightAmbient @ 0x008CBB8C
// vtable 0x008EFC74 (vptr 0x008EFC7C), offset_to_top 0, 20 entries
class BsLightAmbient : public ::BsLightBase
{
public:
    class AmbientLightHioNode;
    BsLightAmbient(); // ctor address unknown
    virtual ~BsLightAmbient(); // 0x00256830 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0025680C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002566F0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002567D0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00256750 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0025669C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00719244 slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x002567F0 slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x48(); // 0x002567C0 slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x00256698 slot 0x4C | virtual slot, introduced by BsLightAmbient
};
