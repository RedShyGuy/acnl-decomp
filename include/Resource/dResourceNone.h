#pragma once

#include "decomp.h"
#include "Object/dObjectResource.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"

// The resource of an object whose graphics file is set by its owner (m_pResourceLoader; nothing without one): the
// first model with the first skeletal animation (names are ours).
// RTTI 12ResourceNone @ 0x008CB7C0
// vtable 0x008EE73C (vptr 0x008EE744), offset_to_top 0, 7 entries
// vtable 0x008EE788 (vptr 0x008EE790), offset_to_top -48, 11 entries
class ResourceNone : public virtual ::ObjectResource
{
public:
    ResourceNone(); // 0x002071B8
    virtual ~ResourceNone(); // 0x00207234 slot 0x00
    // 0x002071F8 (deleting dtor)
    // (path is not used)
    bool Create(const char* path) override; // 0x00207048 slot 0x08
    void Destroy() override; // 0x00207184 slot 0x0C
    void Update(const nn::math::MTX34& matrix) override; // 0x00206FD4 slot 0x10
    void UpdateFrames() override; // 0x00207024 slot 0x14
    void AddToScene() override; // 0x0020700C slot 0x18

    g3d::Model* m_pModel;              // 0x04 m_Model once it is made
    g3d::SkeletalModel m_Model;        // 0x08
    g3d::SkeletalAnim m_SkeletalAnim;  // 0x14
    u32 m_AnimKind;                    // 0x2C ResourceGetSkeletal::AnimKind (not changed here)
};
