#pragma once

#include "decomp.h"
#include "nn/math/math_MTX44.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResFog.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
class AnimGroup;
class Camera;

// RTTI N2nw3gfx3FogE @ 0x008D07CC
// vtable 0x009029AC (vptr 0x009029B4), offset_to_top 0, 11 entries
// A fog: Update fills its lookup table from the camera, when the camera or the updater changed
// (member names are ours).
class Fog : public ::nw::gfx::TransformNode
{
public:
    class DynamicBuilder;
    // (the same fields as the one of TransformNode; name from the reference symbols)
    struct Description : public TransformNode::Description
    {
    };

    // (inline) a resource of its own is freed with the fog
    Fog(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description, res::ResFogData* pOwnResource,
        os::IAllocator* pResourceAllocator)
        : TransformNode(pAllocator, resNode, description), m_pOwnResource(pOwnResource), m_pResourceAllocator(pResourceAllocator),
          m_pLastUpdater(nullptr), m_pLastUpdaterAllocator(nullptr), m_Near(0.0f), m_Far(0.0f), m_WScale(0.0f), m_pAnimGroup(nullptr),
          m_pOriginalValue(nullptr)
    {
    }
    virtual ~Fog();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C6AC slot 0x08 | nintendogs:bytes
    virtual void Accept(ISceneVisitor* pVisitor); // 0x004A5298 slot 0x10 | nintendogs:bytes
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x004A4788 slot 0x24 | nintendogs:bytes

    // a resource for a fog without a file (an image lookup table of 128 entries, all 0)
    static res::ResFogData* CreateResFog(os::IAllocator* pAllocator, const char* name); // 0x004A4880 | nintendogs:bytes-fuzzy [tier A]
    static DECOMP_NOINLINE void DestroyResFog(os::IAllocator* pAllocator, res::ResFogData* pResource); // 0x004A4B1C | nintendogs:bytes [tier A]
    // the animation group of the resource (only its fog members)
    u32 CreateAnimGroup(os::IAllocator* pAllocator); // 0x004A4CB0 | nintendogs:bytes [tier A]
    // the fog of 129 depths into the PICA commands of the lookup table (3dbrew "GPU/Internal Registers":
    // FOG_LUT_INDEX, FOG_LUT_DATA0)
    void SetupFogSampler(res::ResImageLookupTable lookupTable, res::ResFogUpdater updater, const nn::math::MTX44& inverseProjection); // 0x004A4E4C | nintendogs:callgraph [tier A]
    u32 CreateOriginalValue(os::IAllocator* pAllocator); // 0x004A50E4 | nintendogs:bytes [tier A]
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResFog resFog, Description description); // 0x004A5188 | nintendogs:bytes [tier A]
    // (symbols.json calls it a constructor)
    static Fog* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator); // 0x004A52EC (name is ours)
    void Update(const Camera* pCamera); // 0x004A550C | nintendogs:bytes [tier A]

    static const TypeInfo s_TypeInfo;

    res::ResFogData* m_pOwnResource;            // 0x148 (DynamicBuilder)
    os::IAllocator* m_pResourceAllocator;       // 0x14C
    res::ResFogUpdaterData* m_pLastUpdater;     // 0x150 the updater the table was made with
    os::IAllocator* m_pLastUpdaterAllocator;    // 0x154
    f32 m_Near;                                 // 0x158 the camera the table was made for
    f32 m_Far;                                  // 0x15C
    f32 m_WScale;                               // 0x160
    AnimGroup* m_pAnimGroup;                    // 0x164
    res::ResFogData* m_pOriginalValue;          // 0x168 the copy of the resource
};
ASSERT_SIZE(Fog, 0x16C);
} // namespace gfx
} // namespace nw
