#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ResourceFormat.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace lyt {
class Material;
class Pane;
class ResourceAccessor;

// RTTI N2nw3lyt13AnimTransformE @ 0x008D0840
//
// An animation of a layout: the resource, the current frame and the bindings to panes and
// materials (abstract; constructor and destructor are inline). The names of the virtual
// functions are from the reference symbols of AnimTransformBasic (Animate, Bind, SetResource);
// member names are ours.
class AnimTransform
{
public:
    // (inline)
    AnimTransform() : m_pRes(nullptr), m_Frame(0.0f) {}
    // (inline)
    virtual ~AnimTransform() {}

    // the animation content idx of the resource is applied to the pane / material
    virtual void Animate(u32 idx, Pane* pPane) = 0; // slot 0x08
    virtual void Animate(u32 idx, Material* pMaterial) = 0; // slot 0x0C
    // the resource (links for all contents / for animNum bindings)
    virtual void SetResource(const res::AnimationBlock* pRes, ResourceAccessor* pResAccessor) = 0; // slot 0x10
    virtual void SetResource(const res::AnimationBlock* pRes, ResourceAccessor* pResAccessor, u16 animNum) = 0; // slot 0x14
    // the contents are bound to the panes / the material of their names
    virtual void Bind(Pane* pPane, bool bRecursive, bool bDisable) = 0; // slot 0x18
    virtual void Bind(Material* pMaterial, bool bDisable) = 0; // slot 0x1C

    // (inline, copies out of line)
    bool IsLoopData() const; // 0x0073CC38 | nintendogs:bytes [tier A]
    u16 GetFrameSize() const; // 0x0073CC4C (name is ours)
    f32 GetFrame() const { return m_Frame; }
    void SetFrame(f32 frame) { m_Frame = frame; }
    const res::AnimationBlock* GetAnimResource() const { return m_pRes; }

    ut::LinkListNode m_Link;            // 0x04 in the animation list of the layout
    const res::AnimationBlock* m_pRes;  // 0x0C
    f32 m_Frame;                        // 0x10
};
ASSERT_SIZE(AnimTransform, 0x14);
} // namespace lyt
} // namespace nw
