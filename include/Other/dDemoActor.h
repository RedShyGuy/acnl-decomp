#pragma once

#include "decomp.h"
#include "Other/dActor.h"

// RTTI 9DemoActor @ 0x008CD778
// vtable 0x008FA5D8 (vptr 0x008FA5E0), offset_to_top 0, 30 entries
class DemoActor : public ::Actor
{
public:
    class ListNodeDemoActor;
    DemoActor(); // ctor address unknown
    virtual ~DemoActor(); // 0x006E535C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006E533C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanInitialize() const; // 0x006E5244 slot 0x08 | slot vf_0x08 of oml::framework::Process
    virtual void HandleInitializationResult(oml::framework::Result); // 0x00307C80 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void CanFinalize() const; // 0x006E528C slot 0x14 | slot vf_0x14 of oml::framework::Process
    virtual void vf_0x44(); // 0x00770824 slot 0x44 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x48(); // 0x0077082C slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x4C(); // 0x00770578 slot 0x4C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x50(); // 0x006E5240 slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x54(); // 0x007707F8 slot 0x54 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x58(); // 0x0077056C slot 0x58 | virtual slot, introduced by DemoActor
    virtual void vf_0x5C(); // 0x0077055C slot 0x5C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x60(); // 0x007707F0 slot 0x60 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x64(); // 0x006E5238 slot 0x64 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x68(); // 0x006E5050 slot 0x68 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x6C(); // 0x006E5068 slot 0x6C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x70(); // 0x00770554 slot 0x70 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x74(); // 0x00770564 slot 0x74 | virtual slot, introduced by AcStrcCampingCar
};
