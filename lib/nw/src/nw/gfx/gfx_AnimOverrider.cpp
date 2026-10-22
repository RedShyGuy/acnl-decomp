#include "nw/gfx/gfx_AnimOverrider.h"

namespace nw {
namespace gfx {
// 0x00976054
const TypeInfo nw::gfx::AnimOverrider::s_TypeInfo(&AnimBlender::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C17C (static initializer)

// 0x00492A38 slot 0x00 | slot vf_0x00 of nw::gfx::AnimBlender
// 0x004929FC (deleting dtor)
nw::gfx::AnimOverrider::~AnimOverrider()
{
    // empty (in the original too)
}

} // namespace gfx
} // namespace nw
