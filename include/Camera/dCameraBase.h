#pragma once

#include "decomp.h"

// RTTI 10CameraBase @ 0x008CB02C
// vtable 0x008EC120 (vptr 0x008EC128), offset_to_top 0, 6 entries
class CameraBase
{
public:
    CameraBase(); // ctor address unknown
    virtual void vf_0x00(); // 0x001A17B8 slot 0x00 | virtual slot, introduced by CameraBase
    virtual void vf_0x04(); // 0x001A17B4 slot 0x04 | virtual slot, introduced by CameraBase
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x001A164C slot 0x10 | virtual slot, introduced by CameraBase
    virtual void vf_0x14(); // 0x001A1648 slot 0x14 | virtual slot, introduced by CameraBase
};
