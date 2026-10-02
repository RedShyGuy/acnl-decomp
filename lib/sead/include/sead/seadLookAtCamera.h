#pragma once

#include "decomp.h"
#include "sead/seadCamera.h"

namespace sead {
// RTTI N4sead12LookAtCameraE @ 0x008D15A0
// vtable 0x0090522C (vptr 0x00905234), offset_to_top 0, 5 entries
class LookAtCamera : public ::sead::Camera
{
public:
    LookAtCamera(); // ctor candidate(s) 0x0054EBB0 (unverified)
    virtual void vf_0x00(); // 0x0074AB68 slot 0x00 | virtual slot, introduced by sead::Camera
    virtual void vf_0x04(); // 0x0074AB1C slot 0x04 | virtual slot, introduced by sead::Camera
    virtual void vf_0x08(); // 0x00540F04 slot 0x08 | virtual slot, introduced by sead::Camera
    virtual void vf_0x0C(); // 0x00540F00 slot 0x0C | virtual slot, introduced by sead::Camera
    virtual void doUpdateMatrix(sead::Matrix34<float>*) const; // 0x0074A9DC slot 0x10 | mk7dlp:bytes
};
} // namespace sead
