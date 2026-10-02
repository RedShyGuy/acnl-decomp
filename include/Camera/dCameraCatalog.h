#pragma once

#include "decomp.h"
#include "Camera/dCameraBase.h"

// RTTI 13CameraCatalog @ 0x008CB9A0
// vtable 0x008EF20C (vptr 0x008EF214), offset_to_top 0, 6 entries
class CameraCatalog : public ::CameraBase
{
public:
    CameraCatalog(); // ctor candidate(s) 0x0022C010 (unverified)
    virtual void vf_0x00(); // 0x0022C07C slot 0x00 | virtual slot, introduced by CameraBase
    virtual void vf_0x04(); // 0x0022C078 slot 0x04 | virtual slot, introduced by CameraBase
    virtual void vf_0x08(); // 0x0071430C slot 0x08 | virtual slot, introduced by CameraBase
    virtual void vf_0x0C(); // 0x00714318 slot 0x0C | virtual slot, introduced by CameraBase
    virtual void vf_0x10(); // 0x0022C00C slot 0x10 | virtual slot, introduced by CameraBase
};
