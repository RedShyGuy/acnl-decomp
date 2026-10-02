#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// RTTI 8AcNpcNml @ 0x008CD428
// vtable 0x008F952C (vptr 0x008F9534), offset_to_top 0, 105 entries
class AcNpcNml : public ::AcNpc
{
public:
    class TalkRcpt;
    AcNpcNml(); // ctor candidate(s) 0x0064CEA8 (unverified)
    virtual ~AcNpcNml(); // 0x0064CF28 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0064CF18 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Draw(); // 0x00632744 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x48(); // 0x00762FF8 slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x50(); // 0x0064CE5C slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x00117065 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x00116FB1 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x00116F4D slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x00117081 slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0xB0(); // 0x00762FDC slot 0xB0 | virtual slot, introduced by AcNpc
    virtual void vf_0xC0(); // 0x006320B4 slot 0xC0 | virtual slot, introduced by AcNpc
    virtual void vf_0xC4(); // 0x0063240C slot 0xC4 | virtual slot, introduced by AcNpc
    virtual void vf_0xD8(); // 0x00632704 slot 0xD8 | virtual slot, introduced by AcNpc
    virtual void vf_0xF4(); // 0x00632594 slot 0xF4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF8(); // 0x0076155C slot 0xF8 | virtual slot, introduced by AcNpc
    virtual void vf_0xFC(); // 0x006324D8 slot 0xFC | virtual slot, introduced by AcNpc
    virtual void vf_0x100(); // 0x00761118 slot 0x100 | virtual slot, introduced by AcNpc
    virtual void vf_0x11C(); // 0x006320F4 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x00631F58 slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x128(); // 0x00632188 slot 0x128 | virtual slot, introduced by AcNpc
    virtual void vf_0x150(); // 0x00632154 slot 0x150 | virtual slot, introduced by AcNpc
    virtual void vf_0x154(); // 0x00632478 slot 0x154 | virtual slot, introduced by AcNpc
    virtual void vf_0x158(); // 0x00632608 slot 0x158 | virtual slot, introduced by AcNpc
    virtual void vf_0x15C(); // 0x0063203C slot 0x15C | virtual slot, introduced by AcNpc
    virtual void vf_0x160(); // 0x00632078 slot 0x160 | virtual slot, introduced by AcNpc
    virtual void vf_0x164(); // 0x00760B94 slot 0x164 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x168(); // 0x0064CE58 slot 0x168 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x16C(); // 0x00632514 slot 0x16C | virtual slot, introduced by AcNpcNml
    virtual void vf_0x170(); // 0x00632404 slot 0x170 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x174(); // 0x0063258C slot 0x174 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x178(); // 0x00760B84 slot 0x178 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x17C(); // 0x00760B7C slot 0x17C | virtual slot, introduced by AcNpcNml
    virtual void vf_0x180(); // 0x00763080 slot 0x180 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x184(); // 0x0076164C slot 0x184 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x188(); // 0x00760BA0 slot 0x188 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x18C(); // 0x00760B8C slot 0x18C | virtual slot, introduced by AcNpcNml
    virtual void vf_0x190(); // 0x00763088 slot 0x190 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x194(); // 0x00760BA8 slot 0x194 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x198(); // 0x00632140 slot 0x198 | virtual slot, introduced by AcNpcNml
    virtual void vf_0x19C(); // 0x00631FB4 slot 0x19C | virtual slot, introduced by AcNpcNml
    virtual void vf_0x1A0(); // 0x00632604 slot 0x1A0 | virtual slot, introduced by AcNpcNml
};
