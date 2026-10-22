#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx12AmbientLightE @ 0x008D058C
// vtable 0x009023D0 (vptr 0x009023D8), offset_to_top 0, 13 entries
// The ambient light of a scene (member names are ours).
class AmbientLight : public ::nw::gfx::Light
{
public:
    class DynamicBuilder;
    // (the same fields as the one of TransformNode; name from the reference symbols)
    struct Description : public TransformNode::Description
    {
    };

    // (inline) a resource of its own is freed with the light
    AmbientLight(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description, res::ResAmbientLightData* pOwnResource,
          os::IAllocator* pResourceAllocator)
        : Light(pAllocator, resNode, description), m_pOwnResource(pOwnResource), m_pResourceAllocator(pResourceAllocator)
    {
    }
    virtual ~AmbientLight();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00737890 slot 0x08 | nintendogs:bytes
    virtual void Accept(ISceneVisitor* pVisitor); // 0x0048DFD4 slot 0x10 | nintendogs:bytes
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x0048DB08 slot 0x24 | nintendogs:bytes
    virtual u32 GetLightType() const { return LIGHT_TYPE_AMBIENT; } // 0x00737888 slot 0x2C
    virtual u32 GetLightKind() const { return 3; } // 0x00737880 slot 0x30

    // a copy of the resource for the original values
    u32 CreateOriginalValue(os::IAllocator* pAllocator); // 0x0048DC34 | nintendogs:bytes [tier A]
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResAmbientLight resLight, Description description); // 0x0048DEDC | nintendogs:bytes [tier A]
    // a resource for a light without a file (the colors 0)
    static res::ResAmbientLightData* CreateResAmbientLight(os::IAllocator* pAllocator, const char* name); // 0x0048DD1C | nintendogs:bytes [tier A]
    // (symbols.json calls it a constructor)
    static AmbientLight* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator); // 0x0048E028 (name is ours)

    static const TypeInfo s_TypeInfo;

    res::ResAmbientLightData* m_pOwnResource;      // 0x174 (DynamicBuilder)
    os::IAllocator* m_pResourceAllocator;  // 0x178
};
ASSERT_SIZE(AmbientLight, 0x17C);
} // namespace gfx
} // namespace nw
