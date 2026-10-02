#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"

// RTTI 10AcBrochure @ 0x008CAF10
// vtable 0x008EB59C (vptr 0x008EB5A4), offset_to_top 0, 30 entries
class AcBrochure : public ::DemoActor
{
public:
    AcBrochure(); // ctor candidate(s) 0x007EBC94 (unverified)
    virtual ~AcBrochure(); // 0x006E5358 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00313B14 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanInitialize() const; // 0x00189FB8 slot 0x08 | slot vf_0x08 of oml::framework::Process
    virtual void HandleInitializationResult(oml::framework::Result); // 0x00189F18 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void Calc(); // 0x00189FB0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00189FA8 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x48(); // 0x0070B278 slot 0x48 | virtual slot, introduced by DemoActor
};
