#pragma once

#include "decomp.h"
#include "Human/dHumanModel.h"

// RTTI 12NpcModelBase @ 0x008CB7A8
// vtable 0x008EE6D4 (vptr 0x008EE6DC), offset_to_top 0, 20 entries
class NpcModelBase : public ::HumanModel
{
public:
    NpcModelBase(); // ctor candidate(s) 0x0020674C (unverified)
    virtual void vf_0x00(); // 0x0020639C slot 0x00 | virtual slot, introduced by HumanModel
    virtual void vf_0x08(); // 0x004F14A0 slot 0x08 | virtual slot, introduced by HumanModel
    virtual void vf_0x0C(); // 0x00206534 slot 0x0C | virtual slot, introduced by HumanModel
    virtual void vf_0x10(); // 0x001AC808 slot 0x10 | virtual slot, introduced by HumanModel
    virtual void vf_0x14(); // 0x004EE434 slot 0x14 | virtual slot, introduced by HumanModel
    virtual void vf_0x18(); // 0x0020630C slot 0x18 | virtual slot, introduced by HumanModel
    virtual ~NpcModelBase(); // 0x00206954 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x002068B8 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x24(); // 0x00204228 slot 0x24 | virtual slot, introduced by HumanModel
    virtual void vf_0x2C(); // 0x00304260 slot 0x2C | virtual slot, introduced by NpcModelBase
    virtual void vf_0x30(); // 0x002058A8 slot 0x30 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x34(); // 0x00206668 slot 0x34 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x3C(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x48(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x4C(); // 0x00205E9C slot 0x4C | virtual slot, introduced by NpcModelBase
};
