#pragma once

#include "decomp.h"
#include "Camera/dCameraBase.h"

// RTTI 10CameraGame @ 0x008CB034
// vtable 0x008EC140 (vptr 0x008EC148), offset_to_top 0, 6 entries
class CameraGame : public ::CameraBase
{
public:
    CameraGame(); // ctor candidate(s) 0x001A6E04 (unverified)
    virtual void vf_0x00(); // 0x001A7CEC slot 0x00 | virtual slot, introduced by CameraBase
    virtual void vf_0x04(); // 0x001A7CBC slot 0x04 | virtual slot, introduced by CameraBase
    virtual void vf_0x08(); // 0x0070B450 slot 0x08 | virtual slot, introduced by CameraBase
    virtual void vf_0x0C(); // 0x0070B488 slot 0x0C | virtual slot, introduced by CameraBase
    virtual void vf_0x10(); // 0x001A6BDC slot 0x10 | virtual slot, introduced by CameraBase
    virtual void vf_0x14(); // 0x001A6BD8 slot 0x14 | virtual slot, introduced by CameraBase
};
