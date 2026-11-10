#include "lyt/dAnmObject.h"

#include "nw/lyt/lyt_Group.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Pane.h"

namespace lyt {
// (inline)
inline void lyt::AnmObject::SetupFrames()
{
    if (m_pAnimTransform->IsLoopData()) {
        m_PlayMode = PLAY_MODE_LOOP;
        m_EndFrame = m_pAnimTransform->GetFrameSize();
    } else {
        m_PlayMode = PLAY_MODE_ONCE;
        m_EndFrame = m_pAnimTransform->GetFrameSize() - 1.0f;
    }
    m_StartFrame = 0.0f;
    m_Speed = 1.0f;
    m_State = STATE_STOPPED;
    m_IsReverse = false;
    Reset();
}

// (inline)
inline void lyt::AnmObject::AnimatePanes()
{
    if (m_pGroup != NULL) {
        nw::lyt::Group::PaneLinkList& paneList = m_pGroup->GetPaneList();
        for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
            nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget->AnimateSelf(0);
        }
    } else {
        m_pPane->AnimateSelf(0);
    }
}

// 0x0050CA1C
void lyt::AnmObject::ApplyFrame(f32 frame)
{
    SetAnimationEnable(true);
    m_Frame = frame;
    m_pAnimTransform->SetFrame(frame);
    AnimatePanes();
    SetAnimationEnable(false);
}

// 0x0050CAAC
void lyt::AnmObject::Bind()
{
    nw::lyt::Group::PaneLinkList& paneList = m_pGroup->GetPaneList();
    for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget->BindAnimation(m_pAnimTransform, false, true);
    }
    SetupFrames();
}

// 0x0050CBA8
void lyt::AnmObject::ApplyLimitFrame(bool isStart)
{
    SetAnimationEnable(true);
    const f32 frame = isStart ? m_StartFrame : m_EndFrame;
    m_Frame = frame;
    m_pAnimTransform->SetFrame(frame);
    AnimatePanes();
    SetAnimationEnable(false);
}

// 0x0050CC3C
void lyt::AnmObject::SetAnimationEnable(bool enable)
{
    if (m_pGroup != NULL) {
        nw::lyt::Group::PaneLinkList& paneList = m_pGroup->GetPaneList();
        for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
            nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget->SetAnimationEnable(m_pAnimTransform, enable, false);
        }
    } else {
        m_pPane->SetAnimationEnable(m_pAnimTransform, enable, false);
    }
}

// 0x0050CCB4 slot 0x18
void lyt::AnmObject::Stop()
{
    SetAnimationEnable(false);
    m_State = STATE_STOPPING;
}

// 0x0050CCD0 slot 0x10
void lyt::AnmObject::Restart()
{
    SetAnimationEnable(true);
    Reset();
    Play();
}

// 0x0050CD04 slot 0x0C
bool lyt::AnmObject::Calc()
{
    bool isApplied = false;
    if (m_State == STATE_STOPPING) {
        SetAnimationEnable(false);
    }
    if (m_State == STATE_PLAYING || m_State == STATE_STOPPING) {
        m_pAnimTransform->SetFrame(m_Frame);
        isApplied = true;
    }
    Update();
    return isApplied;
}

// 0x0050CD58 slot 0x14
void lyt::AnmObject::Play()
{
    SetAnimationEnable(true);
    m_State = STATE_PLAYING;
    m_IsEnd = false;
}

// 0x0050CD7C
lyt::AnmObject::AnmObject(nw::lyt::AnimTransform* animTransform, nw::lyt::Pane* pane, void* unknown, bool isShared)
    : m_pAnimTransform(animTransform), m_pGroup(NULL), m_pUnknown(unknown), m_pPane(pane), m_IsShared(isShared)
{
    SetupFrames();
}

// 0x0050CE88
lyt::AnmObject::AnmObject(nw::lyt::AnimTransform* animTransform, nw::lyt::Group* group, bool isShared)
    : m_pAnimTransform(animTransform), m_pGroup(group), m_pUnknown(NULL), m_pPane(NULL), m_IsShared(isShared)
{
    Bind();
}

// 0x0050CF58 slot 0x00
// 0x0050CF00 (deleting dtor)
lyt::AnmObject::~AnmObject()
{
    if (m_pAnimTransform != NULL && !m_IsShared) {
        nw::lyt::Layout::DeleteObj(m_pAnimTransform);
        m_pAnimTransform = NULL;
    }
}

} // namespace lyt
