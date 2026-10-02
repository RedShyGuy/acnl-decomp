#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 7BsScene @ 0x008CD348
// vtable 0x008F8E6C (vptr 0x008F8E74), offset_to_top 0, 17 entries
class BsScene : public ::Base
{
public:
    BsScene(); // ctor candidate(s) 0x00607938 (unverified)
    virtual ~BsScene(); // 0x00313AF4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00607974 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanInitialize() const; // 0x006078B0 slot 0x08 | slot vf_0x08 of oml::framework::Process
    virtual void HandleInitializationResult(oml::framework::Result); // 0x00607194 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void CanFinalize() const; // 0x00607924 slot 0x14 | slot vf_0x14 of oml::framework::Process
    virtual void HandleFinalizationResult(oml::framework::Result); // 0x00607198 slot 0x1C | slot vf_0x1C of oml::framework::Process
    virtual void CanCalc() const; // 0x006071DC slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void HandleCalcResult(oml::framework::Result); // 0x006074F8 slot 0x28 | slot vf_0x28 of oml::framework::Process
    virtual void CanDraw() const; // 0x00607898 slot 0x2C | slot vf_0x2C of oml::framework::Process
    virtual void ProcessDrawResult(oml::framework::Result); // 0x006078AC slot 0x34 | slot vf_0x34 of oml::framework::Process
    virtual void Unk0(); // 0x0075E164 slot 0x3C | slot vf_0x3C of Base
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
};
