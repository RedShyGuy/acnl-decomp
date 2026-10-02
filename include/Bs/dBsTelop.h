#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 7BsTelop @ 0x008CD37C
// vtable 0x008F9020 (vptr 0x008F9028), offset_to_top 0, 16 entries
// vtable 0x008F9068 (vptr 0x008F9070), offset_to_top -20, 3 entries
class BsTelop : public ::Base, public ::state::Mode<BsTelop>
{
public:
    struct TelopName { u32 _unknown; }; // TODO: real type unknown (placeholder)
    BsTelop(); // ctor address unknown
    virtual ~BsTelop(); // 0x0060CC50 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0060CBDC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0060C37C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0060CA24 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0060C734 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0060C35C slot 0x30 | slot vf_0x30 of oml::framework::Process
    void ShowTourTelop(BsTelop::TelopName, TourName); // 0x0060BBC8 | libgarden [tier A]
    void ShowTelop(BsTelop::TelopName); // 0x0060C900 | libgarden [tier A]
};
