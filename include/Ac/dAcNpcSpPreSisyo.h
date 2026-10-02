#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// RTTI 15AcNpcSpPreSisyo @ 0x008CBE84
// vtable 0x008F0CFC (vptr 0x008F0D04), offset_to_top 0, 89 entries
class AcNpcSpPreSisyo : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpPreSisyo(); // ctor address unknown
    virtual ~AcNpcSpPreSisyo(); // 0x0027F7D4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0027F6E0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x4C(); // 0x00770574 slot 0x4C | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x0010B011 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x0010AFE5 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x0010AF91 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x0010B021 slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x11C(); // 0x0027F184 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x0027EEEC slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x144(); // 0x0027EEF0 slot 0x144 | virtual slot, introduced by AcNpc
};
