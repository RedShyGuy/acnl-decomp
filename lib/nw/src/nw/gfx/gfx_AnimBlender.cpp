#include "nw/gfx/gfx_AnimBlender.h"

namespace nw {
namespace gfx {
// 0x00976048
const TypeInfo nw::gfx::AnimBlender::s_TypeInfo(&AnimObject::s_TypeInfo);

// sets s_TypeInfo
// 0x0079A23C (static initializer)

// 0x0048C498 slot 0x20 | mk7dlp:bytes
void nw::gfx::AnimBlender::UpdateCache()
{
    for (int i = 0; i < m_AnimObjects.size(); ++i) {
        AnimObject* pAnimObject = m_AnimObjects[i];
        if (pAnimObject != nullptr) {
            pAnimObject->UpdateCache();
        }
    }
}

// 0x0048C4F0 slot 0x14 | mk7dlp:bytes
void nw::gfx::AnimBlender::UpdateFrame()
{
    for (int i = 0; i < m_AnimObjects.size(); ++i) {
        AnimObject* pAnimObject = m_AnimObjects[i];
        if (pAnimObject != nullptr) {
            pAnimObject->UpdateFrame();
        }
    }
}

// 0x0048C548 slot 0x10 (name is ours)
void nw::gfx::AnimBlender::Release()
{
    m_pAnimGroup = nullptr;
}

// 0x0048C554 slot 0x0C | slot vf_0x0C of nw::gfx::AnimBlender
u32 nw::gfx::AnimBlender::TryBind(AnimGroup* pAnimGroup)
{
    m_pAnimGroup = pAnimGroup;
    return RESULT_SUCCESS;
}

// 0x0048C59C slot 0x00 | nintendogs:bytes
// 0x0048C560 (deleting dtor)
nw::gfx::AnimBlender::~AnimBlender()
{
    // empty (in the original too: only the array)
}

} // namespace gfx
} // namespace nw
