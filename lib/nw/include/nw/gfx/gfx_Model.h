#pragma once

#include "decomp.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX34.h"
#include "nw/anim/res/anim_ResAnimGroupMember.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_IMaterialActivator.h"
#include "nw/gfx/gfx_NodeCallback.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/os/os_MemorySizeCalculator.h"
#include "nw/ut/internal/ut_ResArray.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class AnimGroup;
class IMaterialActivator;
class ISceneVisitor;
class Material;

// RTTI N2nw3gfx5ModelE @ 0x008D07E4
// vtable 0x00902A1C (vptr 0x00902A24), offset_to_top 0, 11 entries
// A model: copies of its meshes, its materials (or those of another model), the visibilities of its
// mesh nodes, render callbacks and the animation groups of its materials and visibilities. Member
// names are ours.
class Model : public TransformNode
{
public:
    // the settings of a model (the names are ours)
    struct Description : public TransformNode::Description
    {
        u32 bufferOption;                       // 0x10 the parts of the materials that get buffers (Material::BufferFlag)
        Model* pSharedMaterialModel;            // 0x14 the model whose materials are used (NULL: its own)
        Model* pSharedMeshNodeVisibilityModel;  // 0x18 the model whose mesh node visibilities are used (NULL: its own)
    };

    // what the model takes from other models (m_SharingFlags)
    enum SharingFlag
    {
        SHARING_MATERIALS = 1 << 0,
        SHARING_MESH_NODE_VISIBILITIES = 1 << 1,
    };

    // the callbacks around the rendering of the model (the argument is not known; no code of the
    // binary invokes them)
    typedef NodeCallbackList<Model> RenderCallbackList;

    Model(os::IAllocator* pAllocator, res::ResModel resource, const Description& description); // (inline)
    virtual ~Model(); // 0x004AC958 slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C82C slot 0x08
    virtual void Accept(ISceneVisitor* pVisitor); // 0x004AC53C slot 0x10 | nintendogs:bytes
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x004AAD24 slot 0x24 | nintendogs:bytes

