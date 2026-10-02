#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// RTTI 12AcNpcSpHonma @ 0x008CB3A8
// vtable 0x008ECFE8 (vptr 0x008ECFF0), offset_to_top 0, 89 entries
class AcNpcSpHonma : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpHonma(); // ctor address unknown
    virtual ~AcNpcSpHonma(); // 0x001F20B8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001F1FA8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x4C(); // 0x007125A4 slot 0x4C | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x00109719 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x001096ED slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x00109645 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x0010976D slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x11C(); // 0x001F1E20 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x001F1CDC slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x144(); // 0x001F1DD8 slot 0x144 | virtual slot, introduced by AcNpc
};
