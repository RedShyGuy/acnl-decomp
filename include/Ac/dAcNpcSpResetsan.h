#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// RTTI 15AcNpcSpResetsan @ 0x008CBE90
// vtable 0x008F0E68 (vptr 0x008F0E70), offset_to_top 0, 89 entries
class AcNpcSpResetsan : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpResetsan(); // ctor address unknown
    virtual ~AcNpcSpResetsan(); // 0x0028217C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0028206C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Calc(); // 0x00281DC0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x4C(); // 0x0071B5D4 slot 0x4C | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x0010B071 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x0010B045 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x00280F2C slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x0010B091 slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x11C(); // 0x00281AD8 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x00281820 slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x124(); // 0x002818F8 slot 0x124 | virtual slot, introduced by AcNpc
    virtual void vf_0x144(); // 0x00281824 slot 0x144 | virtual slot, introduced by AcNpc
};
