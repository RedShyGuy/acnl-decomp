#pragma once

#include "decomp.h"
#include "g3d/dTransformNode.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"

namespace nw {
namespace gfx {
class AnimGroup;
class Model;
namespace res {
struct ResMeshData;
} // namespace res
} // namespace gfx
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace g3d {
class AnimPack;
class BaseAnim;

// a model (nw::gfx::Model) of the scene (names after libgarden except the functions of our own)
// RTTI N3g3d5ModelE @ 0x008D0D6C
// vtable 0x00903BC4 (vptr 0x00903BCC), offset_to_top 0, 4 entries
class Model : public ::g3d::TransformNode
{
public:
    // the most meshes of a model (the size of the arrays on the stack)
    static const s32 MAX_MESHES = 64;

    Model(); // 0x004F1B04 | libgarden
    virtual ~Model(); // 0x004F1B34 slot 0x00 | libgarden
    // 0x004F1B24 (deleting dtor)
    // puts the meshes back into the order of the resource and destroys the node
    void Finalize() override; // 0x004F18F8 slot 0x0C | libgarden

    // the node of a resource (a model or another transform node); false if it could not be made (names are ours)
    bool Create(nw::gfx::res::ResSceneObject resource, nw::os::IAllocator* allocator, nw::os::IAllocator* deviceAllocator,
                u32 bufferOption, bool isAnimationEnabled, s32 maxAnimObjectsPerGroup); // 0x004F1848
    // the node as a model (NULL: none)
    nw::gfx::Model* GetModel() const; // 0x004F19B0
    nw::gfx::AnimGroup* GetMaterialAnimGroup() const; // 0x004F16B0
    nw::gfx::AnimGroup* GetVisibilityAnimGroup() const; // 0x004F1774
    // the evaluator of an animation as an animation object of a group of the node (nothing for an index outside)
    void SetMaterialAnim(const BaseAnim* anim, s32 objectIdx); // 0x004F16BC
    void SetMaterialAnim(const AnimPack* animPack); // 0x004F1718
    void ClearMaterialAnim(s32 objectIdx); // 0x004F178C
    void SetVisibilityAnim(const BaseAnim* anim); // 0x004F17EC
    // UpdateFrame of all animation objects of the node
    void UpdateAnimFrames(); // 0x004F14DC
    // the animations of the node evaluated (both timings)
    void EvaluateAnims(); // 0x004F1468
    // UpdateFrame and evaluation
    void UpdateAnims(); // 0x004F191C
    // the meshes of the resource in the order of their render layers and priorities; RestoreMeshOrder sorts them back
    // by address (the order of the file)
    void SortMeshes(); // 0x004F19FC
    void RestoreMeshOrder(); // 0x004F1540
    // the meshes of the node (the copies of the model or those of the resource); their number
    s32 GetMeshes(nw::gfx::res::ResMeshData** meshes); // 0x004F1608

    u8 m_MaxAnimObjectsPerGroup; // 0x8
};
ASSERT_SIZE(Model, 0xC);
} // namespace g3d
