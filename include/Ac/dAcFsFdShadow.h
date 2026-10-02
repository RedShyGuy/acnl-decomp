#pragma once

#include "decomp.h"
#include "Ac/dAcFishFieldBase.h"
#include "Object/dObjectState.h"

// RTTI 12AcFsFdShadow @ 0x008CB388
// vtable 0x008ECE4C (vptr 0x008ECE54), offset_to_top 0, 50 entries
// vtable 0x008ECF20 (vptr 0x008ECF28), offset_to_top -208, 9 entries
// vtable 0x008ECF4C (vptr 0x008ECF54), offset_to_top -396, 2 entries
// vtable 0x008ECF84 (vptr 0x008ECF8C), offset_to_top -524, 11 entries
class AcFsFdShadow : public ::AcFishFieldBase, public ::ObjectState<AcFsFdShadow>
{
public:
    AcFsFdShadow(); // ctor candidate(s) 0x001ED99C, 0x001EDA60 (unverified)
    virtual ~AcFsFdShadow(); // 0x001EDA58 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001EDA44 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x48(); // 0x00712574 slot 0x48 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x60(); // 0x001ECD08 slot 0x60 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x64(); // 0x001EC740 slot 0x64 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x6C(); // 0x002FDF5C slot 0x6C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x70(); // 0x001EC008 slot 0x70 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x9C(); // 0x001EBC8C slot 0x9C | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xA0(); // 0x001EB41C slot 0xA0 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xA4(); // 0x00712598 slot 0xA4 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xA8(); // 0x001EB3F4 slot 0xA8 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xAC(); // 0x001EC200 slot 0xAC | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xB0(); // 0x001EC370 slot 0xB0 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xBC(); // 0x001EA180 slot 0xBC | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xC0(); // 0x001EA3D4 slot 0xC0 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xC4(); // 0x001EBBCC slot 0xC4 | virtual slot, introduced by AcFishFieldBase
};
