#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx11SceneObjectE @ 0x008D0580
// A gfx object made from a scene object resource (members are ours).
class SceneObject : public ::nw::gfx::GfxObject
{
public:
    // (inline)
    SceneObject(os::IAllocator* pAllocator, res::ResSceneObject resObject) : GfxObject(pAllocator), m_ResSceneObject(resObject) {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const = 0; // slot 0x08

    res::ResSceneObject GetResSceneObject() const { return m_ResSceneObject; }

    static const TypeInfo s_TypeInfo;

    res::ResSceneObject m_ResSceneObject; // 0x8
};
ASSERT_SIZE(SceneObject, 0xC);
// (inline) the object as a T if it is one or derives from one (NULL: not, or no object; name is ours)
template <typename T>
T* DynamicCast(SceneObject* pObject)
{
    if (pObject == nullptr) {
        return nullptr;
    }
    const TypeInfo* pTypeInfo = pObject->GetRuntimeTypeInfo();
    do {
        if (pTypeInfo == &T::s_TypeInfo) {
            return static_cast<T*>(pObject);
        }
        pTypeInfo = pTypeInfo->pParent;
    } while (pTypeInfo != nullptr);
    return nullptr;
}
} // namespace gfx
} // namespace nw