    // a model of the resource under the parent (NULL: no memory)
    static Model* Create(SceneNode* pParent, res::ResSceneObject resource, const Description& description, os::IAllocator* pAllocator); // 0x004AC590 | nintendogs:callseq [tier A]
    // the memory Create allocates for Initialize
    static void GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, res::ResModel resource, Description description); // 0x004ABED8 | nintendogs:callseq [tier A]

    // whether the mesh is drawn (its own visibility and the one of its mesh node)
    bool IsMeshVisible(res::ResMesh mesh); // 0x004AAEEC | nintendogs:bytes [tier B]
    // (inline in the original, called out of line) the view matrix for the normals: the view
    // matrix times the inverse transpose of the world matrix, else times the scale correction of
    // the world matrix (none for a world scale of one; skeletal models whose bones are not in model
    // space use the inverse transpose; name is ours)
    DECOMP_NOINLINE void UpdateNormalMatrix(const nn::math::MTX34& viewMatrix, bool useInverseTranspose) // 0x00135B9C (name is ours)
    {
        if (useInverseTranspose) {
            nn::math::MTX34 inverseTranspose;
            nn::math::ARMv6::MTX34InvTransposeAsm(&inverseTranspose, &m_WorldMatrix);
            nn::math::ARMv6::MTX34MultAsm(&m_NormalMatrix, &viewMatrix, &inverseTranspose);
        } else if (!m_WorldTransform.IsEnabledFlags(CalculatedTransform::FLAG_IS_SCALE_ONE)) {
            nn::math::MTX34 inverse;
            nn::math::ARMv6::MTX34InverseAsm(&inverse, &m_WorldMatrix);
            nn::math::MTX34 transpose;
            nn::math::ARMv6::MTX34TransposeAsm(&transpose, &inverse);
            nn::math::MTX34 correction;
            nn::math::ARMv6::MTX34MultAsm(&correction, &transpose, &inverse);
            nn::math::ARMv6::MTX34MultAsm(&m_NormalMatrix, &viewMatrix, &correction);
        } else {
            nn::math::ARMv6::MTX34CopyAsm(&m_NormalMatrix, &viewMatrix);
        }
    }

    // whether a model is drawn: visible and on a visible branch (the class name is from the
    // reference symbols)
    struct IsVisibleModelDefaultFunctor
    {
        bool operator()(const Model* pModel) const { return pModel->m_IsVisible && pModel->IsVisible(); }
    };
    // (name is ours) a test whether a model is drawn, given to SceneUpdater::SubmitView
    class IsVisibleModelFunctor
    {
    public:
        virtual bool operator()(const Model* pModel) const = 0;
    };

    // (inline) the copies of the meshes, else the meshes of the resource (the copy is for the game)
    DECOMP_EMIT_COPY ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits> GetResMeshes() const // 0x004AAEAC (name is ours)
    {
        if (m_ResMeshes.m_pBegin != m_ResMeshes.m_pEnd) {
            return m_ResMeshes;
        }
        const res::ResModelData* pResource = GetResModel().ptr();
        return ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits>(pResource->meshes.array.To<ut::Offset>(),
                                                                                       pResource->meshes.count);
    }

    res::ResModel GetResModel() const { return res::ResModel(GetResSceneObject().ptr()); }
    u32 GetBufferOption() const { return m_BufferOption; }
    Material* GetMaterial(int index) const { return m_Materials[index]; }
    IMaterialActivator* GetMaterialActivator() const { return m_MaterialActivator.Get(); }

    static const TypeInfo s_TypeInfo;

    u32 CreateCallbacks(os::IAllocator* pAllocator); // 0x004AAF70 | nintendogs:callgraph [tier A]
    u32 CreateMaterials(os::IAllocator* pAllocator); // 0x004AB1CC | nintendogs:bytes [tier A]
    u32 CreateResMeshes(os::IAllocator* pAllocator); // 0x004AB3DC | nintendogs:callgraph [tier A]
    // the members of a material animation group bound to the materials
    void BindMaterialAnim(AnimGroup* pAnimGroup); // 0x004AB6E0 | nintendogs:bytes-fuzzy [tier A]
    u32 CreateAnimGroups(os::IAllocator* pAllocator); // 0x004AB964 | nintendogs:bytes [tier A]
    // the copies of the meshes freed (one block)
    static void DestroyResMeshes(os::IAllocator* pAllocator, ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits> meshes); // 0x004ABB04 | nintendogs:bytes [tier A]
    // the members of a visibility animation group bound to the model, its meshes and mesh nodes
    void BindVisibilityAnim(AnimGroup* pAnimGroup); // 0x004ABB4C | nintendogs:bytes [tier A]
    // the object a visibility animation member changes
    void* GetAnimTargetObject(const anim::res::ResAnimGroupMember& member); // 0x004ABDB0 | nintendogs:bytes [tier A]
    u32 CreateResMeshNodeVisibilities(os::IAllocator* pAllocator); // 0x004AC1D8 | nintendogs:callgraph [tier A]

    // (inline) the mesh at index (of the resource if there are no copies)
    res::ResMesh GetResMesh(int index) const
    {
        if (m_ResMeshes.m_pBegin != m_ResMeshes.m_pEnd) {
            return m_ResMeshes[index];
        }
        return res::ResMesh(GetResModel().GetMesh(index));
    }
    // (inline) the mesh node visibility at index (of the resource if there are no copies)
    res::ResMeshNodeVisibilityData* GetMeshNodeVisibility(int index)
    {
        if (!m_MeshNodeVisibilities.empty()) {
            return &m_MeshNodeVisibilities[index];
        }
        return GetResModel().GetMeshNodeVisibility(index);
    }

    AnimGroup* m_pMaterialAnimGroup;                                // 0x148
    s32 m_MaterialAnimGroupIndex;                                   // 0x14C
    AnimGroup* m_pVisibilityAnimGroup;                              // 0x150
    s32 m_VisibilityAnimGroupIndex;                                 // 0x154
    RenderCallbackList* m_pPreRenderCallbacks;                      // 0x158
    RenderCallbackList* m_pPostRenderCallbacks;                     // 0x15C
    ut::MoveArray<Material*> m_Materials;                           // 0x160
    ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits> m_ResMeshes; // 0x170 the copies of the meshes
    ut::MoveArray<res::ResMeshNodeVisibilityData> m_MeshNodeVisibilities; // 0x178 copies (or those of the shared model)
    u32 m_BufferOption;                                             // 0x188
    nn::math::MTX34 m_Unknown0x18C;                                 // 0x18C (the identity)
    nn::math::MTX34 m_NormalMatrix;                                 // 0x1BC the view matrix for the normals (UpdateNormalMatrix)
    GfxPtr<IMaterialActivator> m_MaterialActivator;                // 0x1EC
    u8 m_LayerId;                                                   // 0x1F4 (the renderer sorts by it)
    bool m_IsResourceVisible;                                       // 0x1F5 the visibility of the resource
    bool m_IsVisible;                                               // 0x1F6 (a visibility animation changes it)
    u8 m_SharingFlags;                                              // 0x1F7 SharingFlag
    ut::MoveArray<bool> m_MeshVisibilities;                         // 0x1F8 per mesh copy
    ut::MoveArray<bool> m_MeshNodeVisibilityFlags;                  // 0x208 per mesh node
    const Description* m_pDescription;                              // 0x218 (only valid while Create runs)
    u32 m_Unknown0x21C;                                             // 0x21C
};
ASSERT_SIZE(Model, 0x220);

// (no copy in the original: inlined in Model::Create and the constructors of the derived models)
DECOMP_ALWAYS_INLINE nw::gfx::Model::Model(os::IAllocator* pAllocator, res::ResModel resource, const Description& description)
    : TransformNode(pAllocator, resource, description), m_pMaterialAnimGroup(nullptr), m_MaterialAnimGroupIndex(0),
      m_pVisibilityAnimGroup(nullptr), m_VisibilityAnimGroupIndex(0), m_pPreRenderCallbacks(nullptr),
      m_pPostRenderCallbacks(nullptr), m_Materials(), m_ResMeshes(nullptr, 0), m_MeshNodeVisibilities(),
      m_BufferOption(description.bufferOption), m_Unknown0x18C(nn::math::MTX34::Identity()),
      m_NormalMatrix(nn::math::MTX34::Identity()), m_MaterialActivator(), m_LayerId(0), m_IsResourceVisible(true),
      m_IsVisible(true),
      m_SharingFlags((description.pSharedMaterialModel != nullptr ? SHARING_MATERIALS : 0) |
                     (description.pSharedMeshNodeVisibilityModel != nullptr ? SHARING_MESH_NODE_VISIBILITIES : 0)),
      m_MeshVisibilities(), m_MeshNodeVisibilityFlags(), m_pDescription(&description), m_Unknown0x21C(0)
{
    m_IsVisible = resource.ptr()->isVisible;
}
} // namespace gfx
} // namespace nw
