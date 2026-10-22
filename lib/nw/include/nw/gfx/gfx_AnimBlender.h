#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_AnimObject.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx11AnimBlenderE @ 0x008D0568
// vtable 0x00902374 (vptr 0x0090237C), offset_to_top 0, 9 entries
// Combines the values of several animation objects (the derived class says how). The function names
// are from the reference symbols, the rest is ours.
class AnimBlender : public AnimObject
{
public:
    // (inline)
    AnimBlender(os::IAllocator* pAllocator, int animType) : AnimObject(pAllocator, animType) {}
    virtual ~AnimBlender();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00737868 slot 0x08
    virtual u32 TryBind(AnimGroup* pAnimGroup);
    virtual void Release();
    virtual void UpdateFrame();
    virtual bool HasMemberAnim(int memberIdx) const // 0x007377F8 slot 0x1C | nintendogs:bytes
    {
        for (int i = 0; i < m_AnimObjects.size(); ++i) {
            const AnimObject* pAnimObject = m_AnimObjects[i];
            if (pAnimObject != nullptr && pAnimObject->HasMemberAnim(memberIdx)) {
                return true;
            }
        }
        return false;
    }
    virtual void UpdateCache();

    static const TypeInfo s_TypeInfo;

    ut::MoveArray<AnimObject*> m_AnimObjects; // 0x10 (NULL entries are skipped)
};
ASSERT_SIZE(AnimBlender, 0x20);
} // namespace gfx
} // namespace nw
