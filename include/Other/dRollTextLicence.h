#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 15RollTextLicence @ 0x008CC13C
// vtable 0x008F1F28 (vptr 0x008F1F30), offset_to_top 0, 3 entries
class RollTextLicence : public ::state::Mode<RollTextLicence>
{
public:
    RollTextLicence(); // ctor address unknown
    virtual void vf_0x00(); // 0x002A30F4 slot 0x00 | virtual slot, introduced by RollTextLicence
    virtual void vf_0x04(); // 0x002A30A4 slot 0x04 | virtual slot, introduced by RollTextLicence
    virtual void vf_0x08(); // 0x0082B940 slot 0x08 | virtual slot, introduced by RollTextLicence
};
