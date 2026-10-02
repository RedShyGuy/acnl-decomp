#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// RTTI 12AcNpcSpMaiko @ 0x008CB3B4
// vtable 0x008ED154 (vptr 0x008ED15C), offset_to_top 0, 89 entries
class AcNpcSpMaiko : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpMaiko(); // ctor address unknown
    virtual ~AcNpcSpMaiko(); // 0x001F3D58 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001F3C64 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Calc(); // 0x001F3870 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x4C(); // 0x007126D8 slot 0x4C | virtual slot, introduced by DemoActor
    virtual void vf_0x64(); // 0x001F38B0 slot 0x64 | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x001098AD slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x00109825 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x00109791 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x001098BD slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x90(); // 0x001F250C slot 0x90 | virtual slot, introduced by AcNpc
    virtual void vf_0xC8(); // 0x001F2C28 slot 0xC8 | virtual slot, introduced by AcNpc
    virtual void vf_0x114(); // 0x001F3830 slot 0x114 | virtual slot, introduced by AcNpc
    virtual void vf_0x11C(); // 0x001F28C8 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x00578488 slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x144(); // 0x001F255C slot 0x144 | virtual slot, introduced by AcNpc
};
