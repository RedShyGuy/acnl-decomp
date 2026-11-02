#include "ssys/ma/lyt/dFrameCtrl.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x0056A25C | libgarden [tier A]
void ssys::ma::lyt::FrameCtrl::SetAnimTransform(nw::lyt::AnimTransform* anim)
{
    m_pAnimTransform = anim;
    if (anim == NULL) {
        return;
    }
    u8 loop = LOOP_FLAG_ONCE;
    if (anim->IsLoopData()) {
        loop = 0;
    }
    u16 frameSize = m_pAnimTransform->GetFrameSize();
    m_FrameIncrease = 1.0f;
    m_FrameCount = frameSize;
    m_Frame = 0.0f;
    m_Loop = loop;
    m_PrevFrame = 0.0f;
    if (m_pAnimTransform != NULL) {
        m_pAnimTransform->SetFrame(m_Frame);
    }
}

// 0x0056A2C8 slot 0x08 (name after libgarden)
void ssys::ma::lyt::FrameCtrl::SetFrame(f32 frame)
{
    m_PrevFrame = frame;
    m_Frame = frame;
    if (m_pAnimTransform != NULL) {
        m_pAnimTransform->SetFrame(frame);
    }
}

// 0x0056A2E0 (name is ours)
void ssys::ma::lyt::FrameCtrl::SetPlayParameters(f32 frameCount, f32 frameIncrease, f32 frame, u8 loop)
{
    m_FrameCount = frameCount;
    if (frame < 0.0f) {
        frame = 0.0f;
    }
    m_Frame = frame;
    m_FrameIncrease = frameIncrease;
    m_Loop = loop;
    m_PrevFrame = frame;
    if (m_pAnimTransform != NULL) {
        m_pAnimTransform->SetFrame(m_Frame);
    }
}

// 0x0056A62C (name is ours)
void ssys::ma::lyt::FrameCtrl::SetPlayParametersNoUpdate(f32 frameCount, f32 frameIncrease, f32 frame, u8 loop)
{
    m_FrameCount = frameCount;
    if (frame < 0.0f) {
        frame = 0.0f;
    }
    m_Frame = frame;
    m_FrameIncrease = frameIncrease;
    m_Loop = loop;
    m_PrevFrame = frame;
}

} // namespace lyt
} // namespace ma
} // namespace ssys
