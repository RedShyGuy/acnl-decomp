#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// RTTI 15AcNpcSpTourDesk @ 0x008CBE9C
// vtable 0x008F0FD4 (vptr 0x008F0FDC), offset_to_top 0, 89 entries
class AcNpcSpTourDesk : public ::AcNpcSp
{
public:
    class AsyncAction;
    class TalkRecept;
    struct AssistPacketData { u32 _unknown; }; // TODO: real type unknown (placeholder)
    AcNpcSpTourDesk(); // ctor address unknown
    virtual ~AcNpcSpTourDesk(); // 0x002835EC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002834F8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Draw(); // 0x0057ADC8 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x4C(); // 0x0071B5EC slot 0x4C | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x0010B195 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x0010B149 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x0010B0D9 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x0010B1AD slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x90(); // 0x00282F1C slot 0x90 | virtual slot, introduced by AcNpc
    virtual void vf_0x11C(); // 0x00283144 slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x12C(); // 0x00283300 slot 0x12C | virtual slot, introduced by AcNpc
    virtual void vf_0x144(); // 0x00282F44 slot 0x144 | virtual slot, introduced by AcNpc
    virtual void vf_0x148(); // 0x002832B8 slot 0x148 | virtual slot, introduced by AcNpc
    void SendAssistPacket(netgame::PlayerNo, AcNpcSpTourDesk::AssistPacketData const&); // 0x007D1F24 | libgarden [tier A]
};
