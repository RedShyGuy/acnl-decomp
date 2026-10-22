#pragma once

#include "decomp.h"
#include "nn/math/math_Transform.h"
#include "nw/gfx/gfx_TransformNode.h"

namespace nw {
namespace gfx {
class AnimGroup;

// RTTI N2nw3gfx5LightE @ 0x008D07D8
// vtable 0x009029E0 (vptr 0x009029E8), offset_to_top 0, 13 entries
// The base of the lights: a transform node whose resource animations can change, with a copy of the
// resource for the original values (member and virtual function names are ours).
class Light : public ::nw::gfx::TransformNode
{
public:
    // the kinds of lights (GetLightType)
    enum LightType
    {
        LIGHT_TYPE_FRAGMENT = 0,
        LIGHT_TYPE_VERTEX = 1,
        LIGHT_TYPE_HEMISPHERE = 2,
        LIGHT_TYPE_AMBIENT = 3,
    };

    // (inline)
    Light(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description)
        : TransformNode(pAllocator, resNode, description), m_pOriginalValue(nullptr), m_pAnimGroup(nullptr)
    {
    }
    virtual ~Light();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C820 slot 0x08 | nintendogs:bytes
    virtual void Accept(ISceneVisitor* pVisitor); // 0x004AA9D8 slot 0x10 | nintendogs:bytes
    virtual u32 GetLightType() const = 0; // slot 0x2C
    // the kind within the type (a fragment light: from its resource)
    virtual u32 GetLightKind() const = 0; // slot 0x30

    // the animation group of the resource, with the light, its resource and the copy as targets
    u32 CreateAnimGroup(os::IAllocator* pAllocator); // 0x004AA760 | nintendogs:bytes-fuzzy [tier A]
    // the copy of the resource freed
    void DestroyOriginalValue(); // 0x004AA9B0 | nintendogs:bytes [tier A]

    static const TypeInfo s_TypeInfo;

    void* m_pOriginalValue;                     // 0x148 the copy of the resource (the original values)
    nn::math::Transform3 m_OriginalTransform;   // 0x14C
    AnimGroup* m_pAnimGroup;                    // 0x170
};
ASSERT_SIZE(Light, 0x174);
} // namespace gfx
} // namespace nw
