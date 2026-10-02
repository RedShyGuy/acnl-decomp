#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx5ModelE @ 0x008D07E4
// vtable 0x00902A1C (vptr 0x00902A24), offset_to_top 0, 11 entries
class Model : public ::nw::gfx::TransformNode
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Model(); // ctor candidate(s) 0x004AC590 (unverified)
    virtual ~Model(); // 0x004AC958 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004AC94C slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x0073C82C slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x004AC53C slot 0x10 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x004AAD24 slot 0x24 | nintendogs:bytes
    void IsMeshVisible(nw::gfx::res::ResMesh); // 0x004AAEEC | nintendogs:bytes [tier B]
    void CreateCallbacks(nw::os::IAllocator*); // 0x004AAF70 | nintendogs:callgraph [tier A]
    void CreateMaterials(nw::os::IAllocator*); // 0x004AB1CC | nintendogs:bytes [tier A]
    void CreateResMeshes(nw::os::IAllocator*); // 0x004AB3DC | nintendogs:callgraph [tier A]
    void BindMaterialAnim(nw::gfx::AnimGroup*); // 0x004AB6E0 | nintendogs:bytes-fuzzy [tier A]
    void CreateAnimGroups(nw::os::IAllocator*); // 0x004AB964 | nintendogs:bytes [tier A]
    void DestroyResMeshes(nw::os::IAllocator*, nw::ut::internal::ResArray<nw::gfx::res::ResMesh, nw::ut::internal::ResArrayClassTraits>); // 0x004ABB04 | nintendogs:bytes [tier A]
    void BindVisibilityAnim(nw::gfx::AnimGroup*); // 0x004ABB4C | nintendogs:bytes [tier A]
    void GetAnimTargetObject(const nw::anim::res::ResAnimGroupMember&); // 0x004ABDB0 | nintendogs:bytes [tier A]
    void GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::gfx::res::ResModel, nw::gfx::Model::Description); // 0x004ABED8 | nintendogs:callseq [tier A]
    void CreateResMeshNodeVisibilities(nw::os::IAllocator*); // 0x004AC1D8 | nintendogs:callgraph [tier A]
    void Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::Model::Description&, nw::os::IAllocator*); // 0x004AC590 | nintendogs:callseq [tier A]
};
} // namespace gfx
} // namespace nw
