#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// RTTI 11AcNpcSpShop @ 0x008CB220
// vtable 0x008EC60C (vptr 0x008EC614), offset_to_top 0, 100 entries
class AcNpcSpShop : public ::AcNpcSp
{
public:
    class ShopNpcTalkRecept;
    class AcNpcSpShopHioNode;
    AcNpcSpShop(); // ctor candidate(s) 0x001C0934 (unverified)
    virtual ~AcNpcSpShop(); // 0x001C0A7C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001C09B4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Calc(); // 0x00605C2C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x48(); // 0x0070E2D8 slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x50(); // 0x0057BB04 slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x54(); // 0x0070DD10 slot 0x54 | virtual slot, introduced by DemoActor
    virtual void vf_0x58(); // 0x00752DB4 slot 0x58 | virtual slot, introduced by DemoActor
    virtual void vf_0x5C(); // 0x0070D9EC slot 0x5C | virtual slot, introduced by DemoActor
    virtual void vf_0x60(); // 0x0070DC60 slot 0x60 | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x00106C11 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x001BF064 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x8C(); // 0x001BF310 slot 0x8C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x90(); // 0x00578340 slot 0x90 | virtual slot, introduced by AcNpc
    virtual void vf_0x11C(); // 0x006E5080 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x148(); // 0x001BF7E4 slot 0x148 | virtual slot, introduced by AcNpc
    virtual void vf_0x164(); // 0x0070DA7C slot 0x164 | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x168(); // 0x0070DA64 slot 0x168 | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x16C(); // 0x0070DC58 slot 0x16C | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x170(); // 0x001BF408 slot 0x170 | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x174(); // 0x0070DE08 slot 0x174 | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x178(); // 0x001C0874 slot 0x178 | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x17C(); // 0x0070DDC0 slot 0x17C | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x180(); // 0x0011C12F slot 0x180 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x184(); // 0x0011C12F slot 0x184 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x188(); // 0x0070DC50 slot 0x188 | virtual slot, introduced by AcNpcSpShop
    virtual void vf_0x18C(); // 0x0070DAC8 slot 0x18C | virtual slot, introduced by AcNpcSpShop
};
