#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_AnimTransform.h"
#include "nw/lyt/lyt_Types.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt18AnimTransformBasicE @ 0x008D0850
// vtable 0x00902B4C (vptr 0x00902B54), offset_to_top 0, 8 entries
//
// The animation of a BCLAN resource: Hermite curves for the SRT, colors and texture SRT, step
// curves for the visibility and the texture patterns (textures from the resource accessor).
class AnimTransformBasic : public ::nw::lyt::AnimTransform
{
public:
    AnimTransformBasic(); // 0x004B6AC0 | nintendogs:bytes [tier A]
    virtual ~AnimTransformBasic(); // 0x004B6B30 slot 0x00 | nintendogs:bytes-fuzzy
    // 0x004B6AF8 (deleting dtor)
    virtual void Animate(u32 idx, Pane* pPane); // 0x004B6460 slot 0x08 | nintendogs:bytes
    virtual void Animate(u32 idx, Material* pMaterial); // 0x004B673C slot 0x0C | nintendogs:bytes
    virtual void SetResource(const res::AnimationBlock* pRes, ResourceAccessor* pResAccessor); // 0x004B606C slot 0x10
    // (symbols.json calls it TextBox::AllocStringBuffer(unsigned short) after a wrong call
    // sequence; it is slot 0x14 of this vtable)
    virtual void SetResource(const res::AnimationBlock* pRes, ResourceAccessor* pResAccessor, u16 animNum); // 0x004B607C slot 0x14
    virtual void Bind(Pane* pPane, bool bRecursive, bool bDisable); // 0x004B61E4 slot 0x18 | nintendogs:bytes
    virtual void Bind(Material* pMaterial, bool bDisable); // 0x004B6374 slot 0x1C

    // (inline) a free link from pLink on (NULL: from the first); NULL if there is none (name is
    // ours)
    AnimationLink* FindUnbindLink(AnimationLink* pLink) const
    {
        if (pLink == nullptr) {
            pLink = m_pAnimLinks;
            if (pLink == nullptr) {
                return nullptr;
            }
        }
        for (; pLink < &m_pAnimLinks[m_AnimLinkNum]; ++pLink) {
            if (pLink->m_pAnimTransform == nullptr) {
                return pLink;
            }
        }
        return nullptr;
    }

    TextureInfo* m_pTextures;       // 0x14 of the texture patterns (fileNum)
    AnimationLink* m_pAnimLinks;    // 0x18
    u16 m_AnimLinkNum;              // 0x1C
};
ASSERT_SIZE(AnimTransformBasic, 0x20);
} // namespace lyt
} // namespace nw
