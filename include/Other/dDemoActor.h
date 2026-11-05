#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "Other/dActor.h"
#include "Other/dListNodeId.h"

// RTTI 9DemoActor @ 0x008CD778
// vtable 0x008FA5D8 (vptr 0x008FA5E0), offset_to_top 0, 30 entries
class DemoActor : public ::Actor
{
public:
    // the node of the actor in the list of the demo actors; the id finds the actor (names are ours)
    // RTTI N9DemoActor17ListNodeDemoActorE @ 0x008D40D8
    // vtable 0x0090C258 (vptr 0x0090C260), offset_to_top 0, 2 entries
    class ListNodeDemoActor : public ::ListNodeId
    {
    public:
        ListNodeDemoActor(); // ctor address unknown
        virtual ~ListNodeDemoActor(); // 0x006E5120 slot 0x00
        // 0x006E511C slot 0x04 (deleting dtor)

        DemoActor* m_pActor; // 0x10
        u8 _14[0xC];         // 0x14
    };
    DemoActor(); // ctor address unknown
    virtual ~DemoActor(); // 0x006E535C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006E533C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual bool CanInitialize() const; // 0x006E5244 slot 0x08 | slot vf_0x08 of oml::framework::Process
    virtual void HandleInitializationResult(oml::framework::Result); // 0x00307C80 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual bool CanFinalize() const; // 0x006E528C slot 0x14 | slot vf_0x14 of oml::framework::Process
    virtual void vf_0x44(); // 0x00770824 slot 0x44 | virtual slot, introduced by AcStrcCampingCar
    virtual bool vf_0x48(DemoActor* actor); // 0x0077082C slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x4C(); // 0x00770578 slot 0x4C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x50(u8 message, netgame::PlayerNo player); // 0x006E5240 slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x54(); // 0x007707F8 slot 0x54 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x58(); // 0x0077056C slot 0x58 | virtual slot, introduced by DemoActor
    virtual void vf_0x5C(); // 0x0077055C slot 0x5C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x60(); // 0x007707F0 slot 0x60 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x64(); // 0x006E5238 slot 0x64 | virtual slot, introduced by AcStrcCampingCar
    virtual bool vf_0x68(u32 value); // 0x006E5050 slot 0x68 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x6C(); // 0x006E5068 slot 0x6C | virtual slot, introduced by AcStrcCampingCar
    virtual bool vf_0x70(DemoActor* actor); // 0x00770554 slot 0x70 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x74(); // 0x00770564 slot 0x74 | virtual slot, introduced by AcStrcCampingCar

    u32 GetId() const { return m_Node.m_Id; }

    ListNodeDemoActor m_Node; // 0x48 (size after libgarden)
};
ASSERT_SIZE(DemoActor, 0x68);
