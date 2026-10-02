#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/gfx_Model.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004AC590 (unverified)
nw::gfx::Model::Model()
{
}

// 0x004AC958 slot 0x00 | nintendogs:bytes
nw::gfx::Model::~Model()
{
}

// 0x004AC94C slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::Model::vf_0x04()
{
}

// 0x0073C82C slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::Model::GetRuntimeTypeInfo() const
{
}

// 0x004AC53C slot 0x10 | nintendogs:bytes
void nw::gfx::Model::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x004AAD24 slot 0x24 | nintendogs:bytes
void nw::gfx::Model::Initialize(nw::os::IAllocator*)
{
}

// 0x004AAEEC | nintendogs:bytes [tier B]
void nw::gfx::Model::IsMeshVisible(nw::gfx::res::ResMesh)
{
}

// 0x004AAF70 | nintendogs:callgraph [tier A]
void nw::gfx::Model::CreateCallbacks(nw::os::IAllocator*)
{
}

// 0x004AB1CC | nintendogs:bytes [tier A]
void nw::gfx::Model::CreateMaterials(nw::os::IAllocator*)
{
}

// 0x004AB3DC | nintendogs:callgraph [tier A]
void nw::gfx::Model::CreateResMeshes(nw::os::IAllocator*)
{
}

// 0x004AB6E0 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::Model::BindMaterialAnim(nw::gfx::AnimGroup*)
{
}

// 0x004AB964 | nintendogs:bytes [tier A]
void nw::gfx::Model::CreateAnimGroups(nw::os::IAllocator*)
{
}

// 0x004ABB04 | nintendogs:bytes [tier A]
void nw::gfx::Model::DestroyResMeshes(nw::os::IAllocator*, nw::ut::internal::ResArray<nw::gfx::res::ResMesh, nw::ut::internal::ResArrayClassTraits>)
{
}

// 0x004ABB4C | nintendogs:bytes [tier A]
void nw::gfx::Model::BindVisibilityAnim(nw::gfx::AnimGroup*)
{
}

// 0x004ABDB0 | nintendogs:bytes [tier A]
void nw::gfx::Model::GetAnimTargetObject(const nw::anim::res::ResAnimGroupMember&)
{
}

// 0x004ABED8 | nintendogs:callseq [tier A]
void nw::gfx::Model::GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::gfx::res::ResModel, nw::gfx::Model::Description)
{
}

// 0x004AC1D8 | nintendogs:callgraph [tier A]
void nw::gfx::Model::CreateResMeshNodeVisibilities(nw::os::IAllocator*)
{
}

// 0x004AC590 | nintendogs:callseq [tier A]
void nw::gfx::Model::Create(nw::gfx::SceneNode*, nw::gfx::res::ResSceneObject, const nw::gfx::Model::Description&, nw::os::IAllocator*)
{
}

} // namespace gfx
} // namespace nw
