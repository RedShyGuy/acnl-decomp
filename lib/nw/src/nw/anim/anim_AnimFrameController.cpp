#include "nw/anim/anim_AnimFrameController.h"

namespace nw {
namespace anim {
// 0x004D5254 slot 0x00 (name is ours)
// 0x004D5244 (deleting dtor)
nw::anim::AnimFrameController::~AnimFrameController()
{
    // empty (in the original too: only the member AnimFrame, destroyed without a call)
}

} // namespace anim
} // namespace nw
