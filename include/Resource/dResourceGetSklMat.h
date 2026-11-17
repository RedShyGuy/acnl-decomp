#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSkeletal.h"
#include "g3d/dMaterialAnim.h"

// ResourceGetSkeletal with the first material animation of the file (texture animations) next to the skeletal one;
// SetAnim changes both (names are ours).
// RTTI 17ResourceGetSklMat @ 0x008CC6AC
// vtable 0x008437E8 (vptr 0x008437F0), offset_to_top 0, 9 entries
// vtable 0x008F3C00 (vptr 0x008F3C08), offset_to_top 0, 9 entries
// vtable 0x008F3C54 (vptr 0x008F3C5C), offset_to_top -80, 11 entries
// vtable 0x008438C8 (vptr 0x008438D0), offset_to_top -780, 11 entries
class ResourceGetSklMat : public ::ResourceGetSkeletal
{
public:
    ResourceGetSklMat(); // 0x002D12C4
    virtual ~ResourceGetSklMat(); // 0x002D1380 slot 0x00
    // 0x002D1328 (deleting dtor)
    // (the material animation stops at its first frame)
    bool Create(const char* path) override; // 0x002D0F90 slot 0x08
    void Destroy() override; // 0x002D1174 slot 0x0C

    // the skeletal and the material animation of the kind (nothing if either is missing)
    void SetAnim(u32 kind); // 0x002D11C4

    g3d::MaterialAnim m_MaterialAnim; // 0x30
};
