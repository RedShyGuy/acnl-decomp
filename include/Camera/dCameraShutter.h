#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 13CameraShutter @ 0x008CB9AC
// vtable 0x008EF22C (vptr 0x008EF234), offset_to_top 0, 3 entries
class CameraShutter : public ::state::Mode<CameraShutter>
{
public:
    CameraShutter(); // ctor address unknown
    virtual void vf_0x00(); // 0x0022C210 slot 0x00 | virtual slot, introduced by CameraShutter
    virtual void vf_0x04(); // 0x0022C1E0 slot 0x04 | virtual slot, introduced by CameraShutter
    virtual void vf_0x08(); // 0x0082AFE0 slot 0x08 | virtual slot, introduced by CameraShutter
};
