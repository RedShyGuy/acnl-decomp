#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnimGroupMember.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/res/gfx_ResFragmentLightingTable.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/os/os_MemorySizeCalculator.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class Model;

// RTTI N2nw3gfx8MaterialE @ 0x008D0808
// vtable 0x00902A98 (vptr 0x00902AA0), offset_to_top 0, 3 entries
// The material of a model at runtime: its resource, and copies of it ("buffers") for the parts the
// model changes at runtime (Model::Description::bufferOption). For each part the material keeps the
// resource it is read from: the buffer if the part is buffered, else the original. Member names are
// ours.
class Material : public SceneObject
{
public:
    // the parts of a material that can be buffered (bits of the buffer option; names are ours)
    enum BufferFlag : u16
    {
        BUFFER_USER_UNIFORMS = 1 << 0,
        BUFFER_1 = 1 << 1,
        BUFFER_MATERIAL_COLOR = 1 << 2,
        BUFFER_3 = 1 << 3,
        BUFFER_TEXTURE_COORDINATORS = 1 << 4,
        BUFFER_TEXTURE_MAPPERS = 1 << 5, // with the texture samplers
        BUFFER_PROCEDURAL_TEXTURE_MAPPER = 1 << 6,
        BUFFER_7 = 1 << 7,               // bits 7-10: a copy of the fragment shader
        BUFFER_FRAGMENT_LIGHTING_TABLE = 1 << 8,
        BUFFER_9 = 1 << 9,
        BUFFER_10 = 1 << 10,
        BUFFER_FRAGMENT_OPERATION = 1 << 11,
        BUFFER_12 = 1 << 12,
        BUFFER_FRAGMENT_SHADER = BUFFER_7 | BUFFER_FRAGMENT_LIGHTING_TABLE | BUFFER_9 | BUFFER_10,
    };
    // the parts with a resource of their own (the order of m_pResources)
    static const int RESOURCE_COUNT = 12;

    // a copied resource and whether the copy worked (bit 31 of the result: no memory)
    template <typename TResource>
    struct CopyResult
    {
        TResource resource; // 0x0
        u32 result;         // 0x4
    };

    Material(os::IAllocator* pAllocator, res::ResMaterial resource, int bufferCount, Model* pOwner); // 0x004B04EC | nintendogs (C)
    virtual ~Material(); // 0x004B0658 slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C918 slot 0x08

    // a material with bufferCount buffers (0 or 1) for the model (NULL: no memory)
    static Material* Create(res::ResMaterial resource, int bufferCount, Model* pOwner, os::IAllocator* pAllocator); // 0x004B0388 | nintendogs:bytes [tier A]
    // the memory Create allocates
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResMaterial resource, int bufferCount, u32 bufferOption); // 0x004AF818 | nintendogs:bytes-fuzzy [tier A]
    // whether the parts a material animation member of the type changes are buffered
    bool CanUseBuffer(u32 memberType) const; // 0x0073C888 | nintendogs:bytes-fuzzy [tier A]
    // the resource a part is read from (the buffer if the part is buffered; name is ours)
    DECOMP_NOINLINE res::ResMaterial GetResMaterial(BufferFlag flag) const; // 0x00525404 (name is ours)
    // the value a material animation member changes in the resource; the member remembers the
    // resource (NULL: an unknown type; name is ours)
    void* GetAnimTargetObject(const anim::res::ResAnimGroupMember& member, res::ResMaterial resource) const; // 0x004AF74C (name is ours)

    res::ResMaterial GetOriginal() const { return res::ResMaterial(GetResSceneObject().ptr()); }
    // (inline) the first buffer (NULL: none)
    res::ResMaterial GetBuffer() const { return res::ResMaterial(m_BufferCount > 0 ? m_Buffers[0] : nullptr); }
    // (inline) the resource an animation of the member type changes
    res::ResMaterial GetResMaterialForAnim(u32 memberType) const
    {
        if (CanUseBuffer(memberType)) {
            return GetBuffer();
        }
        return GetOriginal();
    }
    Model* GetOwner() const { return m_pOwner; }

    static const TypeInfo s_TypeInfo;

protected:
    // the buffers copied from the resource
    u32 CreateBuffers(os::IAllocator* pAllocator); // 0x004AEECC | nintendogs:callgraph [tier A]
    // a copy of the resource with the parts of the buffer option copied as well
    CopyResult<res::ResMaterial> CopyResMaterial(os::IAllocator* pAllocator, u32 bufferOption); // 0x004AF064 | nintendogs:callseq [tier A]
    // a copy of CopyResMaterial freed
    static void DestroyResMaterial(os::IAllocator* pAllocator, res::ResMaterial resource); // 0x004AF5C4 | nintendogs:bytes [tier A]
    // a copied fragment shader freed (with its lighting tables)
    static void DestroyResFragmentShader(os::IAllocator* pAllocator, res::ResFragmentShader shader); // 0x004AFC78 | nintendogs:callgraph [tier A]
    CopyResult<res::ResLightingLookupTable> CopyResLightingLookupTable(os::IAllocator* pAllocator, res::ResLightingLookupTable table); // 0x004AFF70 | nintendogs:bytes [tier A]
    CopyResult<res::ResFragmentLightingTable> CopyResFragmentLightingTable(os::IAllocator* pAllocator, res::ResFragmentLightingTable table); // 0x004B00B4 | nintendogs:bytes [tier A]

    ut::MoveArray<res::ResMaterialData*> m_Buffers;                // 0x0C
    Model* m_pOwner;                                               // 0x1C
    res::ResShaderProgramDescription m_ShaderProgramDescription;  // 0x20
    s32 m_BufferCount;                                             // 0x24
    // per part (the flags without BUFFER_PROCEDURAL_TEXTURE_MAPPER, in order)
    res::ResMaterial m_Resources[RESOURCE_COUNT];                 // 0x28
};
ASSERT_SIZE(Material, 0x58);
} // namespace gfx
} // namespace nw
