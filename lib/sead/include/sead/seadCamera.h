#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead6CameraE @ 0x008D20A8
// vtable 0x00906B60 (vptr 0x00906B68), offset_to_top 0, 5 entries
class Camera
{
public:
    Camera(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EDBC slot 0x00 | virtual slot, introduced by sead::Camera
    virtual void vf_0x04(); // 0x0074ED70 slot 0x04 | virtual slot, introduced by sead::Camera
    virtual void vf_0x08(); // 0x0055D5A0 slot 0x08 | virtual slot, introduced by sead::Camera
    virtual void vf_0x0C(); // 0x0055D59C slot 0x0C | virtual slot, introduced by sead::Camera
    virtual void doUpdateMatrix(sead::Matrix34<float>*) const; // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
