#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Player/dPlayerSimpleMessage.h"
#include "Utl/dUtlBase.h"

// RTTI 8AcPlayer @ 0x008CD434
// vtable 0x008F96D8 (vptr 0x008F96E0), offset_to_top 0, 36 entries
// vtable 0x008F9770 (vptr 0x008F9778), offset_to_top -104, 63 entries
class AcPlayer : public ::UtlBase<DemoActor>, public ::PlayerSimpleMessage
{
public:
    class ToolFunctor;
    class ActionPosition;
    AcPlayer(); // ctor candidate(s) 0x0068EC58 (unverified)
    virtual ~AcPlayer(); // 0x0068F210 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0068F200 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void HandleInitializationResult(oml::framework::Result); // 0x001170A5 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void CanCalc() const; // 0x0064D6FC slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void Calc(); // 0x0068DB7C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0068D44C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00650CFC slot 0x40 | virtual slot, introduced by Actor
    virtual void vf_0x5C(); // 0x007631D4 slot 0x5C | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x0064DD30 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x001170B9 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x0064D0D0 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x84(); // 0x00117175 slot 0x84 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x0011715D slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x8C(); // 0x00117125 slot 0x8C | virtual slot, introduced by UtlBase<DemoActor>
    void DoAction(PlayerAction::Name, PlayerState const*, bool); // 0x0064DB90 | libgarden [tier A]
    void GetScratchState() const; // 0x006576F8 | libgarden [tier A]
};
