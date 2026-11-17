#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "Other/dDemoActor.h"
#include "Utl/dUtlBase.h"

// RTTI 16AcSimpleTalkBase @ 0x008CC190
// vtable 0x008F2120 (vptr 0x008F2128), offset_to_top 0, 37 entries
class AcSimpleTalkBase : public ::UtlBase<DemoActor>
{
public:
    AcSimpleTalkBase(); // ctor candidate(s) 0x002A8B2C (unverified)
    virtual ~AcSimpleTalkBase(); // 0x002A8B6C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002A8B5C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x002A8A90 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual bool vf_0x48(DemoActor* actor); // 0x0071D800 slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x4C(); // 0x0071D5F0 slot 0x4C | virtual slot, introduced by DemoActor
    virtual void vf_0x50(u8 message, netgame::PlayerNo player); // 0x002A8AA4 slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x5C(); // 0x0071D5E0 slot 0x5C | virtual slot, introduced by DemoActor
    virtual void vf_0x60(); // 0x0071D7F8 slot 0x60 | virtual slot, introduced by DemoActor
    bool StartInitialize() override; // 0x002A8A70 slot 0x78
    oml::framework::Result UpdateInitialize() override; // 0x002A8A68 slot 0x7C
    bool EndInitialize() override; // 0x002A8A3C slot 0x80
    bool StartFinalize() override; // 0x002A8A88 slot 0x84
    oml::framework::Result UpdateFinalize() override; // 0x002A8A80 slot 0x88
    bool EndFinalize() override; // 0x002A8A78 slot 0x8C
    virtual void vf_0x90(); // 0x0071D5E8 slot 0x90 | virtual slot, introduced by AcSimpleTalkBase
};
