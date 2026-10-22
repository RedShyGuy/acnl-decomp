#include "nw/lyt/lyt_AnimTransform.h"

namespace nw {
namespace lyt {
// 0x0073CC38 | nintendogs:bytes [tier A]
bool nw::lyt::AnimTransform::IsLoopData() const
{
    return m_pRes->loop != 0;
}

// 0x0073CC4C (name is ours)
u16 nw::lyt::AnimTransform::GetFrameSize() const
{
    return m_pRes->frameSize;
}

} // namespace lyt
} // namespace nw
