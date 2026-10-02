#pragma once

#include "decomp.h"
#include "sead/seadLookAtCamera.h"

namespace sead {
// RTTI N4sead11OrthoCameraE @ 0x008D1410
// vtable 0x00905024 (vptr 0x0090502C), offset_to_top 0, 5 entries
class OrthoCamera : public ::sead::LookAtCamera
{
public:
    OrthoCamera(); // ctor candidate(s) 0x005404FC, 0x005405D0 (unverified)
    virtual void vf_0x00(); // 0x0074A744 slot 0x00 | virtual slot, introduced by sead::Camera
    virtual void vf_0x04(); // 0x0074A6F8 slot 0x04 | virtual slot, introduced by sead::Camera
    virtual void vf_0x08(); // 0x00540678 slot 0x08 | virtual slot, introduced by sead::Camera
    virtual void vf_0x0C(); // 0x00540674 slot 0x0C | virtual slot, introduced by sead::Camera
};
} // namespace sead
