#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimResult.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
class AnimGroup;

// RTTI N2nw3gfx10AnimObjectE @ 0x008D0550
// Something that gives the values of the members of an animation group: an evaluator of one
// animation or a blender of several animation objects. The function names are from the reference
// symbols, the rest is ours.
class AnimObject : public GfxObject
{
public:
    // the kind of object (only these values are seen; names are ours)
    enum AnimType
    {
        ANIM_TYPE_EVALUATOR = 0,           // AnimEvaluator
        ANIM_TYPE_TRANSFORM_EVALUATOR = 1, // TransformAnimEvaluator
        ANIM_TYPE_OVERRIDER = 2,
    };

    // (inline)
    AnimObject(os::IAllocator* pAllocator, int animType) : GfxObject(pAllocator), m_pAnimGroup(nullptr), m_AnimType(animType) {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // slot 0x08
    // binds the object to the members of the group (RESULT_SUCCESS: all bound)
    virtual u32 TryBind(AnimGroup* pAnimGroup) = 0; // slot 0x0C
    // unbinds the object
    virtual void Release() = 0; // slot 0x10
    virtual void UpdateFrame() = 0; // slot 0x14
    // the value of a member of the group, written into pTarget (an AnimResult if the member has a
    // blend operation, else the member itself); NULL: no value
    virtual const anim::AnimResult* GetResult(void* pTarget, int memberIdx) const = 0; // slot 0x18
    virtual bool HasMemberAnim(int memberIdx) const = 0; // slot 0x1C
    virtual void UpdateCache() = 0; // slot 0x20

    AnimGroup* GetAnimGroup() const { return m_pAnimGroup; }

    static const TypeInfo s_TypeInfo;

    AnimGroup* m_pAnimGroup; // 0x8 the bound group
    int m_AnimType;          // 0xC AnimType
};
ASSERT_SIZE(AnimObject, 0x10);

// (inline) the object as a T if it is one or derives from one (NULL: not; name is ours)
template <typename T>
T* DynamicCast(AnimObject* pAnimObject)
{
    const TypeInfo* pTypeInfo = pAnimObject->GetRuntimeTypeInfo();
    do {
        if (pTypeInfo == &T::s_TypeInfo) {
            return static_cast<T*>(pAnimObject);
        }
        pTypeInfo = pTypeInfo->pParent;
    } while (pTypeInfo != nullptr);
    return nullptr;
}
} // namespace gfx
} // namespace nw
