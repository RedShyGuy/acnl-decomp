#include "nw/anim/anim_Api.h"

#include <cmath>

namespace nw {
namespace anim {
// 0x004D5190 | nintendogs:bytes [tier A]
f32 PlayPolicy_Loop(f32 startFrame, f32 endFrame, f32 frame, void* /*pUserData*/)
{
    const f32 length = endFrame - startFrame;
    if (length == 0.0f) {
        return startFrame;
    }
    f32 offset = frame - startFrame;
    if (offset < 0.0f) {
        offset += (std::floor(-offset / length) + 1.0f) * length;
    }
    return std::fmod(offset, length) + startFrame;
}

// 0x004D521C | nintendogs:bytes [tier A]
f32 PlayPolicy_Onetime(f32 startFrame, f32 endFrame, f32 frame, void* /*pUserData*/)
{
    if (frame > endFrame) {
        return endFrame;
    }
    if (frame < startFrame) {
        return startFrame;
    }
    return frame;
}

} // namespace anim
} // namespace nw
