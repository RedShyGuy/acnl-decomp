#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13FragmentLightE @ 0x008D05D4
// vtable 0x009024C0 (vptr 0x009024C8), offset_to_top 0, 13 entries
// A light of the fragment lighting: with its direction in world space (member names are ours).
class FragmentLight : public ::nw::gfx::Light
{
public:
    class DynamicBuilder;
    // (the same fields as the one of TransformNode; name from the reference symbols)
    struct Description : public TransformNode::Description
    {
    };

    // (inline) a resource of its own is freed with the light
    FragmentLight(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description,
                  res::ResFragmentLightData* pOwnResource, os::IAllocator* pResourceAllocator)
        : Light(pAllocator, resNode, description), m_pOwnResource(pOwnResource), m_pResourceAllocator(pResourceAllocator),
          m_Direction(static_cast<const res::ResFragmentLightData*>(resNode.ptr())->direction)
    {
    }
    virtual ~FragmentLight();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738A18 slot 0x08 | nintendogs:bytes
    virtual void Accept(ISceneVisitor* pVisitor); // 0x0049324C slot 0x10 | nintendogs:bytes
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x00492A84 slot 0x24 | nintendogs:callseq
    // the direction in world space (turned with the parent if the resource says so)
    virtual void UpdateDirection(); // 0x00492BA0 slot 0x28 | nintendogs:bytes
    virtual u32 GetLightType() const { return LIGHT_TYPE_FRAGMENT; } // 0x00738A10 slot 0x2C
    virtual u32 GetLightKind() const // 0x00738A04 slot 0x30
    {
        return static_cast<const res::ResFragmentLightData*>(GetResSceneObject().ptr())->lightKind;
    }

    // a copy of the resource for the original values
    u32 CreateOriginalValue(os::IAllocator* pAllocator); // 0x00492C78 | nintendogs:bytes [tier A]
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResFragmentLight resLight, Description description); // 0x00492E04 | nintendogs:bytes [tier A]
    // a resource for a light without a file (all colors 0; the lookup tables are references without a name)
    static res::ResFragmentLightData* CreateResFragmentLight(os::IAllocator* pAllocator, const char* name); // 0x00492EFC | nintendogs:bytes-fuzzy [tier A]
    static DECOMP_NOINLINE void DestroyResFragmentLight(os::IAllocator* pAllocator, res::ResFragmentLightData* pResource); // 0x0049319C | nintendogs:bytes [tier A]
    static FragmentLight* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                 os::IAllocator* pAllocator); // 0x004932A0 | nintendogs:bytes [tier A]

    static const TypeInfo s_TypeInfo;

    res::ResFragmentLightData* m_pOwnResource;  // 0x174 (DynamicBuilder)
    os::IAllocator* m_pResourceAllocator;       // 0x178
    nn::math::VEC3 m_Direction;                 // 0x17C in world space
};
ASSERT_SIZE(FragmentLight, 0x188);
} // namespace gfx
} // namespace nw
