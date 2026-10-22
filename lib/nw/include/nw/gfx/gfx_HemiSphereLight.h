#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx15HemiSphereLightE @ 0x008D064C
// vtable 0x00902618 (vptr 0x00902620), offset_to_top 0, 13 entries
// The hemisphere light of a scene: a sky and a ground color (member names are ours).
class HemiSphereLight : public ::nw::gfx::Light
{
public:
    class DynamicBuilder;
    // (the same fields as the one of TransformNode; name from the reference symbols)
    struct Description : public TransformNode::Description
    {
    };

    // (inline) a resource of its own is freed with the light
    HemiSphereLight(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description, res::ResHemiSphereLightData* pOwnResource,
          os::IAllocator* pResourceAllocator)
        : Light(pAllocator, resNode, description), m_pOwnResource(pOwnResource), m_pResourceAllocator(pResourceAllocator)
    {
    }
    virtual ~HemiSphereLight();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738BC0 slot 0x08 | nintendogs:bytes
    virtual void Accept(ISceneVisitor* pVisitor); // 0x0049AE60 slot 0x10 | nintendogs:bytes
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x0049A98C slot 0x24 | nintendogs:bytes
    virtual u32 GetLightType() const { return LIGHT_TYPE_HEMISPHERE; } // 0x00738BB8 slot 0x2C
    virtual u32 GetLightKind() const { return 3; } // 0x00738BB0 slot 0x30

    // a copy of the resource for the original values
    u32 CreateOriginalValue(os::IAllocator* pAllocator); // 0x0049AAB8 | nintendogs:bytes [tier A]
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResHemiSphereLight resLight, Description description); // 0x0049ABA8 | nintendogs:bytes [tier A]
    // a resource for a light without a file (the colors 0)
    static res::ResHemiSphereLightData* CreateResHemiSphereLight(os::IAllocator* pAllocator, const char* name); // 0x0049ACA0 | nintendogs:bytes [tier A]
    // (symbols.json calls it a constructor)
    static HemiSphereLight* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator); // 0x0049AEB4 (name is ours)

    static const TypeInfo s_TypeInfo;

    res::ResHemiSphereLightData* m_pOwnResource;      // 0x174 (DynamicBuilder)
    os::IAllocator* m_pResourceAllocator;  // 0x178
};
ASSERT_SIZE(HemiSphereLight, 0x17C);
} // namespace gfx
} // namespace nw
