#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSklVis.h"
#include "g3d/dMaterialAnim.h"

// ResourceGetSklVis with the first material animation of the file as well (names are ours).
// RTTI 20ResourceGetSklVisMat @ 0x008CCBF4
// vtable 0x008F59E8 (vptr 0x008F59F0), offset_to_top 0, 9 entries
// vtable 0x008F5A3C (vptr 0x008F5A44), offset_to_top -104, 11 entries
class ResourceGetSklVisMat : public ::ResourceGetSklVis
{
public:
    ResourceGetSklVisMat(); // 0x00321C24
    virtual ~ResourceGetSklVisMat(); // 0x00321D18 slot 0x00
    // 0x00321CA4 (deleting dtor)
    // (the material animation stops at its first frame)
    bool Create(const char* path) override; // 0x003215E4 slot 0x08
    void Destroy() override; // 0x00321868 slot 0x0C

    // the skeletal and the material animation of the kind / with the speed of both
    void SetMaterialAnim(u32 kind); // 0x00321928
    void SetMaterialAnim(u32 kind, f32 speed); // 0x003218D8
    // the skeletal, the material and the visibility animation of the kind / with the speed of all three
    void SetAllAnims(u32 kind); // 0x00321A8C
    void SetAllAnims(u32 kind, f32 speed); // 0x00321A28

    g3d::MaterialAnim m_MaterialAnim; // 0x48
};
