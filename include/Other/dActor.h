#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 5Actor @ 0x008CD298
// vtable 0x008F89B0 (vptr 0x008F89B8), offset_to_top 0, 17 entries
class Actor : public ::Base
{
public:
    Actor(); // ctor candidate(s) 0x0057C550 (unverified)
    virtual ~Actor(); // 0x0057C654 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0057C640 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanInitialize() const; // 0x0057C528 slot 0x08 | slot vf_0x08 of oml::framework::Process
    virtual void HandleInitializationResult(oml::framework::Result); // 0x0057C470 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void CanFinalize() const; // 0x0057C53C slot 0x14 | slot vf_0x14 of oml::framework::Process
    virtual void HandleFinalizationResult(oml::framework::Result); // 0x0057C474 slot 0x1C | slot vf_0x1C of oml::framework::Process
    virtual void CanCalc() const; // 0x0057C478 slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void HandleCalcResult(oml::framework::Result); // 0x0057C50C slot 0x28 | slot vf_0x28 of oml::framework::Process
    virtual void CanDraw() const; // 0x0057C514 slot 0x2C | slot vf_0x2C of oml::framework::Process
    virtual void ProcessDrawResult(oml::framework::Result); // 0x005220E4 slot 0x34 | slot vf_0x34 of oml::framework::Process
    virtual void Unk0(); // 0x007537F0 slot 0x3C | slot vf_0x3C of Base
    virtual void vf_0x40(); // 0x0057C510 slot 0x40 | virtual slot, introduced by AcStrcCampingCar
    void GetPositionTo(nn::math::Vector<float, 3u>&, unsigned short, float, bool) const; // 0x0076330C | libgarden [tier A]
};
