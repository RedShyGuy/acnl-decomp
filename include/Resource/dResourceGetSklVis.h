#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSkeletal.h"
#include "g3d/dVisibilityAnim.h"

// ResourceGetSkeletal with the first visibility animation of the file next to the skeletal one; SetAnim changes
// both (names are ours).
// RTTI 17ResourceGetSklVis @ 0x008CC6B8
// vtable 0x008824E0 (vptr 0x008824E8), offset_to_top 0, 9 entries
// vtable 0x008F3C9C (vptr 0x008F3CA4), offset_to_top 0, 9 entries
// vtable 0x008F3CF0 (vptr 0x008F3CF8), offset_to_top -72, 11 entries
// vtable 0x008825C0 (vptr 0x008825C8), offset_to_top -104, 11 entries
class ResourceGetSklVis : public ::ResourceGetSkeletal
{
public:
    ResourceGetSklVis(); // 0x002D17A8
    virtual ~ResourceGetSklVis(); // 0x002D1864 slot 0x00
    // 0x002D180C (deleting dtor)
    bool Create(const char* path) override; // 0x002D13E4 slot 0x08
    void Destroy() override; // 0x002D15D0 slot 0x0C

    // the skeletal and the visibility animation of the kind (nothing if either is missing or without a file)
    void SetAnim(u32 kind); // 0x002D1670
    // SetAnim and the speed of both animations
    void SetAnim(u32 kind, f32 speed); // 0x002D1620
    void SetVisibilityAnimFrame(f32 frame); // 0x002D1778
    void SetVisibilityAnimSpeed(f32 speed); // 0x002D1790

    g3d::VisibilityAnim m_VisibilityAnim; // 0x30
};
