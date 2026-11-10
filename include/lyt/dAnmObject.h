#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_AnimTransform.h"
#include "nw/ut/internal/ut_LinkListImpl.h"
#include "ut/dFrameController.h"

namespace nw {
namespace lyt {
class Group;
class Pane;
} // namespace lyt
} // namespace nw

namespace lyt {
// An animation of a lyt::Object: an animation transform of the layout bound to the panes of a group or to a single
// pane, played with the frame controller (only the class name is from the RTTI; member and function names are ours).
// RTTI N3lyt9AnmObjectE @ 0x008D0FB4
// vtable 0x00904624 (vptr 0x0090462C), offset_to_top 0, 12 entries
class AnmObject : public ::ut::FrameController
{
public:
    // the animation of the panes of a group (bound here)
    AnmObject(nw::lyt::AnimTransform* animTransform, nw::lyt::Group* group, bool isShared); // 0x0050CE88
    // the animation of a single pane, already bound to it
    AnmObject(nw::lyt::AnimTransform* animTransform, nw::lyt::Pane* pane, void* unknown, bool isShared); // 0x0050CD7C
    virtual ~AnmObject(); // 0x0050CF58 slot 0x00
    // 0x0050CF00 slot 0x04 (deleting dtor)

    bool Calc() override; // 0x0050CD04 slot 0x0C
    void Restart() override; // 0x0050CCD0 slot 0x10
    void Play() override; // 0x0050CD58 slot 0x14
    void Stop() override; // 0x0050CCB4 slot 0x18

    // the panes are animated once with the frame
    void ApplyFrame(f32 frame); // 0x0050CA1C
    // the panes are animated once with the start (isStart) or the end frame
    void ApplyLimitFrame(bool isStart); // 0x0050CBA8

private:
    // the animation is bound to the panes of the group, the frames are set up
    void Bind(); // 0x0050CAAC
    // the animation of the panes is enabled or disabled
    void SetAnimationEnable(bool enable); // 0x0050CC3C
    // the frame range and play mode after the animation (a loop or once), then Reset (inline)
    void SetupFrames();
    // the panes animate themselves (inline)
    void AnimatePanes();

public:
    nw::ut::LinkListNode m_Link;               // 0x20 in the list of the lyt::Object
    nw::lyt::AnimTransform* m_pAnimTransform;  // 0x28
    nw::lyt::Group* m_pGroup;                  // 0x2C NULL: the animation of m_pPane
    void* m_pUnknown;                          // 0x30 (NULL wherever it is built)
    nw::lyt::Pane* m_pPane;                    // 0x34
    bool m_IsShared;                           // 0x38 the animation transform is not owned (not deleted)
};
ASSERT_OFFSET(AnmObject, m_Link, 0x20);
ASSERT_OFFSET(AnmObject, m_pPane, 0x34);
ASSERT_SIZE(AnmObject, 0x3C);
} // namespace lyt
