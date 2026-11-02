#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dDummyLayout.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "ssys/ma/dAllocator.h"
#include "nw/lyt/lyt_AnimResource.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x00569FB0 (name is ours)
void ssys::ma::lyt::Animation::Finalize()
{
    if (m_pAnimTransform == NULL) {
        return;
    }
    nw::lyt::Layout::DeleteObj(m_pAnimTransform);
    m_pAnimTransform = NULL;
    delete[] m_pEntries;
    m_pEntries = NULL;
}

// 0x0056A02C | libgarden [tier A]
void ssys::ma::lyt::Animation::IncreaseFrame()
{
    m_PrevFrame = m_Frame;
    f32 frame = m_Frame;
    f32 increase = m_FrameIncrease;
    if (m_Loop & LOOP_FLAG_REVERSE) {
        if (increase > frame) {
            if (m_Loop & LOOP_FLAG_ONCE) {
                frame = 0.0f;
            } else {
                frame = (m_FrameCount - increase) + frame;
            }
        } else {
            frame -= increase;
        }
    } else {
        frame += increase;
        if (m_Loop & LOOP_FLAG_ONCE) {
            f32 last = m_FrameCount - 1.0f;
            if (last < frame) {
                frame = last;
            }
        } else if (m_FrameCount <= frame) {
            frame -= m_FrameCount;
        }
    }
    m_Frame = frame;
    if (m_pAnimTransform != NULL) {
        m_pAnimTransform->SetFrame(frame);
    }
}

// 0x0056A0CC | libgarden [tier A]
bool ssys::ma::lyt::Animation::Initialize(const char* name, const ResAccInterface* resAcc)
{
    if (m_pAnimTransform != NULL) {
        nw::lyt::Layout::DeleteObj(m_pAnimTransform);
        m_pAnimTransform = NULL;
        delete[] m_pEntries;
        m_pEntries = NULL;
    }
    // the layout that makes the transforms
    // 0x00AE67B4 (guard 0x00976C3C)
    static DummyLayout s_DummyLayout;
    const void* data = resAcc->GetResource('anim', name);
    if (data == NULL) {
        data = resAcc->GetResource(0, name);
    }
    nw::lyt::AnimTransform* animTrans = s_DummyLayout.DummyLayout::CreateAnimTransform(data, resAcc->m_pResourceAccessor);
    if (animTrans == NULL) {
        return false;
    }
    nw::lyt::AnimResource animRes;
    animRes.Set(data);
    m_IsDescendingBind = animRes.IsDescendingBind();
    u16 count = animTrans->GetAnimResource()->animContNum;
    m_EntryCount = count;
    m_pEntries = new (GetAllocator()->m_pHeap, 4) nw::lyt::AnimationLink[count];
    SetAnimTransform(animTrans);
    return true;
}

// 0x0056A374 slot 0x00 | libgarden
// 0x0056A320 (deleting dtor)
ssys::ma::lyt::Animation::~Animation()
{
    Finalize();
}

} // namespace lyt
} // namespace ma
} // namespace ssys
