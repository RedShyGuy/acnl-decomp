#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// RTTI 13AcNpcSpRollan @ 0x008CB868
// vtable 0x008EEAB0 (vptr 0x008EEAB8), offset_to_top 0, 89 entries
class AcNpcSpRollan : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpRollan(); // ctor address unknown
    virtual ~AcNpcSpRollan(); // 0x00210FD4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00210EE0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x48(); // 0x00713F20 slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x64(); // 0x00210C04 slot 0x64 | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x0010A045 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x0010A019 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x00210434 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x0010A06D slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x90(); // 0x00210860 slot 0x90 | virtual slot, introduced by AcNpc
    virtual void vf_0xC8(); // 0x00210BB0 slot 0xC8 | virtual slot, introduced by AcNpc
    virtual void vf_0x114(); // 0x00210BF4 slot 0x114 | virtual slot, introduced by AcNpc
    virtual void vf_0x11C(); // 0x00210AC4 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x00210890 slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x144(); // 0x00210894 slot 0x144 | virtual slot, introduced by AcNpc
};
