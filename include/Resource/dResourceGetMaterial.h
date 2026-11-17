#pragma once

#include "decomp.h"
#include "Object/dObjectResource.h"
#include "g3d/dMaterialAnim.h"
#include "g3d/dSkeletalModel.h"

// The resource of an object with the first model of its file and the first material animation (from the texture
// animations of the file; names are ours).
// RTTI 19ResourceGetMaterial @ 0x008CCA44
// vtable 0x008F5218 (vptr 0x008F5220), offset_to_top 0, 7 entries
// vtable 0x008F5264 (vptr 0x008F526C), offset_to_top -52, 11 entries
class ResourceGetMaterial : public virtual ::ObjectResource
{
public:
    ResourceGetMaterial(); // 0x002FCF7C
    virtual ~ResourceGetMaterial(); // 0x002FD058 slot 0x00
    // 0x002FD01C (deleting dtor)
    bool Create(const char* path) override; // 0x002FCD80 slot 0x08
    void Destroy() override; // 0x002FCF08 slot 0x0C
    void Update(const nn::math::MTX34& matrix) override; // 0x004EE384 slot 0x10
    // empty
    void UpdateFrames() override; // 0x002FCD6C slot 0x14
    void AddToScene() override; // 0x004EE9E0 slot 0x18

    g3d::SkeletalModel m_Model;        // 0x04
    g3d::MaterialAnim m_MaterialAnim;  // 0x10
    u8 m_PlayMode;                     // 0x30 g3d::FrameController (1: loop)
};
