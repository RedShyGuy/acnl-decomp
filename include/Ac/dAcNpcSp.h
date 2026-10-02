#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// RTTI 7AcNpcSp @ 0x008CD33C
// vtable 0x008F8D00 (vptr 0x008F8D08), offset_to_top 0, 89 entries
class AcNpcSp : public ::AcNpc
{
public:
    class AcNpcSpHioNode;
    AcNpcSp(); // ctor address unknown
    virtual ~AcNpcSp(); // 0x00605F00 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00605E38 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Calc(); // 0x00605C30 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x78(); // 0x00300751 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x00303F21 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0xB4(); // 0x0075E148 slot 0xB4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF4(); // 0x00605B30 slot 0xF4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF8(); // 0x0075E15C slot 0xF8 | virtual slot, introduced by AcNpc
    virtual void vf_0xFC(); // 0x006059D4 slot 0xFC | virtual slot, introduced by AcNpc
    virtual void vf_0x100(); // 0x0075E154 slot 0x100 | virtual slot, introduced by AcNpc
    virtual void vf_0x118(); // 0x00605E04 slot 0x118 | virtual slot, introduced by AcNpc
    virtual void vf_0x150(); // 0x006053E4 slot 0x150 | virtual slot, introduced by AcNpc
};
