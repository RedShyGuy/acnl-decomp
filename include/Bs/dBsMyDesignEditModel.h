#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 19BsMyDesignEditModel @ 0x008CC9AC
// vtable 0x008F4EEC (vptr 0x008F4EF4), offset_to_top 0, 22 entries
class BsMyDesignEditModel : public ::UtlBase<Base>
{
public:
    BsMyDesignEditModel(); // ctor candidate(s) 0x002F5B94 (unverified)
    virtual ~BsMyDesignEditModel(); // 0x002F5DF8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002F5CF0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002F56D4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002F5428 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x002F4DA4 slot 0x40 | virtual slot, introduced by BsMyDesignEditModel
    virtual void vf_0x44(); // 0x002F4764 slot 0x44 | virtual slot, introduced by BsMyDesignEditModel
    virtual void vf_0x48(); // 0x002F3AC4 slot 0x48 | virtual slot, introduced by BsMyDesignEditModel
    virtual void vf_0x4C(); // 0x002F5064 slot 0x4C | virtual slot, introduced by BsMyDesignEditModel
    virtual void vf_0x50(); // 0x002F505C slot 0x50 | virtual slot, introduced by BsMyDesignEditModel
    virtual void vf_0x54(); // 0x002F4F18 slot 0x54 | virtual slot, introduced by BsMyDesignEditModel
};
